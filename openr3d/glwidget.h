#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QKeyEvent>
#include <QTimer>
#include "opengl.h"
#include "scene.h"

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

private:
    QTimer timer;
    Scene *scene;

public:
    explicit GLWidget(int framesPerSecond = 0, QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *keyEvent);

public slots:
    virtual void timeOutSlot();

private:
    void printContextInformationGL();

};

#endif // GLWIDGET_H
