#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include "constant.h"
#include "bird.h"
#include "gamebackground.h"
#include "gameforeground.h"
#include "gameelementlayer.h"
#include "welcomanimation.h"
#include "gameoveranimation.h"

class Game : public QWidget {
    Q_OBJECT

public:
    Game(QWidget *parent = nullptr);
    ~Game();

    enum GameState {
        GAME_READY = 0,
        GAME_START = 1,
        STATE_OVER = 2
    };

    static void setGameState(int state);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void initFrame();
    void initGame();
    void resetGame();

    static int gameState;

    QTimer *timer;
    Bird *bird;
    GameBackground *background;
    GameForeground *foreground;
    GameElementLayer *gameElement;
    WelcomeAnimation *welcomeAnimation;
    GameOverAnimation *gameOverAnimation;

    bool keyFlag = true;  // 按键状态，true为已释放
};

#endif // GAME_H