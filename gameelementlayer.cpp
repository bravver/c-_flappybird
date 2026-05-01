#include "gameelementlayer.h"
#include "bird.h"
#include "scorecounter.h"
#include <QRandomGenerator>
#include <QDebug>

GameElementLayer::GameElementLayer(QObject *parent)
    : QObject(parent)
    , counter(ScoreCounter::getInstance())
    , lastScoreX(-1) {
}

GameElementLayer::~GameElementLayer() {
    reset();
}

void GameElementLayer::draw(QPainter &painter, Bird *bird) {
    // 遍历水管容器
    for (int i = 0; i < pipes.size(); i++) {
        Pipe *pipe = pipes[i];
        if (pipe->isVisible()) {
            pipe->draw(painter, bird);
        } else {
            PipePool::getInstance()->giveBack(pipe);
            pipes.removeAt(i);
            i--;
        }
    }

    // 碰撞检测
    isCollideBird(bird);

    // 得分检测 - 当鸟穿过管道时
    checkScore(bird);

    // 生成新水管
    pipeBornLogic(bird);
}

void GameElementLayer::checkScore(Bird *bird) {
    if (bird->isDead()) {
        return;
    }

    int birdX = bird->getBirdX();

    for (Pipe *pipe : pipes) {
        // 检测上管道是否被越过（得分点）
        if (pipe->getType() == Pipe::TYPE_TOP_NORMAL ||
            pipe->getType() == Pipe::TYPE_TOP_HARD) {
            int pipeX = pipe->getX();

            // 如果鸟的X坐标刚越过管道（从左到右穿过）
            // 并且这个管道还没被计过分
            if (lastScoreX != pipeX && birdX > pipeX + Pipe::PIPE_WIDTH) {
                counter->score();
                lastScoreX = pipeX;
            }
        }
    }
}

void GameElementLayer::pipeBornLogic(Bird *bird) {
    if (bird->isDead()) {
        return;
    }

    if (pipes.isEmpty()) {
        // 第一个管道对
        addNormalPipe(Constant::FRAME_WIDTH + 100);
    } else {
        Pipe *lastPipe = pipes.last();

        // 检查是否应该生成新管道
        // 当最后一个管道的X坐标小于某个阈值时生成新管道
        if (lastPipe->getX() < Constant::FRAME_WIDTH - Constant::HORIZONTAL_INTERVAL) {
            // 根据分数决定管道类型
            int currentScore = static_cast<int>(counter->getCurrentScore());

            if (currentScore > 19 || QRandomGenerator::global()->bounded(20) < currentScore) {
                if (QRandomGenerator::global()->bounded(4) == 0) {
                    addMovingHoverPipe();
                } else {
                    addMovingNormalPipe();
                }
            } else {
                if (QRandomGenerator::global()->bounded(2) == 0) {
                    addNormalPipe();
                } else {
                    addHoverPipe();
                }
            }
        }
    }
}

void GameElementLayer::addNormalPipe(int x) {
    int topHeight = QRandomGenerator::global()->bounded(
        Constant::MIN_PIPE_HEIGHT, Constant::MAX_PIPE_HEIGHT + 1);

    Pipe *top = PipePool::getInstance()->get("Pipe");
    top->setAttribute(x, -Constant::TOP_PIPE_LENGTHENING,
                     topHeight + Constant::TOP_PIPE_LENGTHENING,
                     Pipe::TYPE_TOP_NORMAL, true);

    Pipe *bottom = PipePool::getInstance()->get("Pipe");
    bottom->setAttribute(x, topHeight + Constant::VERTICAL_INTERVAL,
                        Constant::FRAME_HEIGHT - topHeight - Constant::VERTICAL_INTERVAL,
                        Pipe::TYPE_BOTTOM_NORMAL, true);

    pipes.append(top);
    pipes.append(bottom);
}

void GameElementLayer::addNormalPipe() {
    if (pipes.isEmpty()) return;
    Pipe *lastPipe = pipes.last();
    int x = lastPipe->getX() + Constant::HORIZONTAL_INTERVAL;
    addNormalPipe(x);
}

