#include "skinmanager.h"

SkinManager* SkinManager::instance = nullptr;

SkinManager::SkinManager() : currentSkinId("blue") {
    // 蓝色小鸟（默认选中）
    Skin blue;
    blue.id = "blue";
    blue.name = "蓝色小鸟";
    for (int i = 0; i < 8; i++) {
        blue.flyFrames.append(QString(":/resources/img/%1.png").arg(i));
    }
    blue.upFrame = ":/resources/img/up.png";
    for (int i = 0; i < 8; i++) {
        blue.downFrames.append(QString(":/resources/img/down_%1.png").arg(i));
    }
    blue.deadFrame = ":/resources/img/dead.png";
    skins.append(blue);

    // 黄色小鸟（3帧简化版）
    Skin yellow;
    yellow.id = "yellow";
    yellow.name = "经典小鸟";
    yellow.flyFrames = {
        ":/resources/skin/yellow/yellowbird-midflap.png",
        ":/resources/skin/yellow/yellowbird-upflap.png",
        ":/resources/skin/yellow/yellowbird-midflap.png",
        ":/resources/skin/yellow/yellowbird-downflap.png",
        ":/resources/skin/yellow/yellowbird-midflap.png",
        ":/resources/skin/yellow/yellowbird-upflap.png",
        ":/resources/skin/yellow/yellowbird-midflap.png",
        ":/resources/skin/yellow/yellowbird-downflap.png"
    };
    yellow.upFrame = ":/resources/skin/yellow/yellowbird-upflap.png";
    for (int i = 0; i < 8; i++) {
        yellow.downFrames.append(yellow.flyFrames[i % yellow.flyFrames.size()]);
    }
    yellow.deadFrame = ":/resources/img/dead.png";
    skins.append(yellow);
}

SkinManager* SkinManager::getInstance() {
    if (!instance) {
        instance = new SkinManager();
    }
    return instance;
}

QVector<Skin> SkinManager::getAllSkins() const {
    return skins;
}

Skin SkinManager::getCurrentSkin() const {
    for (const Skin &skin : skins) {
        if (skin.id == currentSkinId) {
            return skin;
        }
    }
    return skins.first();
}

void SkinManager::setCurrentSkin(const QString &skinId) {
    currentSkinId = skinId;
}

QString SkinManager::getCurrentSkinId() const {
    return currentSkinId;
}

bool SkinManager::isSkinUnlocked(const QString &skinId) const {
    Q_UNUSED(skinId);
    return true;  // 默认全部解锁，后续可加解锁逻辑
}

void SkinManager::unlockSkin(const QString &skinId) {
    Q_UNUSED(skinId);
}
