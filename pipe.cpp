#include "pipe.h"
#include "bird.h"

int Pipe::PIPE_WIDTH = 0;
int Pipe::PIPE_HEIGHT = 0;
int Pipe::PIPE_HEAD_WIDTH = 0;
int Pipe::PIPE_HEAD_HEIGHT = 0;

Pipe::Pipe(QObject *parent)
    : QObject(parent)
    , type(TYPE_TOP_NORMAL)
    , speed(Constant::GAME_SPEED)
    , x(0), y(0)
    , width(0), height(0)
    , visible(false)
    , scored(false) {

    static bool initialized = false;
    if (!initialized) {
        pipeBodyImage = QPixmap(Constant::PIPE_IMG_PATH[0]);
        pipeTopImage = QPixmap(Constant::PIPE_IMG_PATH[1]);
        pipeBottomImage = QPixmap(Constant::PIPE_IMG_PATH[2]);

        PIPE_WIDTH = pipeBodyImage.width();
        PIPE_HEIGHT = pipeBodyImage.height();
        PIPE_HEAD_WIDTH = pipeTopImage.width();
        PIPE_HEAD_HEIGHT = pipeTopImage.height();

        initialized = true;
    } else {
        pipeBodyImage = QPixmap(Constant::PIPE_IMG_PATH[0]);
        pipeTopImage = QPixmap(Constant::PIPE_IMG_PATH[1]);
        pipeBottomImage = QPixmap(Constant::PIPE_IMG_PATH[2]);
    }

    width = PIPE_WIDTH;
    pipeRect.setWidth(PIPE_WIDTH);
}

Pipe::~Pipe() {}

void Pipe::setAttribute(int x, int y, int height, int type, bool visible) {
    this->x = x;
    this->y = y;
    this->height = height;
    this->type = type;
    this->visible = visible;
    setRectangle(this->x, this->y, this->height);
}

void Pipe::setRectangle(int x, int y, int height) {
    pipeRect.moveTo(x, y);
    pipeRect.setHeight(height);
}

void Pipe::draw(QPainter &painter, Bird *bird) {
    switch (type) {
    case TYPE_TOP_NORMAL:
        drawTopNormal(painter);
        break;
    case TYPE_BOTTOM_NORMAL:
        drawBottomNormal(painter);
        break;
    case TYPE_HOVER_NORMAL:
        drawHoverNormal(painter);
        break;
    case TYPE_TOP_HARD:
        drawTopHard(painter);
        break;
    case TYPE_BOTTOM_HARD:
        drawBottomHard(painter);
        break;
    case TYPE_HOVER_HARD:
        drawHoverHard(painter);
        break;
    }

    if (bird->isDead()) {
        return;
    }
    movement();
}

void Pipe::drawTopNormal(QPainter &painter) {
    int count = (height - PIPE_HEAD_HEIGHT) / PIPE_HEIGHT + 1;

    for (int i = 0; i < count; i++) {
        painter.drawPixmap(x, y + i * PIPE_HEIGHT, pipeBodyImage);
    }

    int headX = x - ((PIPE_HEAD_WIDTH - width) >> 1);
    painter.drawPixmap(headX, height - Constant::TOP_PIPE_LENGTHENING - PIPE_HEAD_HEIGHT, pipeTopImage);
}

void Pipe::drawBottomNormal(QPainter &painter) {
    int count = (height - PIPE_HEAD_HEIGHT - Constant::GROUND_HEIGHT) / PIPE_HEIGHT + 1;

    for (int i = 0; i < count; i++) {
        painter.drawPixmap(x, Constant::FRAME_HEIGHT - PIPE_HEIGHT - Constant::GROUND_HEIGHT - i * PIPE_HEIGHT,
                           pipeBodyImage);
    }

    int headX = x - ((PIPE_HEAD_WIDTH - width) >> 1);
    painter.drawPixmap(headX, Constant::FRAME_HEIGHT - height, pipeBottomImage);
}

void Pipe::drawHoverNormal(QPainter &painter) {
    int count = (height - 2 * PIPE_HEAD_HEIGHT) / PIPE_HEIGHT + 1;

    int headX = x - ((PIPE_HEAD_WIDTH - width) >> 1);
    painter.drawPixmap(headX, y, pipeBottomImage);

    for (int i = 0; i < count; i++) {
        painter.drawPixmap(x, y + i * PIPE_HEIGHT + PIPE_HEAD_HEIGHT, pipeBodyImage);
    }

    painter.drawPixmap(headX, y + height - PIPE_HEAD_HEIGHT, pipeTopImage);
}

void Pipe::movement() {
    x -= speed;
    pipeRect.translate(-speed, 0);

    if (x < -1 * PIPE_HEAD_WIDTH) {
        visible = false;
    }
}

bool Pipe::isVisible() const {
    return visible;
}

bool Pipe::isInFrame() const {
    return x + width < Constant::FRAME_WIDTH;
}

int Pipe::getX() const {
    return x;
}

int Pipe::getType() const {
    return type;
}

QRect Pipe::getPipeRect() const {
    return pipeRect;
}

bool Pipe::isScored() const {
    return scored;
}

void Pipe::setScored(bool scored) {
    this->scored = scored;
}

void Pipe::drawTopHard(QPainter &painter) {
    drawTopNormal(painter);
}

void Pipe::drawBottomHard(QPainter &painter) {
    drawBottomNormal(painter);
}

void Pipe::drawHoverHard(QPainter &painter) {
    drawHoverNormal(painter);
}