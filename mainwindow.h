#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QSpinBox>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void drawPlot();

public slots:
    void resizeArray();
    void startCalc();
    void stopCalc();
    void Calc();
    double delta();

private:
    Ui::MainWindow *ui;
    QTimer timer;

    QCustomPlot* plot;
    QCPColorMap* colorMap;
    QCPColorScale *colorScale;
    int typePlot;
    QSpinBox *valuePlot;

    int sizeX;
    int sizeY;
    double* thermalArray;
    double* nextThermalArray;
};

#endif // MAINWINDOW_H
