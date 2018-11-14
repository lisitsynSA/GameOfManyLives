#include "lifeframe.h"
#include "qmath.h"

LifeFrame::LifeFrame(QObject *parent) :
    QObject(parent)
{
    curFrame = nullptr;
    nextFrame = nullptr;
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

int* LifeFrame::random()
{
    stepNumber = 0;
    for (int ny = 0; ny < size; ny++)
    for (int nx = 0; nx < size; nx++)
        curFrame[ny*size + nx] = qrand() % 2;
    return curFrame;
}

int* LifeFrame::step()
{
    int sum = 0;
    for (int y = 0; y < size; y++)
    for (int x = 0; x < size; x++)
    {
        sum = cell(y-1, x-1) + cell(y-1, x) + cell(y-1, x+1) +\
                cell(y, x-1) + cell(y, x+1) +\
                cell(y+1, x-1) + cell(y+1, x) + cell(y+1, x+1);
        nextFrame[y*size + x] = 0;
        if ((sum == 3) || (sum == 2 && curFrame[y*size + x] == 1))
            nextFrame[y*size + x] = 1;
    }

    stepNumber++;
    std::swap(nextFrame, curFrame);
    return curFrame;
}

int LifeFrame::cell(int x, int y)
{
    if (x < 0)
        x = size - 1;
    if (y < 0)
        y = size - 1;
    if (x > size - 1)
        x = 0;
    if (y > size - 1)
        y = 0;
    return (curFrame != nullptr) ? curFrame[y*size + x] : 0;
}

