#ifndef MATHTEST_H
#define MATHTEST_H

#include "vector3.h"
#include "vector4.h"

class MathTest
{
public:
    MathTest() { }

    void run() {
        Vector3 v3(1, 2, 3);
        Vector4 v4(4, 5, 6, 7);

        std::cout << "Normalizatiom" << std::endl;
        std::cout << v3.normalized() << std::endl;
        std::cout << v4.normalized() << std::endl;
        std::cout << "Magnitude" << std::endl;
        std::cout << v3.magnitude() << std::endl;
        std::cout << v4.magnitude() << std::endl;
        std::cout << "Homogenization" << std::endl;
        std::cout << v4.homogenized() << std::endl;


        std::cout << (v3 = v4.asVector3()).asVector4() << std::endl;

    }

};

#endif // MATHTEST_H
