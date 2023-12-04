#include "mainwindow.h"
#include <QApplication>

#define PORT 6
#define BAUD 9600


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    window.show();
    return a.exec();
}

