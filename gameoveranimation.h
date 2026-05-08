#ifndef GAMEOVERANIMATION_H
#define GAMEOVERANIMATION_H

#include <QObject>
#include <QPainter>
#include <QPixmap>

class Bird;

class GameOverAnimation : public QObject {
    Q_OBJECT

public:
    GameOverAnimation(QObject *parent = nullptr);
    ~GameOverAnimation();

    void draw(QPainter &painter, Bird *bird);

private:
    QPixmap overImage;
    QPixmap againImage;
    QPixmap scoreImage;
    int flash;
};

#endif // GAMEOVERANIMATION_H
