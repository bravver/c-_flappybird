#ifndef GAMEBACKGROUND_H
#define GAMEBACKGROUND_H

#include <QObject>
#include <QPainter>
#include <QPixmap>
#include "constant.h"

class Bird;

class GameBackground : public QObject {
    Q_OBJECT

public:
    GameBackground(QObject *parent = nullptr);
    ~GameBackground();

    void draw(QPainter &painter, Bird *bird);

    static int GROUND_HEIGHT;

private:
    QPixmap backgroundImage;
    int layerX;
    int speed;
};

#endif // GAMEBACKGROUND_H
