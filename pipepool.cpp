#include "pipepool.h"

PipePool* PipePool::instance = nullptr;

PipePool::PipePool(QObject *parent)
    : QObject(parent) {
    // 预创建一些水管
    for (int i = 0; i < FULL_PIPE; i++) {
        normalPipes.append(new Pipe());
        movingPipes.append(new Pipe());
    }
}

PipePool::~PipePool() {
    qDeleteAll(normalPipes);
    qDeleteAll(movingPipes);
}

PipePool* PipePool::getInstance() {
    if (!instance) {
        instance = new PipePool();
    }
    return instance;
}

void PipePool::destroy() {
    delete instance;
    instance = nullptr;
}

Pipe* PipePool::get(const QString &type) {
    Pipe* pipe;
    if (type == "MovingPipe") {
        if (!movingPipes.isEmpty()) {
            pipe = movingPipes.takeLast();
        } else {
            pipe = new Pipe();
        }
    } else {
        if (!normalPipes.isEmpty()) {
            pipe = normalPipes.takeLast();
        } else {
            pipe = new Pipe();
        }
    }
    // 重置计分标志
    pipe->setScored(false);
    return pipe;
}

void PipePool::giveBack(Pipe *pipe) {
    if (pipe->type == Pipe::TYPE_TOP_HARD || pipe->type == Pipe::TYPE_BOTTOM_HARD ||
        pipe->type == Pipe::TYPE_HOVER_HARD) {
        movingPipes.append(pipe);
    } else {
        normalPipes.append(pipe);
    }
}