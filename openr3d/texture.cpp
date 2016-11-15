#include "texture.h"
#include "shaderprogram.h"
#include <iostream>
#include <string>

Texture::Texture()
    : Asset(Asset::Type::TEXTURE)
{
}

Texture::Texture(const std::string& fileName)
    : Asset(Asset::Type::TEXTURE)
{
    this->load(fileName);
}

Texture::~Texture()
{
    gl->glDeleteTextures(1, &tbo);
}

/** Load a ppm file from disk.
@input filename The location of the PPM file.  If the file is not found, an error message
will be printed and this function will return 0
@input width This will be modified to contain the width of the loaded image, or 0 if file not found
@input height This will be modified to contain the height of the loaded image, or 0 if file not found
@return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char* loadPPM(const char* filename, unsigned int& width, unsigned int& height)
{
    const int BUFSIZE = 128;
    FILE* fp;
    size_t read;
    unsigned char* rawData;
    char buf[3][BUFSIZE];
    //char* retval_fgets;
    //size_t retval_sscanf;

    //Open the texture file
    if ((fp = fopen(filename, "rb")) == NULL)
    {
        std::cerr << "Error: loading ppm: " << filename << ", file not found" << std::endl;
        width = 0;
        height = 0;
        return NULL;
    }

    //Read magic number:
    /*retval_fgets = */fgets(buf[0], BUFSIZE, fp);

    //Read width and height:
    do
    {
        /*retval_fgets = */fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');

    //Set the width and height
    /*retval_sscanf = */sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width = atoi(buf[1]);
    height = atoi(buf[2]);

    //Read maxval:
    do
    {
        /*retval_fgets = */fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');

    //Read image data:
    rawData = new unsigned char[width * height * 3];
    read = fread(rawData, width * height * 3, 1, fp);
    fclose(fp);

    //If the read was a failure, error
    if (read != 1)
    {
        std::cerr << "Error: parsing ppm: " << filename << ", incomplete data" << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }

    return rawData;
}

//TODO: Use a C++ loader for RPM with error managmenet and store texture data in Texture class
int Texture::load(const std::string& fileName)
{
    unsigned char *rawData = loadPPM(fileName.c_str(), this->width, this->height);
    if (rawData == NULL) {
        std::cerr << "Texture::load(\"" << fileName << "\")\tFile not found." << std::endl;
        return -1;
    }
    std::cout << "Texture::load(\"" << fileName << "\")\tLoading file..." << std::endl;

    /*
    ** TODO: Move the opengl load to a init function called when opengl is ready
    */
    //TODO: Check if glGenTextures set tbo to 0!
    // Create Texture Buffer Object
    gl->glGenTextures(1, &(this->tbo));
    // Bind to the texture in OpenGL
    gl->glBindTexture(GL_TEXTURE_2D, this->tbo);
    // Set Parameters
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // Load the bitmap into the bound texture.
    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawData);


    delete rawData;
    std::cout << "Texture::load(\"" << fileName << "\")\tFile loaded." << std::endl;
    this->fileName = fileName;
    return 0;
}

void Texture::draw() const
{
    // Set the active texture unit to texture unit 0.
    gl->glActiveTexture(GL_TEXTURE0);

    // Bind the texture to this unit.
    gl->glBindTexture(GL_TEXTURE_2D, this->tbo);

    // Tell the texture uniform sampler to use this texture in the shader by binding to texture unit 0.
    gl->glUniform1i(ShaderProgram::activeShaderProgram->textureSamplerIndex, 0);
}
