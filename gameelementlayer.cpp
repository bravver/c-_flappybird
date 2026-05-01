#include "gameelementlayer.h"
#include "bird.h"
#include "scorecounter.h"
#include <QRandomGenerator>
#include <QDebug>

GameElementLayer::GameElementLayer(QObject *parent)
    : QObject(parent)
    , counter(ScoreCounter::getInstance()) {
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

    // 生成新水管
    pipeBornLogic(bird);
}

void GameElementLayer::pipeBornLogic(Bird *bird) {
    if (bird->isDead()) {
        return;
    }

    if (pipes.isEmpty()) {
        int topHeight = QRandomGenerator::global()->bounded(
            Constant::MIN_PIPE_HEIGHT, Constant::MAX_PIPE_HEIGHT + 1);

        Pipe *top = PipePool::getInstance()->get("Pipe");
        top->setAttribute(Constant::FRAME_WIDTH, -Constant::TOP_PIPE_LENGTHENING,
                         topHeight + Constant::TOP_PIPE_LENGTHENING,
                         Pipe::TYPE_TOP_NORMAL, true);

        Pipe *bottom = PipePool::getInstance()->get("Pipe");
        bottom->setAttribute(Constant::FRAME_WIDTH, topHeight + Constant::VERTICAL_INTERVAL,
                            Constant::FRAME_HEIGHT - topHeight - Constant::VERTICAL_INTERVAL,
                            Pipe::TYPE_BOTTOM_NORMAL, true);

        pipes.append(top);
        pipes.append(bottom);
    } else {
        Pipe *lastPipe = pipes.last();
        int currentDistance = lastPipe->getX() - bird->getBirdX() + Bird::BIRD_WIDTH / 2;
        const int SCORE_DISTANCE = Pipe::PIPE_WIDTH * 2 + Constant::HORIZONTAL_INTERVAL;

        if (lastPipe->isInFrame()) {
            if (pipes.size() >= PipePool::FULL_PIPE - 2 &&
                currentDistance <= SCORE_DISTANCE + Pipe::PIPE_WIDTH * 3 / 2) {
                counter->score();
            }

            int currentScore = static_cast<int>(counter->getCurrentScore()) + 1;

            if (currentScore > 19 || QRandomGenerator::global()->bounded(20) < currentScore) {
                if (QRandomGenerator::global()->bounded(4) == 0) {
                    addMovingHoverPipe(lastPipe);
                } else {
                    addMovingNormalPipe(lastPipe);
                }
            } else {
                if (QRandomGenerator::global()->bounded(2) == 0) {
                    addNormalPipe(lastPipe);
                } else {
                    addHoverPipe(lastPipe);
                }
            }
        }
    }
}

void GameElementLayer::addNormalPipe(Pipe *lastPipe) {
    int topHeight = QRandomGenerator::global()->bounded(
        Constant::MIN_PIPE_HEIGHT, Constant::MAX_PIPE_HEIGHT + 1);
    int x = lastPipe->getX() + Constant::HORIZONTAL_INTERVAL;

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

void GameElementLayer::addHoverPipe(Pipe *lastPipe) {
    int topHoverHeight = QRandomGenerator::global()->bounded(
        Constant::FRAME_HEIGHT / 6, Constant::FRAME_HEIGHT / 4);
    int x = lastPipe->getX() + Constant::HORIZONTAL_INTERVAL;
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

void GameElementLayer::addMovingNormalPipe(Pipe *lastPipe) {
    int topHeight = QRandomGenerator::global()->bounded(
        Constant::MIN_PIPE_HEIGHT, Constant::MAX_PIPE_HEIGHT + 1);
    int x = lastPipe->getX() + Constant::HORIZONTAL_INTERVAL;

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

void GameElementLayer::addMovingHoverPipe(Pipe *lastPipe) {
    int topHoverHeight = QRandomGenerator::global()->bounded(
        Constant::FRAME_HEIGHT / 6, Constant::FRAME_HEIGHT / 4);
    int x = lastPipe->getX() + Constant::HORIZONTAL_INTERVAL;
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
}