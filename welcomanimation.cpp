#include "welcomanimation.h"
#include "constant.h"

WelcomeAnimation::WelcomeAnimation(QObject *parent)
    : QObject(parent) {

    titleImage = QPixmap(Constant::TITLE_IMG_PATH);
    noticeImage = QPixmap(Constant::NOTICE_IMG_PATH);
}

WelcomeAnimation::~WelcomeAnimation() {}

void WelcomeAnimation::draw(QPainter &painter) {
    // 绘制标题
    int titleX = (Constant::FRAME_WIDTH - titleImage.width()) / 2;
    int titleY = Constant::FRAME_HEIGHT / 5;
    painter.drawPixmap(titleX, titleY, titleImage);

    // 绘制开始提示
    int noticeX = (Constant::FRAME_WIDTH - noticeImage.width()) / 2;
    int noticeY = Constant::FRAME_HEIGHT * 3 / 4;
    painter.drawPixmap(noticeX, noticeY, noticeImage);
}
