#include "mesh.h"
#include "opengl.h"
#include "shaderprogram.h"
#include <fstream>
#include <list>
#include <sstream>

Mesh::Mesh()
   : Asset(Asset::Type::MESH)
{
}

Mesh::Mesh(const std::string& fileName)
   : Asset(Asset::Type::MESH)
{
    this->load(fileName);
}

int Mesh::load(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Mesh::load(\"" << fileName << "\")\tFile not found." << std::endl;
        return -1;
    }
    std::cout << "Mesh::load(\"" << fileName << "\")\tLoading file..." << std::endl;

    //Create std::list tpmVertices and tmpNormals
    std::list<Face> faceList;
    std::list<Vector3> vertexList;
    std::list<Vector3> normalList;
    std::list<Vector3> textureCoordinateList;

    //Load vextrices and normals into the lists
    while (!file.eof()) {
        std::string strLine;
        std::getline(file, strLine);
        std::istringstream ssLine(strLine);
        std::string lineType;

        ssLine >> lineType;

        //Load vertex
        if (lineType == "v") {
            Vector3 vertex;
            ssLine >> vertex.x;
            ssLine >> vertex.y;
            ssLine >> vertex.z;
            vertexList.push_back(vertex);
            //std::cout << "vertex = " << vertex << std::endl;
        }
        //Load space vertex
        else if (lineType == "vp") {
            std::cerr << "Mesh::load\tSpace vertices not supported." << std::endl;
            return -1;
        }
        //Load texture coordinate
        else if (lineType == "vt") {
            Vector3 textureCoordinate;
            ssLine >> textureCoordinate.x;
            ssLine >> textureCoordinate.y;
            if (!ssLine.eof())
                ssLine >> textureCoordinate.z;
            else
                textureCoordinate.z = 0;
            textureCoordinateList.push_back(textureCoordinate);
            //std::cout << "textureCoordinate = " << textureCoordinate << std::endl;
        }
        //Load normal
        else if (lineType == "vn") {
            Vector3 normal;
            ssLine >> normal.x;
            ssLine >> normal.y;
            ssLine >> normal.z;
            normalList.push_back(normal);
            //std::cout << "normal = " << normal << std::endl;
        }
        //Load face
        else if (lineType == "f") {
            Face face;
            Face faceQuadToTriangle;
            int cornerNbr = 0;
            std::string cornerStr;
            while(cornerNbr < 4 && ssLine >> cornerStr) {
                int vertexIndex;
                int textureCoordinateIndex;
                int normalIndex;

                //Parse cornerStr into vertexIndex, textureCoordinateIndex and normalIndex
                {
                    int i = 0;
                    //Get vertex index
                    if (cornerStr[i] == '/') {
                        vertexIndex = -1;
                        ++i;
                    }
                    else {
                        vertexIndex = std::atoi(cornerStr.c_str() + i) - 1;
                        while (cornerStr[i++] != '/' && cornerStr[i] != '\0');
                    }
                    //Get texture coordinate index
                    if (cornerStr[i] == '/') {
                        textureCoordinateIndex = -1;
                        ++i;
                    }
                    else if (cornerStr[i] == '\0') {
                        textureCoordinateIndex = -1;
                    }
                    else {
                        textureCoordinateIndex = std::atoi(cornerStr.c_str() + i) - 1;
                        while (cornerStr[i++] != '/' && cornerStr[i] != '\0');
                    }
                    //Get normal index
                    if (cornerStr[i] == '\0') {
                        normalIndex = -1;
                    }
                    else {
                        normalIndex = std::atoi(cornerStr.c_str() + i) - 1;
                    }
                }

                if (cornerNbr < 3) {
                    face.vertexIndex[cornerNbr] = vertexIndex;
                    face.textureCoordinateIndex[cornerNbr] = textureCoordinateIndex;
                    face.normalIndex[cornerNbr] = normalIndex;
                }
                if (cornerNbr == 0) {
                    faceQuadToTriangle.vertexIndex[0] = vertexIndex;
                    faceQuadToTriangle.textureCoordinateIndex[0] = textureCoordinateIndex;
                    faceQuadToTriangle.normalIndex[0] = normalIndex;
                }
                else if (cornerNbr == 2) {
                    faceQuadToTriangle.vertexIndex[1] = vertexIndex;
                    faceQuadToTriangle.textureCoordinateIndex[1] = textureCoordinateIndex;
                    faceQuadToTriangle.normalIndex[1] = normalIndex;
                }
                else if (cornerNbr == 3) {
                    faceQuadToTriangle.vertexIndex[2] = vertexIndex;
                    faceQuadToTriangle.textureCoordinateIndex[2] = textureCoordinateIndex;
                    faceQuadToTriangle.normalIndex[2] = normalIndex;
                }

                ++cornerNbr;
            }
            //std::cout << "face = " << face << std::endl;
            faceList.push_back(face);
            if (cornerNbr == 4) {
                //std::cout << "quad to triangle face = " << faceQuadToTriangle << std::endl;
                faceList.push_back(faceQuadToTriangle);
            }
        }

    }//End of file loop

    //Tranfers std::list vector pointers to their respective std::vector class tables
    this->vertices.assign(vertexList.begin(), vertexList.end());
    this->textureCoordinates.assign(textureCoordinateList.begin(), textureCoordinateList.end());
    this->normals.assign(normalList.begin(), normalList.end());
    this->faces.assign(faceList.begin(), faceList.end());

    std::cout << "Mesh::load(\"" << fileName << "\")\tFile loaded." << std::endl;
    this->fileName = fileName;

    /*
    ** TODO: Move the opengl load to a init function called when opengl is ready
    ** TODO: Proper way of NOT having texture coordinates
    */

    /*
    ** Create vertices and normals buffers
    */
    this->verticesBuffer.reserve(this->faces.size() * 3);
    this->normalsBuffer.reserve(this->faces.size() * 3);
    if (textureCoordinates.size() > 0)
        this->textureCoordinatesBuffer.reserve(this->faces.size() * 3);
    for (Face& face : this->faces) {
        verticesBuffer.push_back(this->vertices[face.vertexIndex[0]]);
        verticesBuffer.push_back(this->vertices[face.vertexIndex[1]]);
        verticesBuffer.push_back(this->vertices[face.vertexIndex[2]]);
        normalsBuffer.push_back(this->normals[face.normalIndex[0]]);
        normalsBuffer.push_back(this->normals[face.normalIndex[1]]);
        normalsBuffer.push_back(this->normals[face.normalIndex[2]]);
        if (textureCoordinates.size() > 0) {
            textureCoordinatesBuffer.push_back(this->textureCoordinates[face.textureCoordinateIndex[0]]);
            textureCoordinatesBuffer.push_back(this->textureCoordinates[face.textureCoordinateIndex[1]]);
            textureCoordinatesBuffer.push_back(this->textureCoordinates[face.textureCoordinateIndex[2]]);
        }
    }

    //TODO: Check if glGenBuffers set vbo to 0!

    gl->glGenBuffers(1, &(this->verticesVbo));
    /* Bind VBO as being the active buffer and storing vertex attributes */
    gl->glBindBuffer(GL_ARRAY_BUFFER, this->verticesVbo);
    /* Copy the vertex data to our buffer */
    gl->glBufferData(GL_ARRAY_BUFFER, this->verticesBuffer.size() * sizeof(Vector3), this->verticesBuffer.data(), GL_STATIC_DRAW);

    gl->glGenBuffers(1, &(this->normalsVbo));
    /* Bind VBO as being the active buffer and storing normal attributes */
    gl->glBindBuffer(GL_ARRAY_BUFFER, this->normalsVbo);
    /* Copy the vertex data to our buffer */
    gl->glBufferData(GL_ARRAY_BUFFER, this->normalsBuffer.size() * sizeof(Vector3), this->normalsBuffer.data(), GL_STATIC_DRAW);

    gl->glGenBuffers(1, &(this->textureCoordinatesVbo));
    /* Bind VBO as being the active buffer and storing normal attributes */
    gl->glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVbo);
    /* Copy the vertex data to our buffer */
    gl->glBufferData(GL_ARRAY_BUFFER, this->textureCoordinatesBuffer.size() * sizeof(Vector3), this->textureCoordinatesBuffer.data(), GL_STATIC_DRAW);

    /* OPTIONAL: Unbind vbo */
    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);

    return 0;
}

