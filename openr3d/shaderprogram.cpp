#include "shaderprogram.h"
#include <fstream>
#include <iostream>

const ShaderProgram* ShaderProgram::activeShaderProgram = NULL;

ShaderProgram::ShaderProgram()
    : shaderFileNames(SHADERTYPE_NBR), shaders(SHADERTYPE_NBR, 0), program(0)
{

}

ShaderProgram::~ShaderProgram()
{
    for (auto shader : this->shaders) {
        if (shader != 0) {
           gl->glDetachShader(this->program, shader);
           gl->glDeleteShader(shader);
        }
    }
    gl->glDeleteProgram(this->program);
}

int ShaderProgram::load(std::string shaderFileName, ShaderType type)
{
    std::string sourceCode;
    std::ifstream file(shaderFileName);

    if (!file.is_open()) {
        std::cerr << "ShaderProgram::load(\"" << shaderFileName << "\")\tShader file not found." << std::endl;
        return -1;
    }
    std::cout << "ShaderProgram::load(\"" << shaderFileName << "\")\tCompiling shader..." << std::endl;

    file.seekg(0, std::ios::end);
    sourceCode.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    sourceCode.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    GLuint shader;
    switch (type) {
        case ShaderType::VERTEX:
            shader = gl->glCreateShader(GL_VERTEX_SHADER);
        break;
        case ShaderType::TESSELLATION_CTRL:
            shader = gl->glCreateShader(GL_TESS_CONTROL_SHADER);
        break;
        case ShaderType::TESSELLATION_EVAL:
            shader = gl->glCreateShader(GL_TESS_EVALUATION_SHADER);
        break;
        case ShaderType::GEOMETRY:
            shader = gl->glCreateShader(GL_GEOMETRY_SHADER);
        break;
        case ShaderType::FRAGMENT:
            shader = gl->glCreateShader(GL_FRAGMENT_SHADER);
        break;
        case ShaderType::COMPUTE:
            shader = gl->glCreateShader(GL_COMPUTE_SHADER);
        break;
        default:
            std::cerr << "ShaderProgram::load(\"" << shaderFileName << "\")\tInvalid shader type." << std::endl;
            return -1;
    }

    const char *sourceCodeStr = sourceCode.c_str();
    gl->glShaderSource(shader, 1, &sourceCodeStr, NULL);
    gl->glCompileShader(shader);

    GLint success = GL_FALSE;
    gl->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint logLength;
        char* logStr;
        std::cerr << "ShaderProgram::load(\"" << shaderFileName << "\")\tCompilation failure." << std::endl;
        gl->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength) {
            logStr = new char[logLength];
            gl->glGetShaderInfoLog(shader, logLength, NULL, logStr);
            std::cerr << logStr;
            delete[] logStr;
        }
        gl->glDeleteShader(shader);
        return -1;
    }
    std::cout << "ShaderProgram::load(\"" << shaderFileName << "\")\tCompilation success." << std::endl;

    if (this->shaders[type] != 0)
        gl->glDeleteShader(this->shaders[type]);
    this->shaderFileNames[type] = shaderFileName;
    this->shaders[type] = shader;
    return 0;
}

int ShaderProgram::link()
{
    //Create a shader program
    GLuint program = gl->glCreateProgram();

    std::cout << "ShaderProgram::link()\tLinking shaders into a program..." << std::endl;

    for (GLuint shader : this->shaders) {
        if (shader != 0)
            gl->glAttachShader(program, shader);
    }

    //OPTION1 : ASSIGN INDEX BEFORE LINKING
    //gl->glBindAttribLocation(program, 0, "in_position");
    //gl->glBindAttribLocation(program, 1, "in_color");

    gl->glLinkProgram(program);

    GLint success = GL_FALSE;
    gl->glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint logLength;
        char* logStr;
        std::cerr << "ShaderProgram::link()\tLinking failure." << std::endl;
        gl->glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength) {
            logStr = new char[logLength];
            gl->glGetProgramInfoLog(program, logLength, NULL, logStr);
            std::cerr << logStr;
            delete[] logStr;
        }
        gl->glDeleteProgram(program);
        return -1;
    }
    std::cout << "ShaderProgram::link()\tLinking success." << std::endl;


    //OPTION2 : GET INDEX AFTER LINKING
    this->modeMatrixIndex = gl->glGetUniformLocation(program, "modelMatrix");
    this->viewMatrixIndex = gl->glGetUniformLocation(program, "viewMatrix");
    this->normalMatrixIndex = gl->glGetUniformLocation(program, "normalMatrix");
    this->modelViewProjectionMatrixIndex = gl->glGetUniformLocation(program, "modelViewProjectionMatrix");
    this->lightDirectionIndex = gl->glGetUniformLocation(program, "lightDirection");
    this->lightColorIndex = gl->glGetUniformLocation(program, "lightColor");
    this->vertexIndex = gl->glGetAttribLocation(program, "in_vertex");
    this->normalIndex = gl->glGetAttribLocation(program, "in_normal");

    if (this->program != 0)
        gl->glDeleteProgram(this->program);
    this->program = program;
    return 0;
}

void ShaderProgram::bind() const
{
    if (activeShaderProgram != this) {
        activeShaderProgram = this;
        gl->glUseProgram(this->program);
    }
}

void ShaderProgram::unbind()
{
    if (activeShaderProgram != NULL) {
        activeShaderProgram = NULL;
        gl->glUseProgram(0);
    }
}
