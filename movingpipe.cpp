#include "movingpipe.h"
#include "bird.h"
#include "constant.h"

MovingPipe::MovingPipe(QObject *parent)
    : Pipe(parent)
    , dealtY(0)
    , direction(DIR_DOWN) {
}

MovingPipe::~MovingPipe() {}

void MovingPipe::setAttribute(int x, int y, int height, int type, bool visible) {
    Pipe::setAttribute(x, y, height, type, visible);
    dealtY = 0;
    direction = (type == TYPE_TOP_HARD) ? DIR_UP : DIR_DOWN;
}

void MovingPipe::draw(QPainter &painter, Bird *bird) {
    switch (type) {
    case TYPE_HOVER_HARD:
        drawHoverHard(painter);
        break;
    case TYPE_TOP_HARD:
        drawTopHard(painter);
        break;
    case TYPE_BOTTOM_HARD:
        drawBottomHard(painter);
        break;
    default:
        break;
    }

    if (bird->isDead()) {
        return;
    }
    movement();
}

void MovingPipe::drawTopHard(QPainter &painter) {
    int count = (height - PIPE_HEAD_HEIGHT) / PIPE_HEIGHT + 1;

    for (int i = 0; i < count; i++) {
        painter.drawPixmap(x, y + dealtY + i * PIPE_HEIGHT, pipeBodyImage);
    }

    int headX = x - ((PIPE_HEAD_WIDTH - width) >> 1);
    painter.drawPixmap(headX, height - Constant::TOP_PIPE_LENGTHENING - PIPE_HEAD_HEIGHT + dealtY, pipeTopImage);
}

void MovingPipe::drawBottomHard(QPainter &painter) {
    int count = (height - PIPE_HEAD_HEIGHT) / PIPE_HEIGHT + 1;

    for (int i = 0; i < count; i++) {
        painter.drawPixmap(x, Constant::FRAME_HEIGHT - PIPE_HEIGHT - i * PIPE_HEIGHT + dealtY,
                           pipeBodyImage);
    }

    int headX = x - ((PIPE_HEAD_WIDTH - width) >> 1);
    painter.drawPixmap(headX, Constant::FRAME_HEIGHT - height + dealtY, pipeBottomImage);
}

void MovingPipe::drawHoverHard(QPainter &painter) {
    int count = (height - 2 * PIPE_HEAD_HEIGHT) / PIPE_HEIGHT + 1;

    int headX = x - ((PIPE_HEAD_WIDTH - width) >> 1);
    painter.drawPixmap(headX, y + dealtY, pipeBottomImage);

    for (int i = 0; i < count; i++) {
        painter.drawPixmap(x, y + dealtY + i * PIPE_HEIGHT + PIPE_HEAD_HEIGHT, pipeBodyImage);
    }

    painter.drawPixmap(headX, y + height - PIPE_HEAD_HEIGHT + dealtY, pipeTopImage);
}

void MovingPipe::movement() {
    // 水平移动（与普通管道相同）
    x -= speed;
    pipeRect.translate(-speed, 0);
    if (x < -1 * PIPE_HEAD_WIDTH) {
        visible = false;
    }

    // 垂直往复移动
    if (direction == DIR_DOWN) {
        dealtY++;
        if (dealtY > MAX_DELTA) {
            direction = DIR_UP;
        }
    } else {
        dealtY--;
        if (dealtY <= 0) {
            direction = DIR_DOWN;
        }
    }
    pipeRect.moveTo(x, y + dealtY);
}
