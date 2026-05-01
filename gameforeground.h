#ifndef GAMEFOREGROUND_H
#define GAMEFOREGROUND_H

#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QVector>
#include "constant.h"

class Cloud : public QObject {
    Q_OBJECT

public:
    Cloud(const QPixmap &image, int x, int y, QObject *parent = nullptr);
    ~Cloud();

    void draw(QPainter &painter);
    void movement();
    bool isOutFrame() const;
    int getX() const;

private:
    QPixmap image;
    int x, y;
    int speed;
};

class GameForeground : public QObject {
    Q_OBJECT

public:
    GameForeground(QObject *parent = nullptr);
    ~GameForeground();

    void draw(QPainter &painter);

private:
    void cloudBornLogic();

    QVector<Cloud*> clouds;
    QPixmap cloudImages[Constant::CLOUD_IMAGE_COUNT];
    int lastTime;
};

#endif // GAMEFOREGROUND_H
