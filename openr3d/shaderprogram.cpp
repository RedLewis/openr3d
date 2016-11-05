#include "shaderprogram.h"
#include <fstream>
#include <iostream>

ShaderProgram* ShaderProgram::boundShaderProgram = NULL;

ShaderProgram::ShaderProgram()
    : shaderFileNames(SHADERTYPE_NBR), shaders(SHADERTYPE_NBR, 0), program(0)
{

}

int ShaderProgram::load(std::string shaderFileName, ShaderType type)
{
    std::string sourceCode;
    std::ifstream file(shaderFileName);

    if (!file.is_open()) {
        std::cerr << "ShaderProgram::load(\"" << shaderFileName << "\")\tFile not found." << std::endl;
        return -1;
    }
    std::cout << "ShaderProgram::load(\"" << shaderFileName << "\")\tLoading file..." << std::endl;

    file.seekg(0, std::ios::end);
    sourceCode.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    sourceCode.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    GLuint shader;
    switch (type) {
        case ShaderType::VERTEX:
            shader = GLFunctions->glCreateShader(GL_VERTEX_SHADER);
        break;
        case ShaderType::TESSELLATION_CTRL:
            shader = GLFunctions->glCreateShader(GL_TESS_CONTROL_SHADER);
        break;
        case ShaderType::TESSELLATION_EVAL:
            shader = GLFunctions->glCreateShader(GL_TESS_EVALUATION_SHADER);
        break;
        case ShaderType::GEOMETRY:
            shader = GLFunctions->glCreateShader(GL_GEOMETRY_SHADER);
        break;
        case ShaderType::FRAGMENT:
            shader = GLFunctions->glCreateShader(GL_FRAGMENT_SHADER);
        break;
        case ShaderType::COMPUTE:
            shader = GLFunctions->glCreateShader(GL_COMPUTE_SHADER);
        break;
        default:
            std::cerr << "ShaderProgram::load(\"" << shaderFileName << "\")\tInvalid shader type." << std::endl;
            return -1;
    }

    const char *sourceCodeStr = sourceCode.c_str();
    GLFunctions->glShaderSource(shader, 1, &sourceCodeStr, NULL);
    GLFunctions->glCompileShader(shader);

    GLint success = GL_FALSE;
    GLFunctions->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint logLength;
        char* logStr;
        std::cerr << "ShaderProgram::load(\"" << shaderFileName << "\")\tCompilation failure." << std::endl;
        GLFunctions->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength) {
            logStr = new char[logLength];
            GLFunctions->glGetShaderInfoLog(shader, logLength, NULL, logStr);
            std::cerr << logStr;
            delete[] logStr;
        }
        GLFunctions->glDeleteShader(shader);
        return -1;
    }
    std::cout << "ShaderProgram::load(\"" << shaderFileName << "\")\tCompilation success." << std::endl;

    if (this->shaders[type] != 0)
        GLFunctions->glDeleteShader(this->shaders[type]);
    this->shaderFileNames[type] = shaderFileName;
    this->shaders[type] = shader;
    return 0;
}

int ShaderProgram::link()
{
    //Create a shader program
    GLuint program = GLFunctions->glCreateProgram();

    std::cout << "ShaderProgram::link()\tLinking shaders into a program..." << std::endl;

    for (GLuint shader : this->shaders) {
        if (shader != 0)
            GLFunctions->glAttachShader(program, shader);
    }
    GLFunctions->glLinkProgram(program);

    GLint success = GL_FALSE;
    GLFunctions->glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint logLength;
        char* logStr;
        std::cerr << "ShaderProgram::link()\tLinking failure." << std::endl;
        GLFunctions->glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength) {
            logStr = new char[logLength];
            GLFunctions->glGetProgramInfoLog(program, logLength, NULL, logStr);
            std::cerr << logStr;
            delete[] logStr;
        }
        GLFunctions->glDeleteProgram(program);
        return -1;
    }
    std::cout << "Linking success." << std::endl;

    if (this->program != 0)
        GLFunctions->glDeleteProgram(this->program);
    this->program = program;
    return 0;
}

void ShaderProgram::bind()
{
    if (boundShaderProgram != this) {
        boundShaderProgram = this;
        GLFunctions->glUseProgram(this->program);
    }
}

void ShaderProgram::unbind()
{
    if (boundShaderProgram != NULL) {
        boundShaderProgram = NULL;
        GLFunctions->glUseProgram(0);
    }
}
