#-------------------------------------------------
#
# Project created by QtCreator 2016-11-04T14:57:54
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

win32 {
    LIBS += -lopengl32
} else {
# -march=native selects all the SSE/AVX instructions available on the processor.
# For a Sandy Bridge processor, the flowing flags are selected:
# -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 -mavx
    QMAKE_CXXFLAGS += -march=native
}

TARGET = openr3d
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    asset.cpp \
    camera.cpp \
    color.cpp \
    component.cpp \
    glwidget.cpp \
    light.cpp \
    lighting.cpp \
    matrix4.cpp \
    mesh.cpp \
    meshrenderer.cpp \
    opengl.cpp \
    scene.cpp \
    sceneobject.cpp \
    transform.cpp \
    vector3.cpp \
    vector4.cpp

HEADERS  += mainwindow.h \
    vector4.h \
    vector3.h \
    transform.h \
    sceneobject.h \
    scene.h \
    opengl.h \
    meshrenderer.h \
    mesh.h \
    matrix4.h \
    mathtest.h \
    lighting.h \
    light.h \
    glwidget.h \
    component.h \
    color.h \
    camera.h \
    asset.h \
    aligned.h

FORMS    += mainwindow.ui
