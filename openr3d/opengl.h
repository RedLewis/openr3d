#ifndef OPENGL_H
#define OPENGL_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <QOpenGLFunctions>

class GL : public QOpenGLFunctions
{

public:

    QOpenGLContext& context;

    GL(QOpenGLContext& context);

    void printContext();
    void configure();
    void perspectiveProjetion(GLdouble fov, GLdouble aspect, GLdouble zNear, GLdouble zFar);

    void drawCube();
    void drawSphere();

};

extern GL* gl;

#endif // OPENGL_H
