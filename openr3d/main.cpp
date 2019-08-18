#include "mainwindow.h"
#include <QApplication>
#include "aligned.h"
#include "vector3.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}