void Mesh::draw() const
{
    //TODO: All these function recieve size from their respective vector buffer but these should all be the same size anyway otherwise glDrawArrays will segfault

    /* Bind VBO as being the active buffer and storing vertex attributes */
    gl->glBindBuffer(GL_ARRAY_BUFFER, this->verticesVbo);
    /* Specify that our coordinate data is going into attribute index, and contains two floats per vertex */
    gl->glVertexAttribPointer(ShaderProgram::activeShaderProgram->vertexIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0); //Specify a stride to avoid using the 4th element of the vector3
    /* Enable attribute index as being used */
    gl->glEnableVertexAttribArray(ShaderProgram::activeShaderProgram->vertexIndex);

    /* Bind VBO as being the active buffer and storing vertex attributes */
    gl->glBindBuffer(GL_ARRAY_BUFFER, this->normalsVbo);
    /* Specify that our coordinate data is going into attribute index, and contains two floats per vertex */
    gl->glVertexAttribPointer(ShaderProgram::activeShaderProgram->normalIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0); //Specify a stride to avoid using the 4th element of the vector3
    /* Enable attribute index as being used */
    gl->glEnableVertexAttribArray(ShaderProgram::activeShaderProgram->normalIndex);

    //TODO: Proper way of NOT having texture coordinates
    if (textureCoordinatesBuffer.size() > 0) {
        gl->glUniform1i(ShaderProgram::activeShaderProgram->useTextureIndex, 1);
        /* Bind VBO as being the active buffer and storing vertex attributes */
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVbo);
        /* Specify that our coordinate data is going into attribute index, and contains two floats per vertex */
        gl->glVertexAttribPointer(ShaderProgram::activeShaderProgram->textureCoordinateIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0); //Specify a stride to avoid using the 3rd and 4th elements of the vector3
        /* Enable attribute index as being used */
        gl->glEnableVertexAttribArray(ShaderProgram::activeShaderProgram->textureCoordinateIndex);
    }
    else
        gl->glUniform1i(ShaderProgram::activeShaderProgram->useTextureIndex, 0);

    /* OPTIONAL: Unbind vbo */
    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* Invoke glDrawArrays telling that our data is are triangles and we want to draw all the vertexes */
    gl->glDrawArrays(GL_TRIANGLES, 0, this->verticesBuffer.size());

    /* Disable attribute index as being used */
    gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->vertexIndex);
    /* Disable attribute index as being used */
    gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->normalIndex);
    if (textureCoordinatesBuffer.size() > 0) {
        /* Disable attribute index as being used */
        gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->textureCoordinateIndex);
    }

}

std::ostream& operator<<(std::ostream& out, const Mesh::Face& f) {
    for (int i = 0; i < 3; ++i) {
        if (i > 0)
            out << " ";
        if (f.vertexIndex[i] != -1)
           out <<  f.vertexIndex[i];
        out << "/";
        if (f.textureCoordinateIndex[i] != -1)
           out <<  f.textureCoordinateIndex[i];
        out << "/";
        if (f.normalIndex[i] != -1)
           out <<  f.normalIndex[i];
    }
    return out;
}
