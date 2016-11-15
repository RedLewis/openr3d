#ifndef TEXTURE_H
#define TEXTURE_H

#include "asset.h"
#include "color.h"
#include "opengl.h"

class Texture : public Asset
{

public:

    unsigned int width;
    unsigned int height;

    GLuint tbo;

    Texture();
    Texture(const std::string& fileName);
    //TODO: Handle the destruction case when no texture have been loaded
    ~Texture();

    //Supports PPM Files
    int load(const std::string& fileName);
    void draw() const;

};

#endif // TEXTURE_H
