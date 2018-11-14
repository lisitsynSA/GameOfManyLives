#ifndef LIFEFRAME_H
#define LIFEFRAME_H
#include "QObject"

class LifeFrame : public QObject
{
    Q_OBJECT
public:
    explicit LifeFrame(QObject *parent = nullptr);
    explicit LifeFrame(int initSizeX, int initSizeY, QObject *parent = nullptr);
    ~LifeFrame();
    int* resizeFrame(int initSizeX, int initSizeY);
    int getFrameSizeX();
    int getFrameSizeY();
    int getStepNumber();
    int* getCurFrame();
    int* getColonyStatistic();
    int getColonyNumber();
    int* random(int initColonyNumber);
    int* step();
    int cell(int x, int y);

private:
    int sizeX;
    int sizeY;
    int stepNumber;
    int colonyNumber;
    int* curFrame;
    int* nextFrame;
    int* colonyStatistic; // [0] - dead cells; [colonyNumber] - sick cells
    int* colonyTMP;
};

#endif // LIFEFRAME_H
