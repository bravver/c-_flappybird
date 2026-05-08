#include "gameoveranimation.h"
#include "bird.h"
#include "scorecounter.h"
#include "constant.h"
#include <QFontMetrics>

GameOverAnimation::GameOverAnimation(QObject *parent)
    : QObject(parent)
    , flash(0) {

    overImage = QPixmap(Constant::OVER_IMG_PATH);
    againImage = QPixmap(Constant::AGAIN_IMG_PATH);
    scoreImage = QPixmap(Constant::SCORE_IMG_PATH);
}

GameOverAnimation::~GameOverAnimation() {}

void GameOverAnimation::draw(QPainter &painter, Bird *bird) {
    Q_UNUSED(bird);
    // 绘制 Game Over
    int overX = (Constant::FRAME_WIDTH - overImage.width()) / 2;
    int overY = Constant::FRAME_HEIGHT / 4;
    painter.drawPixmap(overX, overY, overImage);

    // 绘制分数面板
    int scoreX = (Constant::FRAME_WIDTH - scoreImage.width()) / 2;
    int scoreY = overY + overImage.height() + 20;
    painter.drawPixmap(scoreX, scoreY, scoreImage);

    // 在分数面板上绘制分数文字
    ScoreCounter *counter = ScoreCounter::getInstance();

    QFont font("Arial", 24, QFont::Bold);
    painter.setFont(font);
    painter.setPen(Qt::black);

    // 当前分数 - 居中显示在分数面板左半边
    QString currentStr = QString::number(counter->getCurrentScore());
    int halfPanelWidth = scoreImage.width() / 2;
    int currentX = scoreX + halfPanelWidth / 2;
    currentX -= QFontMetrics(font).horizontalAdvance(currentStr) / 2;
    painter.drawText(currentX, scoreY + 35, currentStr);

    // 最高分 - 居中显示在分数面板右半边
    QString bestStr = QString::number(counter->getBestScore());
    int bestX = scoreX + halfPanelWidth + halfPanelWidth / 2;
    bestX -= QFontMetrics(font).horizontalAdvance(bestStr) / 2;
    painter.drawText(bestX, scoreY + 35, bestStr);

    // 绘制再来一次 - 图像闪烁
    const int COUNT = 30;
    if (flash++ > COUNT) {
        int againX = (Constant::FRAME_WIDTH - againImage.width()) / 2;
        int againY = Constant::FRAME_HEIGHT / 5 * 3;
        painter.drawPixmap(againX, againY, againImage);
    }
    if (flash == COUNT * 2) {
        flash = 0;
    }
}
