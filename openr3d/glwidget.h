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
    Scene *scene = nullptr;
    SceneObject* controlledCameraPtr = nullptr;

    const float frameRateUpdateFrequency = 0.2f;
    float timeSinceLastFrameRateUpdate = 0.f;
    unsigned int renderedFrames = 0;

    //Camera ctrl variables
    bool controlCamera = false;
    int cameraMouseDeltaX = 0;
    int cameraMouseDeltaY = 0;
    bool cameraMoveForward = false;
    bool cameraMoveBackward = false;
    bool cameraMoveLeft = false;
    bool cameraMoveRight = false;

public:
    explicit GLWidget(float framesPerSecond = 0, QWidget *parent = 0);
    ~GLWidget() override;
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *keyEvent) override;
    void keyReleaseEvent(QKeyEvent *keyEvent) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

public slots:
    virtual void update(float deltaTime);

signals:
    void frameRateUpdate(int fps);

private:
    void printContextInformationGL();
    void setupScene();

};

#endif // GLWIDGET_H
