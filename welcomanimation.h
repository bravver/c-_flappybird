#ifndef WELCOMEANIMATION_H
#define WELCOMEANIMATION_H

#include <QObject>
#include <QPainter>
#include <QPixmap>

class WelcomeAnimation : public QObject {
    Q_OBJECT

public:
    WelcomeAnimation(QObject *parent = nullptr);
    ~WelcomeAnimation();

    void draw(QPainter &painter);

private:
    QPixmap titleImage;
    QPixmap noticeImage;
    int flashCount;
};

#endif // WELCOMEANIMATION_H
