//OpenGL ES automatically #define GL_ES
#ifndef GL_ES
//Ignore precision specifiers when not using OpenGL ES for desktop OpenGL compatibility
#define lowp
#define mediump
#define highp
#endif

void main() {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
