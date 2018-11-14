#include "lifeframe.h"
#include "qmath.h"

LifeFrame::LifeFrame(QObject *parent) :
    QObject(parent)
{
    curFrame = nullptr;
    nextFrame = nullptr;
    colonyStatistic = nullptr;
    colonyTMP = nullptr;
    size = 0;
    stepNumber = 0;
}

LifeFrame::LifeFrame(int initSize, QObject *parent) :
    QObject(parent)
{
    curFrame = nullptr;
    nextFrame = nullptr;
    size = initSize;
    stepNumber = 0;
    resizeFrame(initSize);
}

LifeFrame::~LifeFrame()
{
    if (curFrame != nullptr)
        delete curFrame;
    if (nextFrame != nullptr)
        delete nextFrame;
    if (colonyStatistic != nullptr)
        delete colonyStatistic;
    if (colonyTMP != nullptr)
        delete colonyTMP;
}

int* LifeFrame::resizeFrame(int initSize)
{
    size = initSize;
    stepNumber = 0;

    if (curFrame != nullptr)
        delete curFrame;
    curFrame = new int[size*size];

    if (nextFrame != nullptr)
        delete nextFrame;
    nextFrame = new int[size*size];

    for (int ny = 0; ny < size; ny++)
    for (int nx = 0; nx < size; nx++)
        curFrame[ny*size + nx] = (ny%3)*(nx%3);
    return curFrame;
}

int LifeFrame::getFrameSize()
{
    return (curFrame != nullptr) ? size : 0;
}

int LifeFrame::getStepNumber()
{
    return (curFrame != nullptr) ? stepNumber : 0;
}

int* LifeFrame::getCurFrame()
{
    return curFrame;
}

int* LifeFrame::getColonyStatistic()
{
    return colonyStatistic;
}

int* LifeFrame::random(int initColonyNumber)
{
    colonyNumber = initColonyNumber;
    if (colonyStatistic != nullptr)
        delete colonyStatistic;
    colonyStatistic = new int[colonyNumber];
    if (colonyTMP != nullptr)
        delete colonyTMP;
    colonyTMP = new int[colonyNumber];
    stepNumber = 0;
    for (int ny = 0; ny < size; ny++)
    for (int nx = 0; nx < size; nx++)
        curFrame[ny*size + nx] = qrand()%2 == 1 ? (qrand()%colonyNumber + 1) : 0;
    return curFrame;
}

int* LifeFrame::step()
{
    int sum = 0;
    for (int colony = 0; colony < colonyNumber; colony++)
    {
        colonyTMP[colony] = 0;
        colonyStatistic[colony] = 0;
    }

    for (int y = 0; y < size; y++)
    for (int x = 0; x < size; x++)
    {
        sum = cell(y-1, x-1) + cell(y-1, x) + cell(y-1, x+1) +\
                cell(y, x-1) + cell(y, x+1) +\
                cell(y+1, x-1) + cell(y+1, x) + cell(y+1, x+1);
        nextFrame[y*size + x] = 0;
        if (curFrame[y*size + x] > 0 && (sum == 2 || sum == 3))
            nextFrame[y*size + x] = curFrame[y*size + x];
        if (curFrame[y*size + x] == 0 && sum == 3)
            for (int colony = 1; colony < colonyNumber; colony++)
            {
                if (colonyTMP[colony] > 1)
                    nextFrame[y*size + x] = colony;
                colonyTMP[colony] = 0;
            }
        colonyTMP[nextFrame[y*size + x]]++;
    }

    stepNumber++;
    std::swap(nextFrame, curFrame);
    return curFrame;
}

int LifeFrame::cell(int y, int x)
{
    if (y < 0)
        y = size - 1;
    if (y > size - 1)
        y = 0;
    if (x < 0)
        x = size - 1;
    if (x > size - 1)
        x = 0;
    if (curFrame[y*size + x] > 0)
    {
        colonyTMP[curFrame[y*size + x]]++;
        return 1;
    }
    return 0;
}

