#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <string>
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

    std::vector<std::string> fileNames;
    std::vector<GLuint> shaders;
    GLuint program;

    GLint textureSamplerIndex;
    GLint normalMapSamplerIndex;
    GLint useLightIndex;
    GLint useLightTwoSideIndex;
    GLint useTextureIndex;
    GLint useNormalMapIndex;
    GLint useVertexColorIndex;
    GLint modelMatrixIndex;
    GLint viewMatrixIndex;
    GLint normalMatrixIndex;
    GLint modelViewProjectionMatrixIndex;
    GLint lightDirectionIndex;
    GLint lightColorIndex;
    GLint vertexIndex;
    GLint normalIndex;
    GLint tangentIndex;
    GLint bitangentIndex;
    GLint textureCoordinateIndex;
    GLint vertexColorIndex;

    //TODO Handle detach delete and reatach shader to program
    ShaderProgram();
    ~ShaderProgram();

    int load(std::string shaderFileName, ShaderType type);
    int link();
    void bind() const;
    static void unbind();

};

#endif // SHADER_H
