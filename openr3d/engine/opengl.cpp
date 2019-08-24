#include "opengl.h"
#include <iostream>
#include <iomanip>

//Uncomment this define to print potential errors when calling opengl functions
//WARNING: This causes the program to crash when built on windows using MSVC2017
//#define GL_DEBUG

GL* gl = NULL;

void GL::init(void (* (*getProcAddress)(const char *procName))(void)) {
    if (gl == NULL)
        gl = new GL(getProcAddress);
    //Save current frame buffer object as defaultFBO
    gl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, reinterpret_cast<GLint*>(&(gl->defaultFBO)));
    //Configure an initial OpenGL state
    gl->configure();
}

void GL::destruct() {
    if (gl != NULL) {
        delete gl;
        gl = NULL;
    }
}

GL::GL(void (* (*getProcAddress)(const char *procName))(void))
    : _getProcAddress(getProcAddress)
{
    this->loadFunctions();
}

GL::~GL()
{
}

void GL::configure()
{
    //OpenGL Behavior (GL_FASTEST, GL_NICEST, GL_DONT_CARE)
    //this->glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //quality of color, texture coordinate, and fog coordinate interpolation
    //this->glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); //sampling quality of antialiased lines
    //this->glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); //sampling quality of antialiased polygons
    //this->glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST); //quality and performance of the compressing texture images

    //Polygon
    //this->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Either GL_FILL, GL_LINE or GL_POINT

    //Depth
    this->glEnable(GL_DEPTH_TEST);
    this->glDepthFunc(GL_LESS); //specifies the conditions under which the pixel will be drawn (default is GL_LESS, pixels with less depth will be drawn first)

    //Transparency
    //WARNING: Transparent objects must be drawn after opaque objects from most opaque to most transparent
    this->glEnable(GL_BLEND);
    this->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Culling
    //this->glEnable(GL_CULL_FACE);
    //this->glCullFace(GL_BACK);

    //Buffers
    this->glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Color set in the buffer atfer a glClear
    this->glClearDepthf(1.0f);
    this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //The engine is written without the use of VAO for compatibility with OpenGL version <=3.1
    //For OpenGL version >3.1 the use of VAO is mandatory, therefore we create and bind a default VAO:
    if (this->_glGenVertexArrays != NULL && this->_glBindVertexArray != NULL) {
        GLuint vao;
        this->glGenVertexArrays(1, &vao);
        this->glBindVertexArray(vao);
        //TODO: The VAO is never deleted
    }
}

void GL::printInfo()
{
    const GLubyte* glstr = NULL;

    glstr = this->glGetString(GL_VENDOR);
    if (glstr != NULL)
        std::cout << "GPU Vendor:\t" << glstr << std::endl;
    else
        std::cerr << "GPU Vendor:\tERROR" << std::endl;

    glstr = this->glGetString(GL_RENDERER);
    if (glstr != NULL)
        std::cout << "GPU Adapter:\t" << glstr << std::endl;
    else
        std::cerr << "GPU Adapter:\tERROR" << std::endl;

    glstr = this->glGetString(GL_VERSION);
    if (glstr != NULL)
        std::cout << "OpenGL Version:\t" << glstr << std::endl;
    else
        std::cerr << "OpenGL Version:\tERROR" << std::endl;

    glstr = this->glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (glstr != NULL)
        std::cout << "GLSL Version:\t" << glstr << std::endl;
    else
        std::cout << "GLSL Version:\tNA" << std::endl;

    /*glstr = this->glGetString(GL_EXTENSIONS);
    if (glstr != NULL)
        std::cout << "OpenGL Extensions:\t" << glstr << std::endl;
    else
        std::cout << "OpenGL Extensions:\tNA" << std::endl;*/
}

bool GL::printErrors(const char* functionName)
{
    GLenum glerror;
    bool error = false;
    while ((glerror = this->glGetError()) != GL_NO_ERROR) {
        if (!error) {
            error = true;
            std::cerr << "GLERROR " << functionName << ":";
        }
        std::cerr << " " << this->errorToString(glerror) << "(0x" << std::setw(4) << std::setfill('0') << std::hex << glerror << ")";
    }
    if (error == true) {
        std::cerr << std::endl;
    }
    return error;
}

