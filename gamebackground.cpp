#include "gamebackground.h"
#include "bird.h"

int GameBackground::GROUND_HEIGHT = 0;

GameBackground::GameBackground(QObject *parent)
    : QObject(parent)
    , layerX(0)
    , speed(Constant::GAME_SPEED) {

    backgroundImage = QPixmap(Constant::BG_IMG_PATH);
    GROUND_HEIGHT = backgroundImage.height() / 2;
}

GameBackground::~GameBackground() {}

void GameBackground::draw(QPainter &painter, Bird *bird) {
    // 绘制背景色
    painter.fillRect(0, 0, Constant::FRAME_WIDTH, Constant::FRAME_HEIGHT, QColor(0x4b, 0xc4, 0xcf));

    int imgWidth = backgroundImage.width();
    int imgHeight = backgroundImage.height();

    // 绘制完整的背景图（天空+地面），从窗口底部开始
    // 这样地面部分正好显示在窗口底部
    int count = Constant::FRAME_WIDTH / imgWidth + 2;

    for (int i = 0; i < count; i++) {
        painter.drawPixmap(imgWidth * i - layerX,
                          Constant::FRAME_HEIGHT - imgHeight,
                          backgroundImage);
    }

    if (bird->isDead()) {
        return;
    }

    // 滚动背景（天空和地面一起滚动）
    layerX += speed;
    if (layerX > imgWidth) {
        layerX = 0;
    }
}
