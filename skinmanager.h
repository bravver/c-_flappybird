#ifndef SKINMANAGER_H
#define SKINMANAGER_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QPixmap>

struct Skin {
    QString id;
    QString name;
    QStringList flyFrames;   // 飞行帧
    QString upFrame;        // 上升帧
    QStringList downFrames; // 下落帧
    QString deadFrame;      // 死亡帧
};

class SkinManager {
public:
    static SkinManager* getInstance();

    QVector<Skin> getAllSkins() const;
    Skin getCurrentSkin() const;
    void setCurrentSkin(const QString &skinId);
    QString getCurrentSkinId() const;
    bool isSkinUnlocked(const QString &skinId) const;
    void unlockSkin(const QString &skinId);

private:
    SkinManager();
    static SkinManager *instance;

    QVector<Skin> skins;
    QString currentSkinId;
};

#endif // SKINMANAGER_H
