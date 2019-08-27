#include "normalmap.h"
#include "shaderprogram.h"
#include <iostream>
#include <string>

NormalMap::NormalMap()
    : Asset(Asset::Type::NORMAL_MAP)
{
}

NormalMap::NormalMap(const std::string& fileName)
    : Asset(Asset::Type::NORMAL_MAP)
{
    this->load(fileName);
}

NormalMap::~NormalMap()
{
    if (baseData.size() > 0) {
        gl->glDeleteTextures(1, &baseTBO);
    }
}

/** Load a ppm file from disk.
@input filename The location of the PPM file.  If the file is not found, an error message
will be printed and this function will return 0
@input width This will be modified to contain the width of the loaded image, or 0 if file not found
@input height This will be modified to contain the height of the loaded image, or 0 if file not found
@return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
static unsigned char* loadPPM(const char* filename, unsigned int& width, unsigned int& height)
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
        std::cerr << "NormalMap::load(\"" << filename << "\")\tFile not found." << std::endl;
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
        std::cerr << "NormalMap::load(\"" << filename << "\")\tInvalid file." << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }

    return rawData;
}

//TODO: Use a C++ loader for PPM with error managmenet and store texture data in Texture class
int NormalMap::load(const std::string& fileName)
{
    std::vector<Color> tmpBaseData;

    unsigned char *rawData = loadPPM(fileName.c_str(), this->width, this->height);
    if (rawData == NULL)
        return -1;
    std::cout << "NormalMap::load(\"" << fileName << "\")\tLoading file..." << std::endl;

    /* Convert raw data to base data
    */

    tmpBaseData.resize(this->width * this->height);
    for (unsigned int i = 0; i < tmpBaseData.size(); ++i) {
        tmpBaseData[i].set((float)(rawData[0]) / 255.f, (float)(rawData[1]) / 255.f, (float)(rawData[2]) / 255.f);
        rawData += 3;
    }
    rawData -= tmpBaseData.size() * 3;
    delete[] rawData;

    //Init opengl data
    // Create Texture Buffer Object (if not previously created)  and delete now useless buffers
    if (tmpBaseData.size() > 0 && this->baseData.size() == 0) {
        gl->glGenTextures(1, &(this->baseTBO));
        // Bind to the texture in OpenGL
        gl->glBindTexture(GL_TEXTURE_2D, this->baseTBO);
        // Set Parameters
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // OPTIONAL: Unbind TBO
        gl->glBindTexture(GL_TEXTURE_2D, 0);
    }
    else if (tmpBaseData.size() == 0 && this->baseData.size() > 0)
        gl->glDeleteTextures(1, &baseTBO);

    //Move tmp data to texture
    this->baseData = std::move(tmpBaseData);

    //Update opengl data
    this->update();

    //Finish
    std::cout << "Texture::load(\"" << fileName << "\")\tFile loaded." << std::endl;
    this->fileName = fileName;
    return 0;
}

// TODO: Remove alternative
// TODO: Remove TBO if baseDate has been emptied? (careful not to remove it twice) (Does an empty texture make sense?)
#include "color32.h"
void NormalMap::update() {
    if (this->baseData.size() > 0) {
        // Bind to the texture in OpenGL
        gl->glBindTexture(GL_TEXTURE_2D, this->baseTBO);
        //Specify row alignment or highest power of two the image width can be devided by. Higher row alignment is faster.
        //TODO: Calculate alignment?
        gl->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // Load the data into the bound texture.
        //gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_FLOAT, this->baseData.data());
        // TODO: Remove alternative
        std::vector<Color32> tmpColor32Data(this->baseData.size());
        for (unsigned int i = 0; i < tmpColor32Data.size(); ++i) {
            tmpColor32Data[i].set(this->baseData[i].getHex());
        }
        gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmpColor32Data.data());
    }
    // OPTIONAL: Unbind TBO
    gl->glBindTexture(GL_TEXTURE_2D, 0);

}

void NormalMap::draw() const
{
    if (this->baseData.size() > 0) {
        gl->glUniform1i(ShaderProgram::activeShaderProgram->useNormalMapIndex, 1);

        // Tell the shader's texture uniform sampler to use texture unit 1
        gl->glUniform1i(ShaderProgram::activeShaderProgram->normalMapSamplerIndex, 1);
        // Set the active texture unit to texture unit 1
        gl->glActiveTexture(GL_TEXTURE1);
        // Bind the texture to this unit
        gl->glBindTexture(GL_TEXTURE_2D, this->baseTBO);

        //TODO: Unbind Texture for each glActiveTexture, must be done after drawing mesh
        //OPTIONAL: Unbind TBO
        //gl->glBindTexture(GL_TEXTURE_2D, 0);
    }
}
