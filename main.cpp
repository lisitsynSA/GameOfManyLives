#include "mainwindow.h"
#include <QApplication>
#include "QTime"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("NeuroLife");
    QTime midnight(0,0,0);
    qsrand(static_cast<uint>(midnight.secsTo(QTime::currentTime())));
    MainWindow w;
    w.show();

    return a.exec();
}
