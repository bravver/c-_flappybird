#include "welcomanimation.h"
#include "constant.h"
#include "skinmanager.h"
#include <QPainter>

WelcomeAnimation::WelcomeAnimation(QObject *parent)
    : QObject(parent)
    , flashCount(0) {

    titleImage = QPixmap(Constant::TITLE_IMG_PATH);
    noticeImage = QPixmap(Constant::NOTICE_IMG_PATH);
}

WelcomeAnimation::~WelcomeAnimation() {}

void WelcomeAnimation::draw(QPainter &painter) {
    // 绘制标题
    int titleX = (Constant::FRAME_WIDTH - titleImage.width()) / 2;
    int titleY = Constant::FRAME_HEIGHT / 3;
    painter.drawPixmap(titleX, titleY, titleImage);

    // 获取皮肤列表和当前选中索引
    QVector<Skin> skins = SkinManager::getInstance()->getAllSkins();
    QString currentId = SkinManager::getInstance()->getCurrentSkinId();

    // 计算当前选中索引
    int selectedIndex = 0;
    for (int i = 0; i < skins.size(); i++) {
        if (skins[i].id == currentId) {
            selectedIndex = i;
            break;
        }
    }

    // 绘制两个皮肤选项（左右排列）
    int birdY = Constant::FRAME_HEIGHT / 2;
    int spacing = 120;

    for (int i = 0; i < skins.size(); i++) {
        Skin &skin = skins[i];
        int birdX = Constant::FRAME_WIDTH / 2 + (i - selectedIndex) * spacing;

        // 加载皮肤的第一帧作为预览
        QPixmap preview(skin.flyFrames.isEmpty() ? skin.upFrame : skin.flyFrames[0]);

        // 未选中皮肤变暗
        if (i != selectedIndex) {
            painter.setOpacity(0.3);
            painter.drawPixmap(birdX - preview.width() / 2, birdY - preview.height() / 2, preview);
            painter.setOpacity(1.0);
        } else {
            // 选中皮肤正常显示，带动画效果
            int frameIndex = (flashCount / 10) % (skin.flyFrames.isEmpty() ? 1 : skin.flyFrames.size());
            QPixmap animFrame(skin.flyFrames.isEmpty() ? skin.upFrame : skin.flyFrames[frameIndex]);
            painter.drawPixmap(birdX - animFrame.width() / 2, birdY - animFrame.height() / 2, animFrame);
        }
    }

    // 在选中皮肤旁边画箭头
    int selectedX = Constant::FRAME_WIDTH / 2;
    int arrowX = selectedX - spacing / 2 - 40;
    painter.drawText(arrowX, birdY + 5, "◀");
    arrowX = selectedX + spacing / 2 + 20;
    painter.drawText(arrowX, birdY + 5, "▶");

    // 绘制开始提示 - 带闪烁效果
    const int CYCLE = 30;
    if (flashCount++ > CYCLE) {
        int noticeX = (Constant::FRAME_WIDTH - noticeImage.width()) / 2;
        int noticeY = Constant::FRAME_HEIGHT / 5 * 3;
        painter.drawPixmap(noticeX, noticeY, noticeImage);
    }
    if (flashCount == CYCLE * 2) {
        flashCount = 0;
    }
}

void WelcomeAnimation::keyLeft() {
    QVector<Skin> skins = SkinManager::getInstance()->getAllSkins();
    QString currentId = SkinManager::getInstance()->getCurrentSkinId();
    int currentIndex = 0;
    for (int i = 0; i < skins.size(); i++) {
        if (skins[i].id == currentId) {
            currentIndex = i;
            break;
        }
    }
    if (currentIndex > 0) {
        SkinManager::getInstance()->setCurrentSkin(skins[currentIndex - 1].id);
    }
}

void WelcomeAnimation::keyRight() {
    QVector<Skin> skins = SkinManager::getInstance()->getAllSkins();
    QString currentId = SkinManager::getInstance()->getCurrentSkinId();
    int currentIndex = 0;
    for (int i = 0; i < skins.size(); i++) {
        if (skins[i].id == currentId) {
            currentIndex = i;
            break;
        }
    }
    if (currentIndex < skins.size() - 1) {
        SkinManager::getInstance()->setCurrentSkin(skins[currentIndex + 1].id);
    }
}
