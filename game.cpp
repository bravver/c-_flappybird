#include "game.h"
#include <QPainter>
#include <QDebug>

int Game::gameState = 0;

Game::Game(QWidget *parent)
    : QWidget(parent), timer(new QTimer(this)) {

    initFrame();
    initGame();

    // 连接定时器到重绘
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Game::update));
    timer->start(Constant::FPS);
}

Game::~Game() {
    delete bird;
    delete background;
    delete foreground;
    delete gameElement;
    delete welcomeAnimation;
    delete gameOverAnimation;
}

void Game::initFrame() {
    setFixedSize(Constant::FRAME_WIDTH, Constant::FRAME_HEIGHT);
    setWindowTitle(Constant::GAME_TITLE);
    setGeometry(Constant::FRAME_X, Constant::FRAME_Y,
                Constant::FRAME_WIDTH, Constant::FRAME_HEIGHT);
    setFocusPolicy(Qt::StrongFocus);
}

void Game::initGame() {
    background = new GameBackground();
    foreground = new GameForeground();
    gameElement = new GameElementLayer();
    welcomeAnimation = new WelcomeAnimation();
    gameOverAnimation = new GameOverAnimation();
    bird = new Bird();

    setGameState(GAME_READY);
}

void Game::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 绘制背景层
    background->draw(painter, bird);

    // 绘制前景层
    foreground->draw(painter);

    if (gameState == GAME_READY) {
        // 游戏未开始，绘制欢迎动画
        welcomeAnimation->draw(painter);
    } else {
        // 游戏进行中，绘制游戏元素
        gameElement->draw(painter, bird);
    }

    // 绘制小鸟
    bird->draw(painter);

    // 结束状态绘制结束动画
    if (gameState == STATE_OVER) {
        gameOverAnimation->draw(painter, bird);
    }
}

void Game::keyPressEvent(QKeyEvent *event) {
    int keycode = event->key();

    if (keycode == Qt::Key_Space) {
        switch (gameState) {
        case GAME_READY:
            bird->birdFlap();
            bird->birdFall();
            setGameState(GAME_START);
            break;
        case GAME_START:
            bird->birdFlap();
            bird->birdFall();
            break;
        case STATE_OVER:
            resetGame();
            break;
        }
    }
}

void Game::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        keyFlag = true;
        bird->keyReleased();
    }
}

void Game::resetGame() {
    setGameState(GAME_READY);
    gameElement->reset();
    bird->reset();
}

void Game::setGameState(int state) {
    Game::gameState = state;
}