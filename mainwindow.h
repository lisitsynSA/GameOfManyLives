#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include "lifeframe.h"
#include <QSpinBox>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void drawPlot();

public slots:
    void resizeArray();
    void startCalc();
    void stopCalc();
    void Calc();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    LifeFrame* lifeframe;

    QCustomPlot* plot;
    QCPColorMap* colorMap;
    QCPColorScale *colorScale;
    int typePlot;
    QSpinBox *valuePlot;
};

#endif // MAINWINDOW_H
