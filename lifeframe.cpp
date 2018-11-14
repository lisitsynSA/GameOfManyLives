#include "lifeframe.h"
#include "qmath.h"

LifeFrame::LifeFrame(QObject *parent) :
    QObject(parent)
{
    curFrame = nullptr;
    nextFrame = nullptr;
    colonyStatistic = nullptr;
    colonyTMP = nullptr;
    sizeX = 0;
    sizeY = 0;
    stepNumber = 0;
}

LifeFrame::LifeFrame(int initSizeX, int initSizeY, QObject *parent) :
    QObject(parent)
{
    curFrame = nullptr;
    nextFrame = nullptr;
    sizeX = initSizeX;
    sizeY = initSizeY;
    stepNumber = 0;
    resizeFrame(sizeX, sizeY);
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

int* LifeFrame::resizeFrame(int initSizeX, int initSizeY)
{
    sizeX = initSizeX;
    sizeY = initSizeY;
    stepNumber = 0;

    if (curFrame != nullptr)
        delete curFrame;
    curFrame = new int[sizeX*sizeY];

    if (nextFrame != nullptr)
        delete nextFrame;
    nextFrame = new int[sizeX*sizeY];

    for (int ny = 0; ny < sizeY; ny++)
    for (int nx = 0; nx < sizeX; nx++)
        curFrame[ny*sizeX + nx] = (ny%3)*(nx%3);
    return curFrame;
}

int LifeFrame::getFrameSizeX()
{
    return (curFrame != nullptr) ? sizeX : 0;
}

int LifeFrame::getFrameSizeY()
{
    return (curFrame != nullptr) ? sizeY : 0;
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

int LifeFrame::getColonyNumber()
{
    return colonyNumber;
}

int* LifeFrame::random(int initColonyNumber)
{
    colonyNumber = initColonyNumber;
    if (colonyTMP != nullptr)
        delete colonyTMP;
    colonyTMP = new int[colonyNumber + 2];
    if (colonyStatistic != nullptr)
        delete colonyStatistic;
    colonyStatistic = new int[colonyNumber + 2];
    for (int colony = 0; colony < colonyNumber + 2; colony++)
        colonyStatistic[colony] = 0;

    stepNumber = 0;
    for (int y = 0; y < sizeY; y++)
    for (int x = 0; x < sizeX; x++)
    {
        curFrame[y*sizeX + x] = qrand()%2 == 1 ? ((qrand()%colonyNumber) + 1) : 0;
        colonyStatistic[curFrame[y*sizeX + x]]++;
    }
    return curFrame;
}

int* LifeFrame::step()
{
    int sum = 0;
    for (int colony = 0; colony < colonyNumber + 2; colony++)
    {
        colonyTMP[colony] = 0;
        colonyStatistic[colony] = 0;
    }

    for (int y = 0; y < sizeY; y++)
    for (int x = 0; x < sizeX; x++)
    {
        sum = cell(y-1, x-1) + cell(y-1, x) + cell(y-1, x+1) +\
                cell(y, x-1) + cell(y, x+1) +\
                cell(y+1, x-1) + cell(y+1, x) + cell(y+1, x+1);
        nextFrame[y*sizeX + x] = 0;
        // safe cell
        if (curFrame[y*sizeX + x] > 0 && (sum == 2 || sum == 3))
            nextFrame[y*sizeX + x] = curFrame[y*sizeX + x];
        // born cell
        if (curFrame[y*sizeX + x] == 0 && sum == 3)
            for (int colony = 1; colony <= colonyNumber; colony++)
                if (colonyTMP[colony] > 1)
                    nextFrame[y*sizeX + x] = colony;
        // check sick
        if (nextFrame[y*sizeX + x] > 0 && colonyTMP[colonyNumber + 1] > 0)
            nextFrame[y*sizeX + x] = colonyNumber + 1;
        colonyTMP[colonyNumber + 1] = 0;
        //get colony statistic
        colonyStatistic[nextFrame[y*sizeX + x]]++;
        for (int colony = 1; colony <= colonyNumber; colony++)
        {
            colonyTMP[colony] = 0;
        }
    }

    stepNumber++;
    std::swap(nextFrame, curFrame);
    return curFrame;
}

int LifeFrame::cell(int y, int x)
{
    if (y < 0)
        y = sizeY - 1;
    if (y > sizeY - 1)
        y = 0;
    if (x < 0)
        x = sizeX - 1;
    if (x > sizeX - 1)
        x = 0;
    if (curFrame[y*sizeX + x] > 0)
    {
        colonyTMP[curFrame[y*sizeX + x]]++;
        return 1;
    }
    return 0;
}

