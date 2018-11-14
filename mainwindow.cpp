#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    lifeframe = new LifeFrame(100, this);

    plot = new QCustomPlot(this);
    colorMap = new QCPColorMap(plot->xAxis, plot->yAxis);
    plot->addPlottable(colorMap);

    colorMap->setGradient(QCPColorGradient::gpThermal);
    colorScale = new QCPColorScale(plot);
    colorScale->setGradient(QCPColorGradient::gpThermal);
    colorScale->setDataRange(QCPRange(0, 1));
    plot->plotLayout()->addElement(0, 1, colorScale);

    plot->rescaleAxes();
    plot->replot();
    plot->setMinimumSize(500, 500);
    plot->setMaximumSize(500, 500);
    ui->plotLayout->addWidget(plot);

    resizeArray();

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(Calc()));
    QObject::connect(ui->startButton, SIGNAL(pressed()), this, SLOT(startCalc()));
    QObject::connect(ui->stopButton, SIGNAL(pressed()), this, SLOT(stopCalc()));
    QObject::connect(ui->generateButton, SIGNAL(pressed()), this, SLOT(generateBtn()));
    QObject::connect(ui->sickButton, SIGNAL(pressed()), this, SLOT(sickBtn()));
    QObject::connect(ui->stepButton, SIGNAL(pressed()), this, SLOT(stepBtn()));
    QObject::connect(ui->XpointBox, SIGNAL(valueChanged(int)), this, SLOT(resizeArray()));
    QObject::connect(ui->YpointBox, SIGNAL(valueChanged(int)), this, SLOT(resizeArray()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::drawPlot()
{
    int sizeX = lifeframe->getFrameSize();
    int sizeY = lifeframe->getFrameSize();
    int* curFrame = lifeframe->getCurFrame();
    plot->xAxis->setLabel("Axis X");
    plot->yAxis->setLabel("Axis Y");
    colorMap->data()->setSize(sizeX, sizeY);
    colorMap->data()->setRange(QCPRange(0, sizeX), QCPRange(0, sizeY));

    for (int nx = 0; nx < sizeX; nx++)
    for (int ny = 0; ny < sizeY; ny++)
    {
        colorMap->data()->setCell(nx, ny, curFrame[nx*sizeY + ny]);
    }

    colorScale->setDataRange(QCPRange(0, ui->colonyBox->value()));
    colorMap->rescaleDataRange(true);
    plot->rescaleAxes();
    plot->replot();

}

void MainWindow::resizeArray()
{
    qDebug() << "Resize";
    lifeframe->resizeFrame(ui->XpointBox->value());
    lifeframe->random(ui->colonyBox->value());
    drawPlot();
}

void MainWindow::startCalc()
{
    qDebug() << "Start";
    drawPlot();
    ui->iterationBox->setValue(lifeframe->getStepNumber());
    timer.start(1);
}

void MainWindow::stopCalc()
{
    qDebug() << "Stop";
    timer.stop();
}

void MainWindow::Calc()
{
    lifeframe->step();
    drawPlot();
    ui->iterationBox->setValue(lifeframe->getStepNumber());
    showStatistic();
}

void MainWindow::generateBtn()
{
    qDebug() << "Generate";
    lifeframe->random(ui->colonyBox->value());
    drawPlot();
    showStatistic();
}

void MainWindow::sickBtn()
{
    qDebug() << "Sick";
}

void MainWindow::stepBtn()
{
    qDebug() << "Step";
    Calc();
}

void MainWindow::showStatistic()
{
    printf("[%d] ", lifeframe->getStepNumber());
    for (int colony = 0; colony < lifeframe->getColonyNumber() + 2; colony++)
    {
        printf("%d ", lifeframe->getColonyStatistic()[colony]);
    }
    printf("\n");
    fflush( stdout );
}

