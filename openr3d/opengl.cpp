#include "opengl.h"
#include <iostream>

GL* gl = NULL;

GL::GL(QOpenGLContext& context)
    : QOpenGLFunctions(&context), context(context)
{
    this->initializeOpenGLFunctions();
}

void GL::printContext()
{
    std::cout << "GPU Adapter:\t" << this->glGetString(GL_RENDERER) << std::endl
        << "OpenGL Version:\t" << this->glGetString(GL_VERSION) << std::endl
        << "GLSL Version:\t" << this->glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl
        << std::endl;
}

void GL::configure()
{
    //OpenGL Behavior (GL_FASTEST, GL_NICEST, GL_DONT_CARE)
    this->glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //quality of color, texture coordinate, and fog coordinate interpolation
    this->glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); //sampling quality of antialiased lines
    this->glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); //sampling quality of antialiased polygons
    this->glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST); //quality and performance of the compressing texture images

    //Polygon
    //this->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Either GL_FILL, GL_LINE or GL_POINT

    //Depth
    this->glEnable(GL_DEPTH_TEST);
    this->glDepthFunc(GL_LESS); //specifies the conditions under which the pixel will be drawn (default is GL_LESS, pixels with less depth will be drawn first)

    //Culling
    this->glEnable(GL_CULL_FACE);
    this->glCullFace(GL_BACK);

    //Lighting
    //this->glEnable(GL_LIGHTING);
    //this->glShadeModel(GL_SMOOTH); //Shading model (GL_FLAT or GL_SMOOTH)
    //this->glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE); //GL_TRUE Enables calculating correct surface normals for backface light
    //this->glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); //Specular reflection can be either by view direction (GL_TRUE) or by -z axis (GL_FALSE)
    //this->glEnable(GL_NORMALIZE); //Automatically normalise surface normals

    //Buffers
    this->glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Color set in the buffer atfer a glClear
    //this->glClearDepth(1.0f); //Default depth buffer value atfer a glClear
    this->glClearDepthf(1.0f);
}
