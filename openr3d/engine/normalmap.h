#ifndef NORMALMAP_H
#define NORMALMAP_H

#include "asset.h"
#include "color.h"
#include "opengl.h"
#include <vector>


//TODO: Use RGB colors instead of RGBA colors
//TODO: Use 3*1 byte unsigned char textures instead of 3*4 bytes float
//That applies to the TBO
class NormalMap : public Asset
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

    NormalMap();
    NormalMap(const std::string& fileName);
    //TODO: Handle the destruction case when no normal map have been loaded
    ~NormalMap();

    //Supports PPM Files
    int load(const std::string& fileName);

    //Upload data to gpu, call after normal map modifications
    void update();
    void draw() const;

};

#endif // NORMALMAP_H
