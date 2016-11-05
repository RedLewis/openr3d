#include "glwidget.h"

GLWidget::GLWidget(int framesPerSecond, QWidget *parent)
    : QOpenGLWidget(parent)
{

    /*
    ** Specify Surface Format
    */

    QSurfaceFormat format;

    //QSurfaceFormat::DefaultRenderableType	: The default unspecified rendering method
    //QSurfaceFormat::OpenGL : Desktop OpenGL rendering
    //QSurfaceFormat::OpenGLES : OpenGL ES 2.0 rendering
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setMajorVersion(3);
    format.setMinorVersion(1);

    //QSurfaceFormat::CompatibilityProfile : Functionality from earlier OpenGL versions is available.
    //QSurfaceFormat::CoreProfile : Functionality deprecated in OpenGL version 3.0 is not available.
    format.setProfile(QSurfaceFormat::CoreProfile);
    //Used to request that deprecated functions be included in the OpenGL context profile. If not specified, you should get a forward compatible context without support functionality marked as deprecated. This requires OpenGL version 3.0 or higher.
    format.setOption(QSurfaceFormat::DeprecatedFunctions, true);
    //Used to request a debug context with extra debugging information.
    format.setOption(QSurfaceFormat::DebugContext, false);

    //Set the preferred number of samples per pixel when multisampling is enabled
    format.setSamples(4);

    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setDepthBufferSize(24);

    this->setFormat(format);


    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
    this->timer.start(1000/framesPerSecond);

    this->scene = new Scene;
}

void GLWidget::initializeGL()
{
    /*
    ** OpenGL rendering context is made current when paintGL(), resizeGL(), or initializeGL() is called.
    ** From this context an already initialized, ready-to-be-used QOpenGLFunctions instance is retrievable by calling QOpenGLContext::functions().
    */
    //Make OpenGL Context Functions available
    GLFunctions = this->context()->functions();


    std::cout << "Program compiled for " << sizeof(size_t) * 8 << "bit systems." << std::endl;
    GLPrintContext();
    GLConfigure();

    //As soon as openGL is ready initialise Scene by calling a first update
    //This sets up the differemt matrices needed before the first draw call
    //(NOT IDEAL : SHOUD NOT REQUIRE OPENGL TO UPDATE SCENE)
    this->scene->update();
}

void GLWidget::resizeGL(int width, int height)
{
    //Set Viewport Matrix : Defines the area of the drawing context (here the whole widget)
    glViewport(0, 0, width, height);

    //Update camera aspect ratio (IMPLEMENT AS EVENT)
    //scene->camera.setAspectRatio((float)width/(float)height);
}

void GLWidget::paintGL()
{
    //Clear off-screen buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Draw Scene
    this->scene->draw();
    //Swap off-screen buffer with on-screen buffer
    //...
}

void GLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
        case Qt::Key_Escape:
            close();
            break;
    }
}

void GLWidget::timeOutSlot()
{
    //Update scene (including matrices, positions, physics etc)
    this->scene->update();

    //Trigger a repaint (paintGL)
    this->QOpenGLWidget::update();
}
