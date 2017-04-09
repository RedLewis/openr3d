#include "opengl.h"
#include <iostream>

GL* gl = NULL;

GL::GL(QOpenGLContext& context)
    : QOpenGLFunctions(&context), context(context)
{
    this->initializeOpenGLFunctions();
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, reinterpret_cast<GLint*>(&this->defaultFBO));
}

GL::~GL()
{
}

void GL::printInfo()
{
    const GLubyte* glstr = NULL;

    glstr = this->glGetString(GL_VENDOR);
    if (glstr != NULL)
        std::cout << "GPU Vendor:\t\t" << glstr << std::endl;
    else
        std::cerr << "GPU Vendor:\t\tERROR" << std::endl;

    glstr = this->glGetString(GL_RENDERER);
    if (glstr != NULL)
        std::cout << "GPU Adapter:\t" << glstr << std::endl;
    else
        std::cerr << "GPU Adapter:\tERROR" << std::endl;

    glstr = this->glGetString(GL_VERSION);
    if (glstr != NULL)
        std::cout << "OpenGL Version:\t" << glstr << std::endl;
    else
        std::cerr << "OpenGL Version:\tERROR" << std::endl;

    glstr = this->glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (glstr != NULL)
        std::cout << "GLSL Version:\t" << glstr << std::endl;
    else
        std::cout << "GLSL Version:\tNA" << std::endl;

    /*glstr = this->glGetString(GL_EXTENSIONS);
    if (glstr != NULL)
        std::cout << "OpenGL Extensions:\t" << glstr << std::endl;
    else
        std::cout << "OpenGL Extensions:\tNA" << std::endl;*/
}

void GL::configure()
{
    //OpenGL Behavior (GL_FASTEST, GL_NICEST, GL_DONT_CARE)
    //this->glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //quality of color, texture coordinate, and fog coordinate interpolation
    //this->glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); //sampling quality of antialiased lines
    //this->glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); //sampling quality of antialiased polygons
    //this->glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST); //quality and performance of the compressing texture images

    //Polygon
    //this->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Either GL_FILL, GL_LINE or GL_POINT

    //Depth
    this->glEnable(GL_DEPTH_TEST);
    this->glDepthFunc(GL_LESS); //specifies the conditions under which the pixel will be drawn (default is GL_LESS, pixels with less depth will be drawn first)

    //Transparency
    //WARNING: Transparent objects must be drawn after opaque objects from most opaque to most transparent
    this->glEnable(GL_BLEND);
    this->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Culling
    //this->glEnable(GL_CULL_FACE);
    //this->glCullFace(GL_BACK);

    //Buffers
    this->glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Color set in the buffer atfer a glClear
    this->glClearDepthf(1.0f);
    this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
