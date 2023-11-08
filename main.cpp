#include "mainwindow.h"

#include <QApplication>
#include <stdlib.h>
#include <time.h>

#include "audioengine.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
