#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    thermalArray = nullptr;
    nextThermalArray = nullptr;
    resizeArray();

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(Calc()));
    QObject::connect(ui->startButton, SIGNAL(pressed()), this, SLOT(startCalc()));
    QObject::connect(ui->stopButton, SIGNAL(pressed()), this, SLOT(stopCalc()));
    QObject::connect(ui->XpointBox, SIGNAL(valueChanged(int)), this, SLOT(resizeArray()));
    QObject::connect(ui->YpointBox, SIGNAL(valueChanged(int)), this, SLOT(resizeArray()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete thermalArray;
    delete nextThermalArray;
}

void MainWindow::drawPlot()
{
    double minVal = 4;
    double maxVal = 0;
    plot->xAxis->setLabel("Axis X");
    plot->yAxis->setLabel("Axis Y");
    colorMap->data()->setSize(sizeX, sizeY);
    colorMap->data()->setRange(QCPRange(0, sizeX), QCPRange(0, sizeY));

    for (int nx = 0; nx < sizeX; nx++)
    for (int ny = 0; ny < sizeY; ny++)
    {
        colorMap->data()->setCell(nx, ny, thermalArray[nx*sizeY + ny]);
        minVal = std::min(minVal, thermalArray[nx*sizeY + ny]);
        maxVal = std::max(maxVal, thermalArray[nx*sizeY + ny]);
    }

    colorScale->setDataRange(QCPRange(minVal, maxVal));
    colorMap->rescaleDataRange(true);
    plot->rescaleAxes();
    plot->replot();

}

void MainWindow::resizeArray()
{
    qDebug() << "Resize";
    sizeX = ui->XpointBox->value();
    sizeY = ui->YpointBox->value();

    if (thermalArray != nullptr)
        delete thermalArray;
    thermalArray = new double[sizeX*sizeY];

    if (nextThermalArray != nullptr)
        delete nextThermalArray;
    nextThermalArray = new double[sizeX*sizeY];

    for (int nx = 0; nx < sizeX; nx++)
    for (int ny = 0; ny < sizeY; ny++)
        thermalArray[nx*sizeY + ny] = sin(nx/M_PI)*cos(ny/M_PI);
    drawPlot();
}

void MainWindow::startCalc()
{
    qDebug() << "Start";

    if (ui->methodBox->currentIndex() == 2 && sizeX != sizeY) // Верхняя релаксация
        ui->YpointBox->setValue(sizeX);

    for (int nx = 0; nx < sizeX; nx++)
    for (int ny = 0; ny < sizeY; ny++)
        if (nx == 0) {
            thermalArray[nx*sizeY + ny] = 1;
            nextThermalArray[nx*sizeY + ny] = 1;
        } else if (ny == 0) {
            thermalArray[nx*sizeY + ny] = 4;
            nextThermalArray[nx*sizeY + ny] = 4;
        } else if (nx == sizeX - 1) {
            thermalArray[nx*sizeY + ny] = 3;
            nextThermalArray[nx*sizeY + ny] = 3;
        } else if (ny == sizeY - 1) {
            thermalArray[nx*sizeY + ny] = 2;
            nextThermalArray[nx*sizeY + ny] = 2;
        } else
            thermalArray[nx*sizeY + ny] = 2.5;
    drawPlot();

    ui->iterationBox->setValue(0);

    timer.start(1);
}

void MainWindow::stopCalc()
{
    qDebug() << "Stop";
    timer.stop();

}

void MainWindow::Calc()
{
    double hx2 = pow(0.1/sizeX, 2);
    double hy2 = pow(0.1/sizeY, 2);
    double tau = ui->tauBox->value();
    if (ui->methodBox->currentIndex() == 0) // Якоби
        for (int nx = 1; nx < sizeX - 1; nx++)
        for (int ny = 1; ny < sizeY - 1; ny++)
        {
            nextThermalArray[nx*sizeY + ny] = (hy2*(thermalArray[(nx-1)*sizeY + ny]+\
               thermalArray[(nx+1)*sizeY + ny]) + hx2*(thermalArray[nx*sizeY + ny - 1] +\
               thermalArray[nx*sizeY + ny + 1]))/(2*(hx2 + hy2));
        }

    if (ui->methodBox->currentIndex() == 1) // Зейделя
        for (int nx = 1; nx < sizeX - 1; nx++)
        for (int ny = 1; ny < sizeY - 1; ny++)
        {
            nextThermalArray[nx*sizeY + ny] = (hy2*(nextThermalArray[(nx-1)*sizeY + ny]+\
               thermalArray[(nx+1)*sizeY + ny]) + hx2*(nextThermalArray[nx*sizeY + ny - 1] +\
               thermalArray[nx*sizeY + ny + 1]))/(2*(hx2 + hy2));
        }

    if (ui->methodBox->currentIndex() == 2) // Верхняя релаксация
        for (int nx = 1; nx < sizeX - 1; nx++)
        for (int ny = 1; ny < sizeY - 1; ny++)
        {
            nextThermalArray[nx*sizeY + ny] = tau*(nextThermalArray[(nx-1)*sizeY + ny]+\
               thermalArray[(nx+1)*sizeY + ny] + nextThermalArray[nx*sizeY + ny - 1] +\
               thermalArray[nx*sizeY + ny + 1])/4 + (1 - tau)*thermalArray[nx*sizeY + ny];
        }

    if (ui->methodBox->currentIndex() == 3) // Test
        for (int nx = 1; nx < sizeX - 1; nx++)
        for (int ny = 1; ny < sizeY - 1; ny++)
        {
            nextThermalArray[nx*sizeY + ny] = (hy2*(nextThermalArray[(nx-1)*sizeY + ny]+\
               thermalArray[(nx+1)*sizeY + ny]) + hx2*(thermalArray[nx*sizeY + ny - 1] +\
               thermalArray[nx*sizeY + ny + 1]))/(2*(hx2 + hy2));
        }

    std::swap(thermalArray, nextThermalArray);
    drawPlot();

    ui->iterationBox->setValue(ui->iterationBox->value() + 1);
    ui->deltaBox->setValue(delta());
}

double MainWindow::delta()
{
    double delta = 0;
    for (int nx = 0; nx < sizeX; nx++)
    for (int ny = 0; ny < sizeY; ny++)
        delta += fabs(thermalArray[nx*sizeY + ny] - nextThermalArray[nx*sizeY + ny]);
    if (delta < 0.001)
        stopCalc();
    return delta;
}
