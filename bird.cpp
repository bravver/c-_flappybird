#include "bird.h"
#include <QPainter>
#include <QDebug>

int Bird::BIRD_WIDTH = 0;
int Bird::BIRD_HEIGHT = 0;

Bird::Bird(QObject *parent)
    : QObject(parent)
    , x(Constant::FRAME_WIDTH >> 2)
    , y(Constant::FRAME_HEIGHT >> 1)
    , wingState(0)
    , state(BIRD_NORMAL)
    , velocity(0)
    , counter(ScoreCounter::getInstance())
    , keyFlag(true)
    , flySound(new QSoundEffect(this))
    , crashSound(new QSoundEffect(this)) {

    // 加载普通飞行帧
    for (int i = 0; i < 8; i++) {
        QPixmap pix(Constant::BIRDS_IMG_PATH[i]);
        normalImages.append(pix);
    }

    // 加载上升帧
    QPixmap upPix(Constant::BIRDS_IMG_PATH[8]);
    for (int i = 0; i < 8; i++) {
        upImages.append(upPix);
    }

    // 加载下落帧
    for (int i = 9; i < 17; i++) {
        QPixmap pix(Constant::BIRDS_IMG_PATH[i]);
        fallImages.append(pix);
    }

    // 加载死亡帧
    deadImage = QPixmap(Constant::BIRDS_IMG_PATH[17]);

    // 设置尺寸
    if (!normalImages.isEmpty()) {
        BIRD_WIDTH = normalImages[0].width();
        BIRD_HEIGHT = normalImages[0].height();
    }

    // 初始化碰撞矩形
    int rectX = x - BIRD_WIDTH / 2;
    int rectY = y - BIRD_HEIGHT / 2;
    collisionRect = QRect(rectX + RECT_DESCALE, rectY + RECT_DESCALE * 2,
                          BIRD_WIDTH - RECT_DESCALE * 3,
                          BIRD_WIDTH - RECT_DESCALE * 4);

    // 初始化音效
    flySound->setSource(QUrl("qrc:/resources/wav/fly.wav"));
    crashSound->setSource(QUrl("qrc:/resources/wav/crash.wav"));
}

Bird::~Bird() {}

void Bird::draw(QPainter &painter) {
    movement();

    // 选择当前帧图片
    QPixmap image;
    if (velocity > 0 && state == BIRD_UP) {
        image = upImages[0];
    } else {
        int stateIndex = qMin(state, BIRD_DEAD_FALL);
        if (stateIndex == BIRD_NORMAL || stateIndex == BIRD_FALL) {
            image = normalImages[wingState / 10 % 8];
        } else if (stateIndex == BIRD_UP) {
            image = upImages[0];
        } else if (stateIndex == BIRD_DEAD_FALL) {
            image = fallImages[wingState / 10 % 8];
        } else {
            image = deadImage;
        }
    }

    // 绘制小鸟（居中）
    int halfWidth = image.width() / 2;
    int halfHeight = image.height() / 2;
    painter.drawPixmap(x - halfWidth, y - halfHeight, image);

    if (state == BIRD_DEAD) {
        // 结束动画稍后添加
    } else if (state != BIRD_DEAD_FALL) {
        drawScore(painter);
    }
}

void Bird::movement() {
    wingState++;

    if (state == BIRD_FALL || state == BIRD_DEAD_FALL) {
        freeFall();
        if (collisionRect.y() > (Constant::FRAME_HEIGHT - 35 - BIRD_HEIGHT / 2)) {
            if (state == BIRD_FALL) {
                crashSound->play();
            }
            die();
        }
    }
}

void Bird::freeFall() {
    if (velocity < MAX_VEL_Y) {
        velocity -= ACC_Y;
    }
    y = qMin(y - velocity, Constant::FRAME_HEIGHT - 35 - BIRD_HEIGHT / 2);
    collisionRect.translate(0, -velocity);
}

void Bird::die() {
    counter->saveScore();
    state = BIRD_DEAD;
    Game::setGameState(Game::STATE_OVER);
}

void Bird::birdFlap() {
    if (keyIsReleased()) {
        if (isDead()) return;

        flySound->play();
        state = BIRD_UP;
        if (collisionRect.y() > Constant::TOP_BAR_HEIGHT) {
            velocity = ACC_FLAP;
            wingState = 0;
        }
        keyPressed();
    }
}

void Bird::birdFall() {
    if (isDead()) return;
    state = BIRD_FALL;
}

void Bird::deadBirdFall() {
    state = BIRD_DEAD_FALL;
    velocity = 0;
}

void Bird::reset() {
    state = BIRD_NORMAL;
    y = Constant::FRAME_HEIGHT >> 1;
    velocity = 0;

    int imgHeight = normalImages[0].height();
    collisionRect.moveTo(x - BIRD_WIDTH / 2 + RECT_DESCALE,
                         y - imgHeight / 2 + RECT_DESCALE * 2);

    counter->reset();
}

bool Bird::isDead() const {
    return state == BIRD_DEAD_FALL || state == BIRD_DEAD;
}

void Bird::keyPressed() {
    keyFlag = false;
}

void Bird::keyReleased() {
    keyFlag = true;
}

bool Bird::keyIsReleased() const {
    return keyFlag;
}

int Bird::getBirdX() const {
    return x;
}

int Bird::getBirdY() const {
    return y;
}

QRect Bird::getCollisionRect() const {
    return collisionRect;
}

void Bird::drawScore(QPainter &painter) {
    painter.setPen(Qt::white);
    QFont font("Arial", 24, QFont::Bold);
    painter.setFont(font);
    QString str = QString::number(counter->getCurrentScore());
    int textWidth = QFontMetrics(font).horizontalAdvance(str);
    painter.drawText(Constant::FRAME_WIDTH / 2 - textWidth / 2,
                     Constant::FRAME_HEIGHT / 10, str);
}
