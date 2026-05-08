#ifndef GAMEFOREGROUND_H
#define GAMEFOREGROUND_H

#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QVector>
#include "constant.h"
#include "bird.h"

class Cloud : public QObject {
    Q_OBJECT

public:
    Cloud(const QPixmap &image, int x, int y, QObject *parent = nullptr);
    ~Cloud();

    void draw(QPainter &painter, Bird *bird);
    bool isOutFrame() const;
    int getX() const;

private:
    QPixmap image;
    int x, y;
    int speed;
    int scaledWidth;
    int scaledHeight;
};

class GameForeground : public QObject {
    Q_OBJECT

public:
    GameForeground(QObject *parent = nullptr);
    ~GameForeground();

    void draw(QPainter &painter, Bird *bird);

private:
    void cloudBornLogic();

    QVector<Cloud*> clouds;
    QPixmap cloudImages[Constant::CLOUD_IMAGE_COUNT];
    int lastTime;
};

#endif // GAMEFOREGROUND_H
