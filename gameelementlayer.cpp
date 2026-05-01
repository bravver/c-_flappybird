#include "gameelementlayer.h"
#include "bird.h"
#include "scorecounter.h"
#include <QRandomGenerator>
#include <QDebug>

GameElementLayer::GameElementLayer(QObject *parent)
    : QObject(parent)
    , counter(ScoreCounter::getInstance())
    , lastBornX(0) {
}

GameElementLayer::~GameElementLayer() {
    reset();
}

void GameElementLayer::draw(QPainter &painter, Bird *bird) {
    // 遍历水管容器，如果可见则绘制，不可见则归还
    for (int i = 0; i < pipes.size(); i++) {
        Pipe *pipe = pipes[i];
        if (pipe->isVisible()) {
            pipe->draw(painter, bird);
        } else {
            Pipe *remove = pipes.takeAt(i);
            PipePool::getInstance()->giveBack(remove);
            i--;
        }
    }

    // 碰撞检测
    isCollideBird(bird);

    // 生成新水管
    pipeBornLogic(bird);
}

void GameElementLayer::pipeBornLogic(Bird *bird) {
    if (bird->isDead()) {
        return;
    }

    if (pipes.isEmpty()) {
        // 若容器为空，则添加一对水管
        int topHeight = QRandomGenerator::global()->bounded(
            Constant::MIN_PIPE_HEIGHT, Constant::MAX_PIPE_HEIGHT + 1);

        Pipe *top = PipePool::getInstance()->get("Pipe");
        top->setAttribute(Constant::FRAME_WIDTH + 100, -Constant::TOP_PIPE_LENGTHENING,
                         topHeight + Constant::TOP_PIPE_LENGTHENING,
                         Pipe::TYPE_TOP_NORMAL, true);

        Pipe *bottom = PipePool::getInstance()->get("Pipe");
        bottom->setAttribute(Constant::FRAME_WIDTH + 100, topHeight + Constant::VERTICAL_INTERVAL,
                            Constant::FRAME_HEIGHT - topHeight - Constant::VERTICAL_INTERVAL,
                            Pipe::TYPE_BOTTOM_NORMAL, true);

        pipes.append(top);
        pipes.append(bottom);
        lastBornX = Constant::FRAME_WIDTH + 100;
    } else {
        // 获取最后一根可见管道的X坐标
        int lastX = pipes.last()->getX();

        // 只有当最后一根管道的X小于阈值，并且与上次生成位置有足够距离时才生成
        if (lastX < Constant::FRAME_WIDTH / 2 && lastX > lastBornX - Constant::HORIZONTAL_INTERVAL) {
            // 生成新管道
            int currentScore = static_cast<int>(counter->getCurrentScore()) + 1;

            // Java: isInProbability(currentScore, 20)
            if (QRandomGenerator::global()->bounded(1, 21) <= currentScore) {
                // 移动水管
                if (QRandomGenerator::global()->bounded(1, 5) <= 1) {
                    addMovingHoverPipe();
                } else {
                    addMovingNormalPipe();
                }
            } else {
                // 普通水管
                if (QRandomGenerator::global()->bounded(1, 3) <= 1) {
                    addNormalPipe();
                } else {
                    addHoverPipe();
                }
            }

            lastBornX = pipes.last()->getX();
        }
    }
}

void GameElementLayer::addNormalPipe() {
    if (pipes.isEmpty()) return;
    int x = pipes.last()->getX() + Constant::HORIZONTAL_INTERVAL;

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

void GameElementLayer::addHoverPipe() {
    if (pipes.isEmpty()) return;
    int x = pipes.last()->getX() + Constant::HORIZONTAL_INTERVAL;

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
    int x = pipes.last()->getX() + Constant::HORIZONTAL_INTERVAL;

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
    int x = pipes.last()->getX() + Constant::HORIZONTAL_INTERVAL;

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
    lastBornX = 0;
}
