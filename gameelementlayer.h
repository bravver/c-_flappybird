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
    void pipeBornLogic(Bird *bird);
    void addNormalPipe();
    void addHoverPipe();
    void addMovingNormalPipe();
    void addMovingHoverPipe();
    void isCollideBird(Bird *bird);

    QVector<Pipe*> pipes;
    ScoreCounter *counter;
};

#endif // GAMEELEMENTLAYER_H