const char* GL::errorToString(GLenum glError) {
    switch (glError) {
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_STACK_OVERFLOW:
            return "GL_STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW:
            return "GL_STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_CONTEXT_LOST:
            return "GL_CONTEXT_LOST";
        case GL_TABLE_TOO_LARGE:
            return "GL_TABLE_TOO_LARGE";
        default:
            return "UNKNOWN";
    }
}

void GL::loadFunctions()
{
    _glGetError = (GLenum (*)(void))(_getProcAddress("glGetError"));
    _glViewport = (void (*)(GLint x, GLint y, GLsizei width, GLsizei height))(_getProcAddress("glViewport"));
    _glGenTextures = (void (*)(GLsizei n, GLuint *textures))(_getProcAddress("glGenTextures"));
    _glBindTexture = (void (*)(GLenum target, GLuint texture))(_getProcAddress("glBindTexture"));
    _glTexParameteri = (void (*)(GLenum target, GLenum pname, GLint param))(_getProcAddress("glTexParameteri"));
    _glPixelStorei = (void (*)(GLenum pname, GLint param))(_getProcAddress("glPixelStorei"));
    _glTexImage2D = (void (*)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data))(_getProcAddress("glTexImage2D"));
    _glActiveTexture = (void (*)(GLenum texture))(_getProcAddress("glActiveTexture"));
    _glDeleteTextures = (void (*)(GLsizei n, const GLuint *textures))(_getProcAddress("glDeleteTextures"));
    _glUseProgram = (void (*)(GLuint program))(_getProcAddress("glUseProgram"));
    _glGetString = (const GLubyte* (*)(GLenum name))(_getProcAddress("glGetString"));
    _glGetProgramiv = (void (*)(GLuint program, GLenum pname, GLint *params))(_getProcAddress("glGetProgramiv"));
    _glGetProgramInfoLog = (void (*)(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog))(_getProcAddress("glGetProgramInfoLog"));
    _glGetShaderiv = (void (*)(GLuint shader, GLenum pname, GLint *params))(_getProcAddress("glGetShaderiv"));
    _glGetShaderInfoLog = (void (*)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog))(_getProcAddress("glGetShaderInfoLog"));
    _glGetUniformLocation = (GLint (*)(GLuint program, const GLchar *name))(_getProcAddress("glGetUniformLocation"));
    _glGetAttribLocation = (GLint (*)(GLuint program, const GLchar *name))(_getProcAddress("glGetAttribLocation"));
    _glGenBuffers = (void (*)(GLsizei n, GLuint * buffers))(_getProcAddress("glGenBuffers"));
    _glBindBuffer = (void (*)(GLenum target, GLuint buffer))(_getProcAddress("glBindBuffer"));
    _glBufferData = (void (*)(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage))(_getProcAddress("glBufferData"));
    _glVertexAttribPointer = (void (*)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer))(_getProcAddress("glVertexAttribPointer"));
    _glEnableVertexAttribArray = (void (*)(GLuint index))(_getProcAddress("glEnableVertexAttribArray"));
    _glUniform1i = (void (*)(GLint location, GLint v0))(_getProcAddress("glUniform1i"));
    _glUniform3fv = (void (*)(GLint location, GLsizei count, const GLfloat *value))(_getProcAddress("glUniform3fv"));
    _glUniform4fv = (void (*)(GLint location, GLsizei count, const GLfloat *value))(_getProcAddress("glUniform4fv"));
    _glUniformMatrix4fv = (void (*)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value))(_getProcAddress("glUniformMatrix4fv"));
    _glGetUniformfv = (void (*)(GLuint program, GLint location, GLfloat *params))(_getProcAddress("glGetUniformfv"));
    _glGetUniformiv = (void (*)(GLuint program, GLint location, GLint *params))(_getProcAddress("glGetUniformiv"));
    _glDrawArrays = (void (*)(GLenum mode, GLint first, GLsizei count))(_getProcAddress("glDrawArrays"));
    _glDisableVertexAttribArray = (void (*)(GLuint index))(_getProcAddress("glDisableVertexAttribArray"));
    _glClear = (void (*)(GLbitfield mask))(_getProcAddress("glClear"));
    _glHint = (void (*)(GLenum target, GLenum mode))(_getProcAddress("glHint"));
    _glEnable = (void (*)(GLenum cap))(_getProcAddress("glEnable"));
    _glBlendFunc = (void (*)(GLenum sfactor, GLenum dfactor))(_getProcAddress("glBlendFunc"));
    _glDepthFunc = (void (*)(GLenum func))(_getProcAddress("glDepthFunc"));
    _glCullFace = (void (*)(GLenum mode))(_getProcAddress("glCullFace"));
    _glClearColor = (void (*)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha))(_getProcAddress("glClearColor"));
    _glClearDepthf = (void (*)(GLclampf depth))(_getProcAddress("glClearDepthf"));
    _glCreateProgram = (GLuint (*)(void))(_getProcAddress("glCreateProgram"));
    _glCreateShader = (GLuint (*)(GLenum shaderType))(_getProcAddress("glCreateShader"));
    _glShaderSource = (void (*)(GLuint shader, GLsizei count, const GLchar **string, const GLint *length))(_getProcAddress("glShaderSource"));
    _glCompileShader = (void (*)(GLuint shader))(_getProcAddress("glCompileShader"));
    _glAttachShader = (void (*)(GLuint program, GLuint shader))(_getProcAddress("glAttachShader"));
    _glLinkProgram = (void (*)(GLuint program))(_getProcAddress("glLinkProgram"));
    _glDetachShader = (void (*)(GLuint program, GLuint shader))(_getProcAddress("glDetachShader"));
    _glDeleteShader = (void (*)(GLuint shader))(_getProcAddress("glDeleteShader"));
    _glDeleteProgram = (void (*)(GLuint program))(_getProcAddress("glDeleteProgram"));
    _glDeleteBuffers = (void (*)(GLsizei n, const GLuint * buffers))(_getProcAddress("glDeleteBuffers"));
    _glGetIntegerv = (void (*)(GLenum pname, GLint * params))(_getProcAddress("glGetIntegerv"));
    _glGenVertexArrays = (void (*)(GLsizei n, GLuint *arrays))(_getProcAddress("glGenVertexArrays"));
    _glBindVertexArray = (void (*)(GLuint array))(_getProcAddress("glBindVertexArray"));
    _glDeleteVertexArrays = (void (*)(GLsizei n, const GLuint *arrays))(_getProcAddress("glDeleteVertexArrays"));
}

