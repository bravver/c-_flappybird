QT       += core gui widgets multimedia

greaterThan(QT_MAJOR_VERSION, 6): QT += widgets

CONFIG += c++17

TARGET = flappybird-qt
TEMPLATE = app

SOURCES += \
    main.cpp \
    game.cpp \
    bird.cpp \
    pipe.cpp \
    movingpipe.cpp \
    pipepool.cpp \
    gamebackground.cpp \
    gameforeground.cpp \
    gameelementlayer.cpp \
    welcomanimation.cpp \
    gameoveranimation.cpp \
    scorecounter.cpp \
    constant.cpp \
    skinmanager.cpp

HEADERS += \
    constant.h \
    game.h \
    bird.h \
    pipe.h \
    movingpipe.h \
    pipepool.h \
    gamebackground.h \
    gameforeground.h \
    gameelementlayer.h \
    welcomanimation.h \
    gameoveranimation.h \
    scorecounter.h \
    skinmanager.h

RESOURCES += resources.qrc

INCLUDEPATH += D:/FlappyBird/resources