#ifndef TEXTURE_H
#define TEXTURE_H

#include "asset.h"
#include "color.h"
#include "opengl.h"
#include <vector>


//TODO: Use unsigned char textures instead of float
//That applies to the TBO
class Texture : public Asset
{

public:

    unsigned int width;
    unsigned int height;

    std::vector<Color> baseData;
    //Color* reflectionData;
    //Color* refractionData;
    //...

    GLuint baseTBO;
    //GLuint reflectionTBO;
    //GLuint refractionTBO;
    //...

    Texture();
    Texture(const std::string& fileName);
    //TODO: Handle the destruction case when no texture have been loaded
    ~Texture();

    //Supports PPM Files
    int load(const std::string& fileName);

    //Upload data to gpu, call after texture modifications
    void update();
    void draw() const;

};

#endif // TEXTURE_H
