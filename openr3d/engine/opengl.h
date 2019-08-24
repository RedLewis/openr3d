#ifndef OPENGL_H
#define OPENGL_H

//This include is only to expose types (GLint, GLenum, etc)
#include <QOpenGLContext>

class GL
{

public:

    //Function used to get the address of OpenGL functions
    void (* (*_getProcAddress)(const char *procName))(void);

    //OpenGL function addresses
    GLenum (*_glGetError)(void) = NULL;
    void (*_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height) = NULL;
    void (*_glGenTextures)(GLsizei n, GLuint *textures) = NULL;
    void (*_glBindTexture)(GLenum target, GLuint texture) = NULL;
    void (*_glTexParameteri)(GLenum target, GLenum pname, GLint param) = NULL;
    void (*_glPixelStorei)(GLenum pname, GLint param) = NULL;
    void (*_glTexImage2D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data) = NULL;
    void (*_glActiveTexture)(GLenum texture) = NULL;
    void (*_glDeleteTextures)(GLsizei n, const GLuint *textures) = NULL;
    void (*_glUseProgram)(GLuint program) = NULL;
    const GLubyte* (*_glGetString)(GLenum name) = NULL;
    void (*_glGetProgramiv)(GLuint program, GLenum pname, GLint *params) = NULL;
    void (*_glGetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog) = NULL;
    void (*_glGetShaderiv)(GLuint shader, GLenum pname, GLint *params) = NULL;
    void (*_glGetShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog) = NULL;
    GLint (*_glGetUniformLocation)(GLuint program, const GLchar *name) = NULL;
    GLint (*_glGetAttribLocation)(GLuint program, const GLchar *name) = NULL;
    void (*_glGenBuffers)(GLsizei n, GLuint * buffers) = NULL;
    void (*_glBindBuffer)(GLenum target, GLuint buffer) = NULL;
    void (*_glBufferData)(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage) = NULL;
    void (*_glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer) = NULL;
    void (*_glEnableVertexAttribArray)(GLuint index) = NULL;
    void (*_glUniform1i)(GLint location, GLint v0) = NULL;
    void (*_glUniform3fv)(GLint location, GLsizei count, const GLfloat *value) = NULL;
    void (*_glUniform4fv)(GLint location, GLsizei count, const GLfloat *value) = NULL;
    void (*_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) = NULL;
    void (*_glGetUniformfv)(GLuint program, GLint location, GLfloat *params) = NULL;
    void (*_glGetUniformiv)(GLuint program, GLint location, GLint *params) = NULL;
    void (*_glDrawArrays)(GLenum mode, GLint first, GLsizei count) = NULL;
    void (*_glDisableVertexAttribArray)(GLuint index) = NULL;
    void (*_glClear)(GLbitfield mask) = NULL;
    void (*_glHint)(GLenum target, GLenum mode) = NULL;
    void (*_glEnable)(GLenum cap) = NULL;
    void (*_glBlendFunc)(GLenum sfactor, GLenum dfactor) = NULL;
    void (*_glDepthFunc)(GLenum func) = NULL;
    void (*_glCullFace)(GLenum mode) = NULL;
    void (*_glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = NULL;
    void (*_glClearDepthf)(GLclampf depth) = NULL;
    GLuint (*_glCreateProgram)(void) = NULL;
    GLuint (*_glCreateShader)(GLenum shaderType) = NULL;
    void (*_glShaderSource)(GLuint shader, GLsizei count, const GLchar **string, const GLint *length) = NULL;
    void (*_glCompileShader)(GLuint shader) = NULL;
    void (*_glAttachShader)(GLuint program, GLuint shader) = NULL;
    void (*_glLinkProgram)(GLuint program) = NULL;
    void (*_glDetachShader)(GLuint program, GLuint shader) = NULL;
    void (*_glDeleteShader)(GLuint shader) = NULL;
    void (*_glDeleteProgram)(GLuint program) = NULL;
    void (*_glDeleteBuffers)(GLsizei n, const GLuint * buffers) = NULL;
    void (*_glGetIntegerv)(GLenum pname, GLint * params) = NULL;
    void (*_glGenVertexArrays)(GLsizei n, GLuint *arrays) = NULL;
    void (*_glBindVertexArray)(GLuint array) = NULL;
    void (*_glDeleteVertexArrays)(GLsizei n, const GLuint *arrays) = NULL;

    GL(void (* (*getProcAddress)(const char *procName))(void));
    ~GL();
    void loadFunctions();

public:

    //Pass the function used to get OpenGL function address
    //or a lambda that calls that function if the function can not be passed)
    //--- EGL ---
    //The function is void (* eglGetProcAddress( char const * procname))(void)
    //GL::init(&eglGetProcAddress);
    //--- Qt ---
    //For Qt the function is QOpenGLContext::getProcAddress(const char *procName) const
    //Since it is a member function, a lambda must be used:
    //GL::init([](const char* procName)->void(*)(){ return QOpenGLContext::currentContext()->getProcAddress(procName); });

    static void init(void (* (*getProcAddress)(const char *procName))(void));
    static void destruct();

    GLuint defaultFBO;

    void configure();
    void printInfo();
    bool printErrors(const char* functionName);
    const char* errorToString(GLenum glError);

    GLenum glGetError(void);
    void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
    void glGenTextures(GLsizei n, GLuint *textures);
    void glBindTexture(GLenum target, GLuint texture);
    void glTexParameteri(GLenum target, GLenum pname, GLint param);
    void glPixelStorei(GLenum pname, GLint param);
    void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
    void glActiveTexture(GLenum texture);
    void glDeleteTextures(GLsizei n, const GLuint *textures);
    void glUseProgram(GLuint program);
    const GLubyte* glGetString(GLenum name);
    void glGetProgramiv(GLuint program, GLenum pname, GLint *params);
    void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
    void glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
    void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
    GLint glGetUniformLocation(GLuint program, const GLchar *name);
    GLint glGetAttribLocation(GLuint program, const GLchar *name);
    void glGenBuffers(GLsizei n, GLuint * buffers);
    void glBindBuffer(GLenum target, GLuint buffer);
    void glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
    void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
    void glEnableVertexAttribArray(GLuint index);
    void glUniform1i(GLint location, GLint v0);
    void glUniform3fv(GLint location, GLsizei count, const GLfloat *value);
    void glUniform4fv(GLint location, GLsizei count, const GLfloat *value);
    void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void glGetUniformfv(GLuint program, GLint location, GLfloat *params);
    void glGetUniformiv(GLuint program, GLint location, GLint *params);
    void glDrawArrays(GLenum mode, GLint first, GLsizei count);
    void glDisableVertexAttribArray(GLuint index);
    void glClear(GLbitfield mask);
    void glHint(GLenum target, GLenum mode);
    void glEnable(GLenum cap);
    void glBlendFunc(GLenum sfactor, GLenum dfactor);
    void glDepthFunc(GLenum func);
    void glCullFace(GLenum mode);
    void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void glClearDepthf(GLclampf depth);
    GLuint glCreateProgram(void);
    GLuint glCreateShader(GLenum shaderType);
    void glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
    void glCompileShader(GLuint shader);
    void glAttachShader(GLuint program, GLuint shader);
    void glLinkProgram(GLuint program);
    void glDetachShader(GLuint program, GLuint shader);
    void glDeleteShader(GLuint shader);
    void glDeleteProgram(GLuint program);
    void glDeleteBuffers(GLsizei n, const GLuint * buffers);
    void glGetIntegerv(GLenum pname, GLint * params);
    void glGenVertexArrays(GLsizei n, GLuint *arrays);
    void glBindVertexArray(GLuint array);
    void glDeleteVertexArrays(GLsizei n, const GLuint *arrays);

};

extern GL* gl;

#endif // OPENGL_H
