#ifndef SCORECOUNTER_H
#define SCORECOUNTER_H

#include <QObject>
#include <QSoundEffect>

class ScoreCounter : public QObject {
    Q_OBJECT

public:
    static ScoreCounter* getInstance();

    void score();
    void reset();
    void saveScore();

    long getCurrentScore() const;
    long getBestScore() const;

private:
    ScoreCounter(QObject *parent = nullptr);
    ~ScoreCounter();

    static ScoreCounter *instance;

    long currentScore;
    long bestScore;
    QSoundEffect *scoreSound;
};

#endif // SCORECOUNTER_H