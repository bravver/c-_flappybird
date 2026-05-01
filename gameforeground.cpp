#include "gameforeground.h"
#include "gamebackground.h"
#include "constant.h"
#include <QPainter>

GameForeground::GameForeground(QObject *parent)
    : QObject(parent) {
    foregroundImage = QPixmap(Constant::BG_IMG_PATH);
}

GameForeground::~GameForeground() {}

void GameForeground::draw(QPainter &painter) {
    // 前景地面 - 使用背景图的下半部分
    int groundY = Constant::FRAME_HEIGHT - GameBackground::GROUND_HEIGHT;
    painter.drawPixmap(0, groundY, foregroundImage.copy(0, foregroundImage.height() / 2,
                                                         foregroundImage.width(),
                                                         foregroundImage.height() / 2));
}
