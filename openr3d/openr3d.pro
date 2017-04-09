#-------------------------------------------------
#
# Project created by QtCreator 2016-11-04T14:57:54
#
#-------------------------------------------------

QT       += core gui openglextensions
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

CONFIG += link_pkgconfig
PKGCONFIG += box2d

win32 {
    LIBS += -lopengl32
} else {
# -march=native selects all the SSE/AVX instructions available on the processor.
# For a Sandy Bridge processor, the flowing flags are selected:
# -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 -mavx
    #QMAKE_CXXFLAGS += -march=native
    QMAKE_CXXFLAGS += -msse4
}

TARGET = openr3d
TEMPLATE = app

INCLUDEPATH += engine \
            engine/SSESIMD/

SOURCES += main.cpp \
    mainwindow.cpp \
    glwidget.cpp \
    engine/box2dUtilities/b2Polygon.cpp \
    engine/box2dUtilities/b2Triangle.cpp \
    engine/SSESIMD/vector3.cpp \
    engine/SSESIMD/vector4.cpp \
    engine/asset.cpp \
    engine/boxcollider2d.cpp \
    engine/camera.cpp \
    engine/circlecollider2d.cpp \
    engine/collider.cpp \
    engine/collider2d.cpp \
    engine/color.cpp \
    engine/color32.cpp \
    engine/component.cpp \
    engine/edgecollider2d.cpp \
    engine/light.cpp \
    engine/lighting.cpp \
    engine/matrix4.cpp \
    engine/mesh.cpp \
    engine/meshrenderer.cpp \
    engine/opengl.cpp \
    engine/physics2d.cpp \
    engine/polygon.cpp \
    engine/polygoncollider2d.cpp \
    engine/rectangle.cpp \
    engine/renderer.cpp \
    engine/scene.cpp \
    engine/sceneobject.cpp \
    engine/screen.cpp \
    engine/shaderprogram.cpp \
    engine/spriterenderer.cpp \
    engine/texture.cpp \
    engine/transform.cpp \
    engine/utilities.cpp \
    engine/vector2.cpp

HEADERS  += \
    mainwindow.h \
    glwidget.h \
    engine/box2dUtilities/b2Polygon.h \
    engine/box2dUtilities/b2Triangle.h \
    engine/SSESIMD/aligned.h \
    engine/SSESIMD/vector3.h \
    engine/SSESIMD/vector4.h \
    engine/asset.h \
    engine/boxcollider2d.h \
    engine/camera.h \
    engine/circlecollider2d.h \
    engine/collider.h \
    engine/collider2d.h \
    engine/color.h \
    engine/color32.h \
    engine/component.h \
    engine/edgecollider2d.h \
    engine/light.h \
    engine/lighting.h \
    engine/mathtest.h \
    engine/matrix4.h \
    engine/mesh.h \
    engine/meshrenderer.h \
    engine/opengl.h \
    engine/physics2d.h \
    engine/polygon.h \
    engine/polygoncollider2d.h \
    engine/rectangle.h \
    engine/renderer.h \
    engine/scene.h \
    engine/sceneobject.h \
    engine/screen.h \
    engine/shaderprogram.h \
    engine/spriterenderer.h \
    engine/texture.h \
    engine/transform.h \
    engine/utilities.h \
    engine/vector2.h \
    timer.hpp

FORMS    += mainwindow.ui

DISTFILES += \
    engine/shaders/basic_fragment_shader.fsh \
    engine/shaders/standard_fragment_shader.fsh \
    engine/shaders/basic_vertex_shader.vsh \
    engine/shaders/standard_vertex_shader.vsh
