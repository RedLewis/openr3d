#include "shaderprogram.h"
#include <fstream>
#include <iostream>

const ShaderProgram* ShaderProgram::activeShaderProgram = NULL;

ShaderProgram::ShaderProgram()
    : fileNames(SHADERTYPE_NBR), shaders(SHADERTYPE_NBR, 0), program(0)
{

}

ShaderProgram::~ShaderProgram()
{
    for (auto shader : this->shaders) {
        if (shader != 0)
            gl->glDeleteShader(shader);
    }
    if (this->program != 0)
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
        case ShaderType::VERTEX: {
            shader = gl->glCreateShader(GL_VERTEX_SHADER);
        } break;
        case ShaderType::TESSELLATION_CTRL: {
            std::cerr << "ShaderProgram::load(\"" << shaderFileName << "\")\tTesselation Control shader not supported." << std::endl;
            return -1;
            //shader = gl->glCreateShader(GL_TESS_CONTROL_SHADER);
        } break;
        case ShaderType::TESSELLATION_EVAL: {
            std::cerr << "ShaderProgram::load(\"" << shaderFileName << "\")\tTesselation Evaluation shader not supported." << std::endl;
            return -1;
            //shader = gl->glCreateShader(GL_TESS_EVALUATION_SHADER);
        } break;
        case ShaderType::GEOMETRY: {
            std::cerr << "ShaderProgram::load(\"" << shaderFileName << "\")\tGeometry shader not supported." << std::endl;
            return -1;
            //shader = gl->glCreateShader(GL_GEOMETRY_SHADER);
        } break;
        case ShaderType::FRAGMENT: {
            shader = gl->glCreateShader(GL_FRAGMENT_SHADER);
        } break;
        case ShaderType::COMPUTE: {
            std::cerr << "ShaderProgram::load(\"" << shaderFileName << "\")\tCompute shader not supported." << std::endl;
            return -1;
            //shader = gl->glCreateShader(GL_COMPUTE_SHADER);
        } break;
        default: {
            std::cerr << "ShaderProgram::load(\"" << shaderFileName << "\")\tInvalid shader type." << std::endl;
            return -1;
        }
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

    // Finishing
    if (this->shaders[type] != 0)
        gl->glDeleteShader(this->shaders[type]);
    this->fileNames[type] = shaderFileName;
    this->shaders[type] = shader;
    return 0;
}

int ShaderProgram::link()
{
    //Create a shader program
    GLuint tmpProgram = gl->glCreateProgram();

    std::cout << "ShaderProgram::link()\tLinking shaders into a program..." << std::endl;


    // Attach shaders for linking
    for (GLuint shader : this->shaders) {
        if (shader != 0)
            gl->glAttachShader(tmpProgram, shader);
    }


    //OPTION1 : ASSIGN INDEX BEFORE LINKING
    //gl->glBindAttribLocation(program, 0, "in_position");
    //gl->glBindAttribLocation(program, 1, "in_color");


    gl->glLinkProgram(tmpProgram);

    GLint success = GL_FALSE;
    gl->glGetProgramiv(tmpProgram, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint logLength;
        char* logStr;
        std::cerr << "ShaderProgram::link()\tLinking failure." << std::endl;
        gl->glGetProgramiv(tmpProgram, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength) {
            logStr = new char[logLength];
            gl->glGetProgramInfoLog(tmpProgram, logLength, NULL, logStr);
            std::cerr << logStr;
            delete[] logStr;
        }
        gl->glDeleteProgram(tmpProgram);
        return -1;
    }
    std::cout << "ShaderProgram::link()\tLinking success." << std::endl;

    // Now that linking is done, detach shaders
    for (GLuint shader : this->shaders) {
        if (shader != 0)
            gl->glDetachShader(tmpProgram, shader);
    }


    //OPTION2 : GET INDEX AFTER
    //OPTION2 : GET INDEX AFTER
    this->textureSamplerIndex = gl->glGetUniformLocation(tmpProgram, "textureSampler");
    if (this->textureSamplerIndex < 0) std::cerr << "ShaderProgram::link()\tUniform \"textureSampler\" not found." << std::endl;
    this->useLightIndex = gl->glGetUniformLocation(tmpProgram, "useLight");
    if (this->useLightIndex < 0) std::cerr << "ShaderProgram::link()\tUniform \"useLight\" not found." << std::endl;
    this->useTextureIndex = gl->glGetUniformLocation(tmpProgram, "useTexture");
    if (this->useTextureIndex < 0) std::cerr << "ShaderProgram::link()\tUniform \"useTexture\" not found." << std::endl;
    this->useVertexColorIndex = gl->glGetUniformLocation(tmpProgram, "useVertexColor");
    if (this->useVertexColorIndex < 0) std::cerr << "ShaderProgram::link()\tUniform \"useVertexColor\" not found." << std::endl;
    this->modelMatrixIndex = gl->glGetUniformLocation(tmpProgram, "modelMatrix");
    if (this->modelMatrixIndex < 0) std::cerr << "ShaderProgram::link()\tUniform \"modelMatrix\" not found." << std::endl;
    this->viewMatrixIndex = gl->glGetUniformLocation(tmpProgram, "viewMatrix");
    if (this->viewMatrixIndex < 0) std::cerr << "ShaderProgram::link()\tUniform \"viewMatrix\" not found." << std::endl;
    this->normalMatrixIndex = gl->glGetUniformLocation(tmpProgram, "normalMatrix");
    if (this->normalMatrixIndex < 0) std::cerr << "ShaderProgram::link()\tUniform \"normalMatrix\" not found." << std::endl;
    this->modelViewProjectionMatrixIndex = gl->glGetUniformLocation(tmpProgram, "modelViewProjectionMatrix");
    if (this->modelViewProjectionMatrixIndex < 0) std::cerr << "ShaderProgram::link()\tUniform \"modelViewProjectionMatrix\" not found." << std::endl;
    this->lightDirectionIndex = gl->glGetUniformLocation(tmpProgram, "lightDirection");
    if (this->lightDirectionIndex < 0) std::cerr << "ShaderProgram::link()\tUniform \"lightDirection\" not found." << std::endl;
    this->lightColorIndex = gl->glGetUniformLocation(tmpProgram, "lightColor");
    if (this->lightColorIndex < 0) std::cerr << "ShaderProgram::link()\tUniform \"lightColor\" not found." << std::endl;
    this->vertexIndex = gl->glGetAttribLocation(tmpProgram, "in_vertex");
    if (this->vertexIndex < 0) std::cerr << "ShaderProgram::link()\tAttribute \"in_vertex\" not found." << std::endl;
    this->normalIndex = gl->glGetAttribLocation(tmpProgram, "in_normal");
    if (this->normalIndex < 0) std::cerr << "ShaderProgram::link()\tAttribute \"in_normal\" not found." << std::endl;
    this->textureCoordinateIndex = gl->glGetAttribLocation(tmpProgram, "in_textureCoordinate");
    if (this->textureCoordinateIndex < 0) std::cerr << "ShaderProgram::link()\tAttribute \"in_textureCoordinate\" not found." << std::endl;
    this->vertexColorIndex = gl->glGetAttribLocation(tmpProgram, "in_vertexColor");
    if (this->vertexColorIndex < 0) std::cerr << "ShaderProgram::link()\tAttribute \"in_vertexColor\" not found." << std::endl;


    // Finishing
    if (this->program != 0)
        gl->glDeleteProgram(this->program);
    this->program = tmpProgram;
    return 0;
}

void ShaderProgram::bind() const
{
    if (this->program != 0 && activeShaderProgram != this) {
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
