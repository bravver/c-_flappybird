#include "scorecounter.h"
#include <QFile>
#include <QTextStream>

ScoreCounter* ScoreCounter::instance = nullptr;

ScoreCounter::ScoreCounter(QObject *parent)
    : QObject(parent)
    , currentScore(0)
    , bestScore(0)
    , scoreSound(new QSoundEffect(this)) {

    // 从文件加载最高分
    QFile file("score.txt");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in >> bestScore;
        file.close();
    }

    // 初始化得分音效
    scoreSound->setSource(QUrl("qrc:/resources/wav/score.wav"));
}

ScoreCounter::~ScoreCounter() {}

ScoreCounter* ScoreCounter::getInstance() {
    if (!instance) {
        instance = new ScoreCounter();
    }
    return instance;
}

void ScoreCounter::score() {
    currentScore++;
    scoreSound->play();
}

void ScoreCounter::reset() {
    currentScore = 0;
}

void ScoreCounter::saveScore() {
    if (currentScore > bestScore) {
        bestScore = currentScore;

        QFile file("score.txt");
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream out(&file);
            out << bestScore;
            file.close();
        }
    }
}

long ScoreCounter::getCurrentScore() const {
    return currentScore;
}

long ScoreCounter::getBestScore() const {
    return bestScore;
}