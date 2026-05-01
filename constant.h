#ifndef CONSTANT_H
#define CONSTANT_H

#include <QString>
#include <QRgb>

class Constant {
public:
    // 窗口尺寸
    static const int FRAME_WIDTH = 420;
    static const int FRAME_HEIGHT = 640;

    // 游戏标题
    static const QString GAME_TITLE;

    // 窗口初始位置
    static const int FRAME_X = 600;
    static const int FRAME_Y = 100;

    // 图片资源路径
    static const QString BG_IMG_PATH;
    static const QStringList BIRDS_IMG_PATH;
    static const QStringList CLOUDS_IMG_PATH;
    static const QStringList PIPE_IMG_PATH;
    static const QString TITLE_IMG_PATH;
    static const QString NOTICE_IMG_PATH;
    static const QString SCORE_IMG_PATH;
    static const QString OVER_IMG_PATH;
    static const QString AGAIN_IMG_PATH;

    // 游戏速度
    static const int GAME_SPEED = 4;

    // 游戏背景色
    static const QRgb BG_COLOR = 0x4bc4cf;

    // 游戏刷新率 ms
    static const int FPS = 1000 / 30;

    // 标题栏高度
    static const int TOP_BAR_HEIGHT = 20;

    // 地面高度
    static const int GROUND_HEIGHT = 35;

    // 上方管道加长
    static const int TOP_PIPE_LENGTHENING = 100;

    // 云朵生成概率
    static const int CLOUD_BORN_PERCENT = 6;
    static const int CLOUD_IMAGE_COUNT = 2;
    static const int MAX_CLOUD_COUNT = 7;

    // 水管间隔
    static const int VERTICAL_INTERVAL = FRAME_HEIGHT / 5;
    static const int HORIZONTAL_INTERVAL = FRAME_HEIGHT >> 2;
    static const int MIN_PIPE_HEIGHT = FRAME_HEIGHT >> 3;
    static const int MAX_PIPE_HEIGHT = (FRAME_HEIGHT >> 3) * 5;
};

#endif // CONSTANT_H