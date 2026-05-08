#ifndef MOVINGPIPE_H
#define MOVINGPIPE_H

#include "pipe.h"

class MovingPipe : public Pipe {
    Q_OBJECT

public:
    MovingPipe(QObject *parent = nullptr);
    ~MovingPipe();

    void setAttribute(int x, int y, int height, int type, bool visible) override;
    void draw(QPainter &painter, Bird *bird) override;

    static constexpr int MAX_DELTA = 50;

private:
    void movement() override;
    void drawTopHard(QPainter &painter);
    void drawBottomHard(QPainter &painter);
    void drawHoverHard(QPainter &painter);

    int dealtY;
    int direction;
    static constexpr int DIR_UP = 0;
    static constexpr int DIR_DOWN = 1;
};

#endif // MOVINGPIPE_H
