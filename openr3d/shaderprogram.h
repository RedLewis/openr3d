#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include "opengl.h"

//TODO: Review class architecture, for instand shaders can be safely deleted after program is created

class ShaderProgram
{

public:

    static const ShaderProgram* activeShaderProgram;

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

    GLuint modeMatrixIndex;
    GLuint viewMatrixIndex;
    GLuint normalMatrixIndex;
    GLuint modelViewProjectionMatrixIndex;
    GLuint lightDirectionIndex;
    GLuint vertexIndex;
    GLuint normalIndex;
    GLuint colorIndex;

    //TODO Handle detach delete and reatach shader to program
    ShaderProgram();
    ~ShaderProgram();

    int load(std::string shaderFileName, ShaderType type);
    int link();
    void bind() const;
    static void unbind();

};

#endif // SHADER_H
