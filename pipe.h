#ifndef PIPE_H
#define PIPE_H

#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include "constant.h"

class Bird;

class Pipe : public QObject {
    Q_OBJECT

public:
    // 水管类型
    enum Type {
        TYPE_TOP_NORMAL = 0,
        TYPE_TOP_HARD = 1,
        TYPE_BOTTOM_NORMAL = 2,
        TYPE_BOTTOM_HARD = 3,
        TYPE_HOVER_NORMAL = 4,
        TYPE_HOVER_HARD = 5
    };

    Pipe(QObject *parent = nullptr);
    ~Pipe();

    virtual void setAttribute(int x, int y, int height, int type, bool visible);
    virtual void setRectangle(int x, int y, int height);

    virtual void draw(QPainter &painter, Bird *bird);
    bool isVisible() const;
    bool isInFrame() const;

    int getX() const;
    int getType() const;
    QRect getPipeRect() const;
    bool isScored() const;
    void setScored(bool scored);

    static int PIPE_WIDTH;
    static int PIPE_HEIGHT;
    static int PIPE_HEAD_WIDTH;
    static int PIPE_HEAD_HEIGHT;

    int type;
    int speed;

protected:
    virtual void movement();

    int x, y;
    int width, height;
    bool visible;
    QRect pipeRect;
    QPixmap pipeBodyImage;
    QPixmap pipeTopImage;
    QPixmap pipeBottomImage;

private:
    void drawTopNormal(QPainter &painter);
    void drawBottomNormal(QPainter &painter);
    void drawHoverNormal(QPainter &painter);

    bool scored;
};

#endif // PIPE_H