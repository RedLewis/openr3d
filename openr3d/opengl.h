#ifndef OPENGL_H
#define OPENGL_H

#define _USE_MATH_DEFINES
#include <cmath>
#include <QOpenGLFunctions>

extern QOpenGLFunctions* GLFunctions;

void GLPrintContext();
void GLConfigure();
void GLPerspectiveProjetion(GLdouble fov, GLdouble aspect, GLdouble zNear, GLdouble zFar);

void GLDrawCube();
void GLDrawSphere();

#endif // OPENGL_H
