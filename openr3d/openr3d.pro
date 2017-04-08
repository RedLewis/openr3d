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

INCLUDEPATH += SSESIMD/

SOURCES += main.cpp \
    mainwindow.cpp \
    glwidget.cpp \
    asset.cpp \
    camera.cpp \
    color.cpp \
    component.cpp \
    light.cpp \
    lighting.cpp \
    matrix4.cpp \
    mesh.cpp \
    meshrenderer.cpp \
    opengl.cpp \
    scene.cpp \
    sceneobject.cpp \
    shaderprogram.cpp \
    texture.cpp \
    transform.cpp \
    screen.cpp \
    rectangle.cpp \
    collider.cpp \
    boxcollider2d.cpp \
    circlecollider2d.cpp \
    polygoncollider2d.cpp \
    edgecollider2d.cpp \
    physics2d.cpp \
    collider2d.cpp \
    polygon.cpp \
    box2dUtilities/b2Polygon.cpp \
    box2dUtilities/b2Triangle.cpp \
    SSESIMD/vector3.cpp \
    SSESIMD/vector4.cpp \
    color32.cpp \
    renderer.cpp \
    spriterenderer.cpp \
    utilities.cpp \
    vector2.cpp

HEADERS  += \
    mainwindow.h \
    glwidget.h \
    asset.h \
    camera.h \
    color.h \
    component.h \
    light.h \
    lighting.h \
    mathtest.h \
    matrix4.h \
    mesh.h \
    meshrenderer.h \
    opengl.h \
    scene.h \
    sceneobject.h \
    shaderprogram.h \
    texture.h \
    transform.h \
    screen.h \
    rectangle.h \
    collider.h \
    boxcollider2d.h \
    circlecollider2d.h \
    polygoncollider2d.h \
    edgecollider2d.h \
    physics2d.h \
    collider2d.h \
    polygon.h \
    box2dUtilities/b2Polygon.h \
    box2dUtilities/b2Triangle.h \
    SSESIMD/aligned.h \
    SSESIMD/vector3.h \
    SSESIMD/vector4.h \
    color32.h \
    renderer.h \
    spriterenderer.h \
    utilities.h \
    vector2.h

FORMS    += mainwindow.ui

DISTFILES += \
    shaders/standard_fragment_shader.fsh \
    shaders/standard_vertex_shader.vsh \
    shaders/basic_fragment_shader.fsh \
    shaders/basic_vertex_shader.vsh
