#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include "vector3.h"
#include <vector>

/*class Base
{
    float t[4] = {1, 2, 3, 4};
};

class Safe : Aligned<16, true>
{
    float t[4] = {1, 2, 3, 4};
};

class UnSafe : Aligned<16, false>
{
    float t[4] = {1, 2, 3, 4};
};

class Unknown : Aligned<16>
{
    float t[4] = {1, 2, 3, 4};
};

class Huge : public Aligned<128>
{
    float t[4] = {1, 2, 3, 4};
};*/

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    /*std::cout << "sizeof(Base) = " << sizeof(Base) << std::endl;
    std::cout << "sizeof(Safe) = " << sizeof(Safe) << std::endl;
    std::cout << "sizeof(UnSafe) = " << sizeof(UnSafe) << std::endl;
    std::cout << "sizeof(Unknown) = " << sizeof(Unknown) << std::endl;


    {
    std::vector<int, AlignedAllocator<int, 4>> vec1;
    vec1.reserve(2);
    }
    {
    std::vector<Vector3, AlignedAllocator<Vector3>> vec2;
    vec2.resize(2);
    }


    {
    std::vector<Huge, AlignedAllocator<Huge, 128>> vec1;
    vec1.reserve(2);
    vec1.resize(2);
    }*/

    return app.exec();
}
