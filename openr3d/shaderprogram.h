#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include "opengl.h"

//TODO: Review class architecture, for instand shaders can be safely deleted after program is created

class ShaderProgram
{

public:

    static ShaderProgram* boundShaderProgram;

    enum ShaderType {
        VERTEX,
        TESSELLATION_CTRL,
        TESSELLATION_EVAL,
        GEOMETRY,
        FRAGMENT,
        COMPUTE,
        SHADERTYPE_NBR
    };

    std::vector<std::string> shaderFileNames;
    std::vector<GLuint> shaders;
    GLuint program;

    ShaderProgram();

    int load(std::string shaderFileName, ShaderType type);
    int link();
    void bind();
    static void unbind();

};

#endif // SHADER_H
