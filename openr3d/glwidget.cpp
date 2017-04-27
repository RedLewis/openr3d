#include "glwidget.h"
#include <cmath>

GLWidget::GLWidget(float framesPerSecond, QWidget *parent)
    : QOpenGLWidget(parent)
{

    /*
    ** Specify Surface Format
    */

    QSurfaceFormat format;

    //QSurfaceFormat::DefaultRenderableType	: The default unspecified rendering method
    //QSurfaceFormat::OpenGL : Desktop OpenGL rendering
    //QSurfaceFormat::OpenGLES : OpenGL ES 2.0 rendering
    format.setRenderableType(QSurfaceFormat::OpenGLES);
    format.setMajorVersion(2);
    format.setMinorVersion(0);

    //QSurfaceFormat::CompatibilityProfile : Functionality from earlier OpenGL versions is available.
    //QSurfaceFormat::CoreProfile : Functionality deprecated in OpenGL version 3.0 is not available.
    format.setProfile(QSurfaceFormat::CoreProfile);
    //Used to request that deprecated functions be included in the OpenGL context profile. If not specified, you should get a forward compatible context without support functionality marked as deprecated. This requires OpenGL version 3.0 or higher.
    format.setOption(QSurfaceFormat::DeprecatedFunctions, true);
    //Used to request a debug context with extra debugging information.
    format.setOption(QSurfaceFormat::DebugContext, false);

    //Set the preferred number of samples per pixel when multisampliQng is enabled
    format.setSamples(4);

    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setDepthBufferSize(24);

    this->setFormat(format);

    QObject::connect(&timer, SIGNAL(timeout(float)), this, SLOT(update(float)));
    this->timer.start(framesPerSecond);
}

void GLWidget::initializeGL()
{
    /*
    ** OpenGL rendering context is made current when paintGL(), resizeGL(), or initializeGL() is called.
    ** From this context an already initialized, ready-to-be-used QOpenGLFunctions instance is retrievable by calling QOpenGLContext::functions().
    */
    //Make OpenGL Context Functions available
    gl = new GL(*this->context());

    std::cout << "Program compiled for " << sizeof(size_t) * 8 << "bit systems." << std::endl;
    gl->printInfo();
    gl->configure();

    this->scene = new Scene(this->width(), this->height());
}

void GLWidget::resizeGL(int width, int height)
{
    scene->screen.width = width;
    scene->screen.height = height;
}

void GLWidget::paintGL()
{
    //Clear off-screen buffer
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

void GLWidget::update(float deltaTime)
{
    //Update scene (including matrices, positions, physics etc)
    this->scene->update(deltaTime);

    //Trigger a repaint (paintGL)
    this->QOpenGLWidget::update();

    //Calculate and send new FPS value
    renderedFrames += 1;
    timeSinceLastFrameRateUpdate += deltaTime;
    if (timeSinceLastFrameRateUpdate > frameRateUpdateFrequency) {
        emit frameRateUpdate(std::roundf(renderedFrames / timeSinceLastFrameRateUpdate));
        timeSinceLastFrameRateUpdate = 0.0f;
        renderedFrames = 0;
    }
}
