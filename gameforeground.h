#ifndef GAMEFOREGROUND_H
#define GAMEFOREGROUND_H

#include <QObject>
#include <QPainter>
#include <QPixmap>

class GameForeground : public QObject {
    Q_OBJECT

public:
    GameForeground(QObject *parent = nullptr);
    ~GameForeground();

    void draw(QPainter &painter);

private:
    QPixmap foregroundImage;
};

#endif // GAMEFOREGROUND_H
