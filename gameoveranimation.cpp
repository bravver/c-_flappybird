#include "gameoveranimation.h"
#include "bird.h"
#include "scorecounter.h"
#include "constant.h"

GameOverAnimation::GameOverAnimation(QObject *parent)
    : QObject(parent) {

    overImage = QPixmap(Constant::OVER_IMG_PATH);
    againImage = QPixmap(Constant::AGAIN_IMG_PATH);
    scoreImage = QPixmap(Constant::SCORE_IMG_PATH);
}

GameOverAnimation::~GameOverAnimation() {}

void GameOverAnimation::draw(QPainter &painter, Bird *bird) {
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

    // 当前分数
    QString currentStr = QString::number(counter->getCurrentScore());
    painter.drawText(scoreX + 50, scoreY + 35, currentStr);

    // 最高分
    QString bestStr = QString::number(counter->getBestScore());
    painter.drawText(scoreX + 170, scoreY + 35, bestStr);

    // 绘制再来一次
    int againX = (Constant::FRAME_WIDTH - againImage.width()) / 2;
    int againY = scoreY + scoreImage.height() + 20;
    painter.drawPixmap(againX, againY, againImage);
}