GLenum GL::glGetError(void) {
    return _glGetError();
}
void GL::glViewport(GLint x, GLint y, GLsizei width, GLsizei height){
    _glViewport(x, y, width, height);
#ifdef GL_DEBUG
    this->printErrors("glViewport");
#endif
}
void GL::glGenTextures(GLsizei n, GLuint *textures){
    _glGenTextures(n, textures);
#ifdef GL_DEBUG
    this->printErrors("glGenTextures");
#endif
}
void GL::glBindTexture(GLenum target, GLuint texture){
    _glBindTexture(target, texture);
#ifdef GL_DEBUG
    this->printErrors("glBindTexture");
#endif
}
void GL::glTexParameteri(GLenum target, GLenum pname, GLint param){
    _glTexParameteri(target, pname, param);
#ifdef GL_DEBUG
    this->printErrors("glTexParameteri");
#endif
}
void GL::glPixelStorei(GLenum pname, GLint param){
    _glPixelStorei(pname, param);
#ifdef GL_DEBUG
    this->printErrors("glPixelStorei");
#endif
}
void GL::glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data){
    _glTexImage2D(target, level, internalFormat, width, height, border, format, type, data);
#ifdef GL_DEBUG
    this->printErrors("glTexImage2D");
#endif
}
void GL::glActiveTexture(GLenum texture){
    _glActiveTexture(texture);
#ifdef GL_DEBUG
    this->printErrors("glActiveTexture");
#endif
}
void GL::glDeleteTextures(GLsizei n, const GLuint *textures){
    _glDeleteTextures(n, textures);
#ifdef GL_DEBUG
    this->printErrors("glDeleteTextures");
#endif
}
void GL::glUseProgram(GLuint program){
    _glUseProgram(program);
#ifdef GL_DEBUG
    this->printErrors("glUseProgram");
#endif
}
const GLubyte* GL::glGetString(GLenum name){
    const GLubyte* ret = _glGetString(name);
#ifdef GL_DEBUG
    this->printErrors("glGetString");
#endif
    return ret;
}
void GL::glGetProgramiv(GLuint program, GLenum pname, GLint *params){
    _glGetProgramiv(program, pname, params);
#ifdef GL_DEBUG
    this->printErrors("glGetProgramiv");
#endif
}
void GL::glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog){
    _glGetProgramInfoLog(program, maxLength, length, infoLog);
#ifdef GL_DEBUG
    this->printErrors("glGetProgramInfoLog");
#endif
}
void GL::glGetShaderiv(GLuint shader, GLenum pname, GLint *params){
    _glGetShaderiv(shader, pname, params);
#ifdef GL_DEBUG
    this->printErrors("glGetShaderiv");
#endif
}
void GL::glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog){
    _glGetShaderInfoLog(shader, maxLength, length, infoLog);
#ifdef GL_DEBUG
    this->printErrors("glGetShaderInfoLog");
#endif
}
GLint GL::glGetUniformLocation(GLuint program, const GLchar *name){
    GLint ret = _glGetUniformLocation(program, name);
#ifdef GL_DEBUG
    this->printErrors("glGetUniformLocation");
#endif
    return ret;
}
GLint GL::glGetAttribLocation(GLuint program, const GLchar *name){
    GLint ret = _glGetAttribLocation(program, name);
#ifdef GL_DEBUG
    this->printErrors("glGetAttribLocation");
#endif
    return ret;
}
void GL::glGenBuffers(GLsizei n, GLuint * buffers){
    _glGenBuffers(n, buffers);
#ifdef GL_DEBUG
    this->printErrors("glGenBuffers");
#endif
}
void GL::glBindBuffer(GLenum target, GLuint buffer){
    _glBindBuffer(target, buffer);
#ifdef GL_DEBUG
    this->printErrors("glBindBuffer");
#endif
}
void GL::glBufferData(GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage){
    _glBufferData(target, size, data, usage);
#ifdef GL_DEBUG
    this->printErrors("glBufferData");
#endif
}
void GL::glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer){
    _glVertexAttribPointer(index, size, type, normalized, stride, pointer);
#ifdef GL_DEBUG
    this->printErrors("glVertexAttribPointer");
#endif
}
void GL::glEnableVertexAttribArray(GLuint index){
    _glEnableVertexAttribArray(index);
#ifdef GL_DEBUG
    this->printErrors("glEnableVertexAttribArray");
#endif
}
void GL::glUniform1i(GLint location, GLint v0){
    _glUniform1i(location, v0);
#ifdef GL_DEBUG
    this->printErrors("glUniform1i");
#endif
}
void GL::glUniform3fv(GLint location, GLsizei count, const GLfloat *value){
    _glUniform3fv(location, count, value);
#ifdef GL_DEBUG
    this->printErrors("glUniform3fv");
#endif
}
void GL::glUniform4fv(GLint location, GLsizei count, const GLfloat *value){
    _glUniform4fv(location, count, value);
#ifdef GL_DEBUG
    this->printErrors("glUniform4fv");
#endif
}
void GL::glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
    _glUniformMatrix4fv(location, count, transpose, value);
