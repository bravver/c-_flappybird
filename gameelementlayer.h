#ifndef GAMEELEMENTLAYER_H
#define GAMEELEMENTLAYER_H

#include <QObject>
#include <QVector>
#include "pipe.h"
#include "pipepool.h"
#include "constant.h"

class Bird;
class ScoreCounter;

class GameElementLayer : public QObject {
    Q_OBJECT

public:
    GameElementLayer(QObject *parent = nullptr);
    ~GameElementLayer();

    void draw(QPainter &painter, Bird *bird);
    void reset();

private:
    void checkScore(Bird *bird);
    void pipeBornLogic(Bird *bird);
    void addNormalPipe(int x);
    void addNormalPipe();
    void addHoverPipe();
    void addMovingNormalPipe();
    void addMovingHoverPipe();
    void isCollideBird(Bird *bird);

    QVector<Pipe*> pipes;
    ScoreCounter *counter;
    int lastScoreX;  // 记录上次得分的管道X坐标，避免重复得分
};

#endif // GAMEELEMENTLAYER_H