void GameElementLayer::addHoverPipe() {
    if (pipes.isEmpty()) return;
    Pipe *lastPipe = pipes.last();
    int x = lastPipe->getX() + Constant::HORIZONTAL_INTERVAL;

    int topHoverHeight = QRandomGenerator::global()->bounded(
        Constant::FRAME_HEIGHT / 6, Constant::FRAME_HEIGHT / 4);
    int y = QRandomGenerator::global()->bounded(
        Constant::FRAME_HEIGHT / 12, Constant::FRAME_HEIGHT / 6);

    Pipe *topHover = PipePool::getInstance()->get("Pipe");
    topHover->setAttribute(x, y, topHoverHeight, Pipe::TYPE_HOVER_NORMAL, true);

    int bottomHoverHeight = Constant::FRAME_HEIGHT - 2 * y - topHoverHeight - Constant::VERTICAL_INTERVAL;
    Pipe *bottomHover = PipePool::getInstance()->get("Pipe");
    bottomHover->setAttribute(x, y + topHoverHeight + Constant::VERTICAL_INTERVAL,
                             bottomHoverHeight, Pipe::TYPE_HOVER_NORMAL, true);

    pipes.append(topHover);
    pipes.append(bottomHover);
}

void GameElementLayer::addMovingNormalPipe() {
    if (pipes.isEmpty()) return;
    Pipe *lastPipe = pipes.last();
    int x = lastPipe->getX() + Constant::HORIZONTAL_INTERVAL;

    int topHeight = QRandomGenerator::global()->bounded(
        Constant::MIN_PIPE_HEIGHT, Constant::MAX_PIPE_HEIGHT + 1);

    Pipe *top = PipePool::getInstance()->get("MovingPipe");
    top->setAttribute(x, -Constant::TOP_PIPE_LENGTHENING,
                     topHeight + Constant::TOP_PIPE_LENGTHENING,
                     Pipe::TYPE_TOP_HARD, true);

    Pipe *bottom = PipePool::getInstance()->get("MovingPipe");
    bottom->setAttribute(x, topHeight + Constant::VERTICAL_INTERVAL,
                        Constant::FRAME_HEIGHT - topHeight - Constant::VERTICAL_INTERVAL,
                        Pipe::TYPE_BOTTOM_HARD, true);

    pipes.append(top);
    pipes.append(bottom);
}

void GameElementLayer::addMovingHoverPipe() {
    if (pipes.isEmpty()) return;
    Pipe *lastPipe = pipes.last();
    int x = lastPipe->getX() + Constant::HORIZONTAL_INTERVAL;

    int topHoverHeight = QRandomGenerator::global()->bounded(
        Constant::FRAME_HEIGHT / 6, Constant::FRAME_HEIGHT / 4);
    int y = QRandomGenerator::global()->bounded(
        Constant::FRAME_HEIGHT / 12, Constant::FRAME_HEIGHT / 6);

    Pipe *topHover = PipePool::getInstance()->get("MovingPipe");
    topHover->setAttribute(x, y, topHoverHeight, Pipe::TYPE_HOVER_HARD, true);

    int bottomHoverHeight = Constant::FRAME_HEIGHT - 2 * y - topHoverHeight - Constant::VERTICAL_INTERVAL;
    Pipe *bottomHover = PipePool::getInstance()->get("MovingPipe");
    bottomHover->setAttribute(x, y + topHoverHeight + Constant::VERTICAL_INTERVAL,
                              bottomHoverHeight, Pipe::TYPE_HOVER_HARD, true);

    pipes.append(topHover);
    pipes.append(bottomHover);
}

void GameElementLayer::isCollideBird(Bird *bird) {
    if (bird->isDead()) {
        return;
    }

    for (Pipe *pipe : pipes) {
        if (pipe->getPipeRect().intersects(bird->getCollisionRect())) {
            bird->deadBirdFall();
            return;
        }
    }
}

void GameElementLayer::reset() {
    for (Pipe *pipe : pipes) {
        PipePool::getInstance()->giveBack(pipe);
    }
    pipes.clear();
    lastScoreX = -1;
}
