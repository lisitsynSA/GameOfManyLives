#ifndef LIFEFRAME_H
#define LIFEFRAME_H
#include "QObject"

class LifeFrame : public QObject
{
    Q_OBJECT
public:
    explicit LifeFrame(QObject *parent = nullptr);
    explicit LifeFrame(int initSize, QObject *parent = nullptr);
    ~LifeFrame();
    int* resizeFrame(int size);
    int getFrameSize();
    int getStepNumber();
    int* getCurFrame();
    int* random();
    int* step();
    int cell(int x, int y);

private:
    int size;
    int stepNumber;
    int* curFrame;
    int* nextFrame;
};

#endif // LIFEFRAME_H