#ifdef GL_DEBUG
    this->printErrors("glUniformMatrix4fv");
#endif
}
void GL::glGetUniformfv(GLuint program, GLint location, GLfloat *params){
    _glGetUniformfv(program, location, params);
#ifdef GL_DEBUG
    this->printErrors("glGetUniformfv");
#endif
}
void GL::glGetUniformiv(GLuint program, GLint location, GLint *params){
    _glGetUniformiv(program, location, params);
#ifdef GL_DEBUG
    this->printErrors("glGetUniformiv");
#endif
}
void GL::glDrawArrays(GLenum mode, GLint first, GLsizei count){
    _glDrawArrays(mode, first, count);
#ifdef GL_DEBUG
    this->printErrors("glDrawArrays");
#endif
}
void GL::glDisableVertexAttribArray(GLuint index){
    _glDisableVertexAttribArray(index);
#ifdef GL_DEBUG
    this->printErrors("glDisableVertexAttribArray");
#endif
}
void GL::glClear(GLbitfield mask){
    _glClear(mask);
#ifdef GL_DEBUG
    this->printErrors("glClear");
#endif
}
void GL::glHint(GLenum target, GLenum mode){
    _glHint(target, mode);
#ifdef GL_DEBUG
    this->printErrors("glHint");
#endif
}
void GL::glEnable(GLenum cap){
    _glEnable(cap);
#ifdef GL_DEBUG
    this->printErrors("glEnable");
#endif
}
void GL::glBlendFunc(GLenum sfactor, GLenum dfactor){
    _glBlendFunc(sfactor, dfactor);
#ifdef GL_DEBUG
    this->printErrors("glBlendFunc");
#endif
}
void GL::glDepthFunc(GLenum func){
    _glDepthFunc(func);
#ifdef GL_DEBUG
    this->printErrors("glDepthFunc");
#endif
}
void GL::glCullFace(GLenum mode){
    _glCullFace(mode);
#ifdef GL_DEBUG
    this->printErrors("glCullFace");
#endif
}
void GL::glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha){
    _glClearColor(red, green, blue, alpha);
#ifdef GL_DEBUG
    this->printErrors("glClearColor");
#endif
}
void GL::glClearDepthf(GLclampf depth){
    _glClearDepthf(depth);
#ifdef GL_DEBUG
    this->printErrors("glClearDepthf");
#endif
}
GLuint GL::glCreateProgram(void){
    GLuint ret = _glCreateProgram();
#ifdef GL_DEBUG
    this->printErrors("glCreateProgram");
#endif
    return ret;
}
GLuint GL::glCreateShader(GLenum shaderType){
    GLuint ret = _glCreateShader(shaderType);
#ifdef GL_DEBUG
    this->printErrors("glCreateShader");
#endif
    return ret;
}
void GL::glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length){
    _glShaderSource(shader, count, string, length);
#ifdef GL_DEBUG
    this->printErrors("glShaderSource");
#endif
}
void GL::glCompileShader(GLuint shader){
    _glCompileShader(shader);
#ifdef GL_DEBUG
    this->printErrors("glCompileShader");
#endif
}
void GL::glAttachShader(GLuint program, GLuint shader){
    _glAttachShader(program, shader);
#ifdef GL_DEBUG
    this->printErrors("glAttachShader");
#endif
}
void GL::glLinkProgram(GLuint program){
    _glLinkProgram(program);
#ifdef GL_DEBUG
    this->printErrors("glLinkProgram");
#endif
}
void GL::glDetachShader(GLuint program, GLuint shader){
    _glDetachShader(program, shader);
#ifdef GL_DEBUG
    this->printErrors("glDetachShader");
#endif
}
void GL::glDeleteShader(GLuint shader){
    _glDeleteShader(shader);
#ifdef GL_DEBUG
    this->printErrors("glDeleteShader");
#endif
}
void GL::glDeleteProgram(GLuint program){
    _glDeleteProgram(program);
#ifdef GL_DEBUG
    this->printErrors("glDeleteProgram");
#endif
}
void GL::glDeleteBuffers(GLsizei n, const GLuint * buffers){
    _glDeleteBuffers(n, buffers);
#ifdef GL_DEBUG
    this->printErrors("glDeleteBuffers");
#endif
}
void GL::glGetIntegerv(GLenum pname, GLint * params){
    _glGetIntegerv(pname, params);
#ifdef GL_DEBUG
    this->printErrors("glGetIntegerv");
#endif
}
void GL::glGenVertexArrays(GLsizei n, GLuint *arrays){
    _glGenVertexArrays(n, arrays);
#ifdef GL_DEBUG
    this->printErrors("glGenVertexArrays");
#endif
}
void GL::glBindVertexArray(GLuint array){
    _glBindVertexArray(array);
#ifdef GL_DEBUG
    this->printErrors("glBindVertexArray");
#endif
}
void GL::glDeleteVertexArrays(GLsizei n, const GLuint *arrays){
    _glDeleteVertexArrays(n, arrays);
#ifdef GL_DEBUG
    this->printErrors("glDeleteVertexArrays");
#endif
}
