#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QKeyEvent>
#include "timer.hpp"
#include "opengl.h"
#include "scene.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

private:
    Timer timer;
    Scene *scene;

    const float frameRateUpdateFrequency = 0.2f;
    float timeSinceLastFrameRateUpdate = 0.f;
    unsigned int renderedFrames = 0;

public:
    explicit GLWidget(float framesPerSecond = 0, QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *keyEvent);

public slots:
    virtual void update(float deltaTime);

signals:
    void frameRateUpdate(int fps);

private:
    void printContextInformationGL();

};

#endif // GLWIDGET_H
