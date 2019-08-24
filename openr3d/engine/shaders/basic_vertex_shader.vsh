//OpenGL ES automatically #define GL_ES
#ifndef GL_ES
//Ignore precision specifiers when not using OpenGL ES for desktop OpenGL compatibility
#define lowp
#define mediump
#define highp
#endif

uniform highp mat4 modelViewProjectionMatrix;
attribute highp vec3 in_vertex;

void main() {
        gl_Position = modelViewProjectionMatrix * vec4(in_vertex, 1.0);
}
