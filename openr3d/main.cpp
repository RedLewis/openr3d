#include "mainwindow.h"
#include <QApplication>

#include <vector>
#include "vector3.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    //w.show();

    Vector3 v3;
    std::vector<Vector3> v;
    v.reserve(10);
    v.push_back(v3);

    //return app.exec();
}
