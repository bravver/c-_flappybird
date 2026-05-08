#ifndef BIRD_H
#define BIRD_H

#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QVector>
#include <QSoundEffect>
#include "constant.h"
#include "scorecounter.h"
#include "skinmanager.h"

class GameBackground;

class Bird : public QObject {
    Q_OBJECT

public:
    Bird(QObject *parent = nullptr);
    ~Bird();

    // 小鸟状态
    enum State {
        BIRD_NORMAL = 0,
        BIRD_UP = 1,
        BIRD_FALL = 2,
        BIRD_DEAD_FALL = 3,
        BIRD_DEAD = 4
    };

    void draw(QPainter &painter);
    void birdFlap();
    void birdFall();
    void deadBirdFall();
    void reset();

    bool isDead() const;
    void keyReleased();
    void keyPressed();
    bool keyIsReleased() const;

    int getBirdX() const;
    int getBirdY() const;
    QRect getCollisionRect() const;

    static int BIRD_WIDTH;
    static int BIRD_HEIGHT;

private:
    void freeFall();
    void die();
    void drawScore(QPainter &painter);
    void movement();
    void loadSkinImages(const QString &skinId);

    // 蓝色皮肤图片组
    QVector<QPixmap> blueNormalImages;
    QVector<QPixmap> blueUpImages;
    QVector<QPixmap> blueFallImages;
    QPixmap blueDeadImage;

    // 黄色皮肤图片组
    QVector<QPixmap> yellowNormalImages;
    QVector<QPixmap> yellowUpImages;
    QVector<QPixmap> yellowFallImages;
    QPixmap yellowDeadImage;

    int x;
    int y;
    int wingState;
    int state;

    int velocity;
    static constexpr int ACC_FLAP = 14;
    static constexpr int ACC_Y = 2;
    static constexpr int MAX_VEL_Y = 15;
    static int BOTTOM_BOUNDARY;

    QRect collisionRect;
    static constexpr int RECT_DESCALE = 2;

    ScoreCounter *counter;
    bool keyFlag;

    QSoundEffect *flySound;
    QSoundEffect *crashSound;

signals:
    void birdDied();
};

#endif // BIRD_H
