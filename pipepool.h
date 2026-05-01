#ifndef PIPEPOOL_H
#define PIPEPOOL_H

#include <QObject>
#include <QVector>
#include "pipe.h"

class PipePool : public QObject {
    Q_OBJECT

public:
    static PipePool* getInstance();
    static void destroy();

    Pipe* get(const QString &type);
    void giveBack(Pipe *pipe);

    static const int FULL_PIPE = 10;

private:
    PipePool(QObject *parent = nullptr);
    ~PipePool();

    static PipePool *instance;
    QVector<Pipe*> normalPipes;
    QVector<Pipe*> movingPipes;
};

#endif // PIPEPOOL_H