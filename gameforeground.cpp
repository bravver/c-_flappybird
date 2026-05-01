#include "gameforeground.h"
#include <QRandomGenerator>
#include <QDateTime>

Cloud::Cloud(const QPixmap &image, int x, int y, QObject *parent)
    : QObject(parent)
    , image(image)
    , x(x)
    , y(y)
    , speed(Constant::GAME_SPEED / 2) {
}

Cloud::~Cloud() {}

void Cloud::draw(QPainter &painter) {
    painter.drawPixmap(x, y, image);
}

void Cloud::movement() {
    x -= speed;
}

bool Cloud::isOutFrame() const {
    return x < -image.width();
}

int Cloud::getX() const {
    return x;
}

GameForeground::GameForeground(QObject *parent)
    : QObject(parent)
    , lastTime(0) {

    for (int i = 0; i < Constant::CLOUD_IMAGE_COUNT; i++) {
        cloudImages[i] = QPixmap(Constant::CLOUDS_IMG_PATH[i]);
    }
    lastTime = QDateTime::currentMSecsSinceEpoch();
}

GameForeground::~GameForeground() {
    qDeleteAll(clouds);
}

void GameForeground::draw(QPainter &painter) {
    cloudBornLogic();

    for (Cloud *cloud : clouds) {
        cloud->draw(painter);
        cloud->movement();
    }

    // 移除飞出屏幕的云
    for (int i = 0; i < clouds.size(); i++) {
        if (clouds[i]->isOutFrame()) {
            delete clouds[i];
            clouds.removeAt(i);
            i--;
        }
    }
}

void GameForeground::cloudBornLogic() {
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    const int CLOUD_INTERVAL = 100;

    if (currentTime - lastTime > CLOUD_INTERVAL) {
        lastTime = currentTime;

        if (clouds.size() < Constant::MAX_CLOUD_COUNT) {
            int percent = QRandomGenerator::global()->bounded(100);
            if (percent < Constant::CLOUD_BORN_PERCENT) {
                int index = QRandomGenerator::global()->bounded(Constant::CLOUD_IMAGE_COUNT);
                int x = Constant::FRAME_WIDTH;
                int y = QRandomGenerator::global()->bounded(
                    Constant::TOP_BAR_HEIGHT, Constant::FRAME_HEIGHT / 3);

                clouds.append(new Cloud(cloudImages[index], x, y, this));
            }
        }
    }
}
