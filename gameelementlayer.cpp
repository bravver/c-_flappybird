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
            Pipe *remove = pipes.takeAt(i);
            PipePool::getInstance()->giveBack(remove);
            i--;
        }
    }

    // 碰撞检测
    isCollideBird(bird);

    // 生成新水管（包含得分检测）
    pipeBornLogic(bird);
}

// 生成新水管 - 完全按照Java逻辑
void GameElementLayer::pipeBornLogic(Bird *bird) {
    if (bird->isDead()) {
        return;
    }

    if (pipes.isEmpty()) {
        // 若容器为空，则添加一对水管
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
        // 判断最后一对水管是否完全进入游戏窗口
        Pipe *lastPipe = pipes.last();
        int currentDistance = lastPipe->getX() - bird->getBirdX() + Bird::BIRD_WIDTH / 2;
        const int SCORE_DISTANCE = Pipe::PIPE_WIDTH * 2 + Constant::HORIZONTAL_INTERVAL;

        if (lastPipe->isInFrame()) {
            // Java: pipes.size() >= PipePool.FULL_PIPE - 2 (即 >= 8)
            // 但由于我们立即移除不可见的管道，pipes.size() 通常 <= 2
            // 所以我们改成：当 currentDistance <= SCORE_DISTANCE + PIPE_WIDTH * 3/2 时得分
            if (currentDistance <= SCORE_DISTANCE + Pipe::PIPE_WIDTH * 3 / 2) {
                counter->score();
            }

            // 生成新管道
            int currentScore = static_cast<int>(counter->getCurrentScore()) + 1;

            // Java: isInProbability(currentScore, 20) = random(1, 21) <= currentScore
            bool generateHardPipe = (QRandomGenerator::global()->bounded(1, 21) <= currentScore);

            if (generateHardPipe) {
                if (QRandomGenerator::global()->bounded(1, 5) <= 1) { // isInProbability(1, 4)
                    addMovingHoverPipe();
                } else {
                    addMovingNormalPipe();
                }
            } else {
                if (QRandomGenerator::global()->bounded(1, 3) <= 1) { // isInProbability(1, 2)
                    addNormalPipe();
                } else {
                    addHoverPipe();
                }
            }
        }
    }
}

void GameElementLayer::addNormalPipe() {
    if (pipes.isEmpty()) return;
    Pipe *lastPipe = pipes.last();

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

void GameElementLayer::addHoverPipe() {
    if (pipes.isEmpty()) return;
    Pipe *lastPipe = pipes.last();

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

void GameElementLayer::addMovingNormalPipe() {
    if (pipes.isEmpty()) return;
    Pipe *lastPipe = pipes.last();

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

void GameElementLayer::addMovingHoverPipe() {
    if (pipes.isEmpty()) return;
    Pipe *lastPipe = pipes.last();

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
