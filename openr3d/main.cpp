#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include "vector3.h"
#include <vector>

class Toto : public SafeAligned<32> {
    int i;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}
