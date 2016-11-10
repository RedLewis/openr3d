#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include "vector3.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}
