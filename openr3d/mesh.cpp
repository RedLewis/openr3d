#include "mesh.h"
#include "opengl.h"
#include "shaderprogram.h"
#include <fstream>
#include <list>
#include <sstream>
#include <stdexcept>

Mesh::Mesh()
   : Asset(Asset::Type::MESH)
{
}

Mesh::Mesh(const std::string& fileName)
   : Asset(Asset::Type::MESH)
{
    this->load(fileName);
}

Mesh::~Mesh()
{
    if (vertices.size() > 0)
        gl->glDeleteBuffers(1, &verticesVBO);
    if (normals.size() > 0)
        gl->glDeleteBuffers(1, &normalsVBO);
    if (textureCoordinates.size() > 0)
        gl->glDeleteBuffers(1, &textureCoordinatesVBO);
}

int Mesh::load(const std::string& fileName)
{
    std::vector<Face> tmpFaces;
    std::vector<Vector3> tmpVertices;
    std::vector<Vector3> tmpNormals;
    std::vector<Vector3> tmpTextureCoordinates;


    /* Parse File into tmp data
    */
    {
        std::list<Face> faceList;
        std::list<Vector3> vertexList;
        std::list<Vector3> normalList;
        std::list<Vector3> textureCoordinateList;

        //Open file
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Mesh::load(\"" << fileName << "\")\tFile not found." << std::endl;
            return -1;
        }
        std::cout << "Mesh::load(\"" << fileName << "\")\tLoading file..." << std::endl;

        //Load vextrices, normals and texture coordinates into the lists
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

        }// End of file loop
        if (faceList.size() == 0) {
            std::cerr << "Mesh::load\tInvalid file: No faces found." << std::endl;
            return -1;
        }
        if (vertexList.size() == 0) {
            std::cerr << "Mesh::load\tInvalid file: No vertices found." << std::endl;
            return -1;
        }

        // Store list content into vectors
        tmpFaces.assign(faceList.begin(), faceList.end());
        tmpVertices.assign(vertexList.begin(), vertexList.end());
        tmpNormals.assign(normalList.begin(), normalList.end());
        tmpTextureCoordinates.assign(textureCoordinateList.begin(), textureCoordinateList.end());
    }


    /* If no normals : generate face normals
    */
    if (tmpNormals.size() == 0) {
        std::cout << "Mesh::load(\"" << fileName << "\")\tNo normals found. Using per face normals." << std::endl;
        unsigned int vi[3];
        tmpNormals.resize(tmpFaces.size());
        for (unsigned int i = 0; i < tmpFaces.size(); ++i) {
            vi[0] = tmpFaces[i].vertexIndex[0];
            vi[1] = tmpFaces[i].vertexIndex[1];
            vi[2] = tmpFaces[i].vertexIndex[2];
            if (vi[0] >= tmpFaces.size() ||
                vi[1] >= tmpFaces.size() ||
                vi[2] >= tmpFaces.size()) {
                std::cerr << "Mesh::load\tInvalid file: Face vertex index missing or out of range." << std::endl;
                return -1;
            }
            tmpNormals[i] = (tmpVertices[vi[1]] - tmpVertices[vi[0]]).cross(tmpVertices[vi[2]] - tmpVertices[vi[0]]).normalize();
            tmpFaces[i].normalIndex[0] = i;
            tmpFaces[i].normalIndex[1] = i;
            tmpFaces[i].normalIndex[2] = i;
        }
    }


    /* Veritfy tmp data
    */
    {
        for (Face& face : tmpFaces) {
            //Vertify face
            if (tmpVertices.size() > 0 &&
                ((unsigned int)face.vertexIndex[0] >= tmpVertices.size() ||
                 (unsigned int)face.vertexIndex[1] >= tmpVertices.size() ||
                 (unsigned int)face.vertexIndex[2] >= tmpVertices.size())) {
                std::cerr << "Mesh::load\tInvalid file: Face vertex index missing or out of range." << std::endl;
                return -1;
            }
            if (tmpNormals.size() > 0 &&
                ((unsigned int)face.normalIndex[0] >= tmpNormals.size() ||
                 (unsigned int)face.normalIndex[1] >= tmpNormals.size() ||
                 (unsigned int)face.normalIndex[2] >= tmpNormals.size())) {
                std::cerr << "Mesh::load\tInvalid file: Face normal index missing or out of range." << std::endl;
                return -1;
            }
            if (tmpTextureCoordinates.size() > 0 &&
                ((unsigned int)face.textureCoordinateIndex[0] >= tmpTextureCoordinates.size() ||
                 (unsigned int)face.textureCoordinateIndex[1] >= tmpTextureCoordinates.size() ||
                 (unsigned int)face.textureCoordinateIndex[2] >= tmpTextureCoordinates.size())) {
                std::cerr << "Mesh::load\tInvalid file: Face texture coordinate index missing or out of range." << std::endl;
                return -1;
            }
        }
    }

    //Init opengl data
    //Create Buffers (if not previously created) and delete now useless buffers

    if (tmpVertices.size() > 0 && this->vertices.size() == 0)
        gl->glGenBuffers(1, &(this->verticesVBO));
    else if (tmpVertices.size() == 0 && this->vertices.size() > 0)
        gl->glDeleteBuffers(1, &(this->verticesVBO));

    if (tmpNormals.size() > 0 && this->normals.size() == 0)
        gl->glGenBuffers(1, &(this->normalsVBO));
    else if (tmpNormals.size() == 0 && this->normals.size() > 0)
        gl->glDeleteBuffers(1, &(this->normalsVBO));

    if (tmpTextureCoordinates.size() > 0 && this->textureCoordinates.size() == 0)
        gl->glGenBuffers(1, &(this->textureCoordinatesVBO));
    else if (tmpTextureCoordinates.size() == 0 && this->textureCoordinates.size() > 0)
        gl->glDeleteBuffers(1, &(this->textureCoordinatesVBO));

    //Move tmp data to mesh
    this->faces = std::move(tmpFaces);
    this->vertices = std::move(tmpVertices);
    this->normals = std::move(tmpNormals);
    this->textureCoordinates = std::move(tmpTextureCoordinates);

    //Update opengl data
    this->update();

    //Finish
    std::cout << "Mesh::load(\"" << fileName << "\")\tFile loaded." << std::endl;
    this->fileName = fileName;
    return 0;
}

void Mesh::update() {
    std::vector<Vector3> tmpVerticesBuffer;
    std::vector<Vector3> tmpNormalsBuffer;
    std::vector<Vector3> tmpTextureCoordinatesBuffer;

    if (this->vertices.size() > 0)
        tmpVerticesBuffer.reserve(this->faces.size() * 3);
    if (this->normals.size() > 0)
        tmpNormalsBuffer.reserve(this->faces.size() * 3);
    if (this->textureCoordinates.size() > 0)
        tmpTextureCoordinatesBuffer.reserve(this->faces.size() * 3);

    for (Face& face : this->faces) {
        //Vertify face
        if (this->vertices.size() > 0 &&
            ((unsigned int)face.vertexIndex[0] >= this->vertices.size() ||
             (unsigned int)face.vertexIndex[1] >= this->vertices.size() ||
             (unsigned int)face.vertexIndex[2] >= this->vertices.size())) {
            throw std::out_of_range("Face vertex index out of range.");
        }
        if (this->normals.size() > 0 &&
            ((unsigned int)face.normalIndex[0] >= this->normals.size() ||
             (unsigned int)face.normalIndex[1] >= this->normals.size() ||
             (unsigned int)face.normalIndex[2] >= this->normals.size())) {
            throw std::out_of_range("Face normal index out of range.");
        }
        if (this->textureCoordinates.size() > 0 &&
            ((unsigned int)face.textureCoordinateIndex[0] >= this->textureCoordinates.size() ||
             (unsigned int)face.textureCoordinateIndex[1] >= this->textureCoordinates.size() ||
             (unsigned int)face.textureCoordinateIndex[2] >= this->textureCoordinates.size())) {
            throw std::out_of_range("Face texture coordinate index out of range.");
        }

        if (this->vertices.size() > 0) {
            tmpVerticesBuffer.push_back(this->vertices[face.vertexIndex[0]]);
            tmpVerticesBuffer.push_back(this->vertices[face.vertexIndex[1]]);
            tmpVerticesBuffer.push_back(this->vertices[face.vertexIndex[2]]);
        }
        if (this->normals.size() > 0) {
            tmpNormalsBuffer.push_back(this->normals[face.normalIndex[0]]);
            tmpNormalsBuffer.push_back(this->normals[face.normalIndex[1]]);
            tmpNormalsBuffer.push_back(this->normals[face.normalIndex[2]]);
        }
        if (this->textureCoordinates.size() > 0) {
            tmpTextureCoordinatesBuffer.push_back(this->textureCoordinates[face.textureCoordinateIndex[0]]);
            tmpTextureCoordinatesBuffer.push_back(this->textureCoordinates[face.textureCoordinateIndex[1]]);
            tmpTextureCoordinatesBuffer.push_back(this->textureCoordinates[face.textureCoordinateIndex[2]]);
        }
    }

    if (this->vertices.size() > 0) {
        gl->glGenBuffers(1, &(this->verticesVBO));
        // Bind VBO as being the active buffer and storing vertex attributes
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
        // Copy the vertex data to our buffer
        gl->glBufferData(GL_ARRAY_BUFFER, tmpVerticesBuffer.size() * sizeof(Vector3), tmpVerticesBuffer.data(), GL_STATIC_DRAW);
    }

    if (this->normals.size() > 0) {
        gl->glGenBuffers(1, &(this->normalsVBO));
        // Bind VBO as being the active buffer and storing normal attributes
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
        // Copy the vertex data to our buffer
        gl->glBufferData(GL_ARRAY_BUFFER, tmpNormalsBuffer.size() * sizeof(Vector3), tmpNormalsBuffer.data(), GL_STATIC_DRAW);
    }

    if (this->textureCoordinates.size() > 0) {
        gl->glGenBuffers(1, &(this->textureCoordinatesVBO));
        // Bind VBO as being the active buffer and storing normal attributes
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVBO);
        // Copy the vertex data to our buffer
        gl->glBufferData(GL_ARRAY_BUFFER, tmpTextureCoordinatesBuffer.size() * sizeof(Vector3), tmpTextureCoordinatesBuffer.data(), GL_STATIC_DRAW);
    }

    // OPTIONAL: Unbind VBO
    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw() const
{
    //TODO: All these function recieve size from their respective vector buffer but these should all be the same size anyway otherwise glDrawArrays will segfault

    if (vertices.size() > 0) {
        // Bind VBO as being the active buffer and storing vertex attributes
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
        // Specify that our coordinate data is going into attribute index, and contains two floats per vertex
        gl->glVertexAttribPointer(ShaderProgram::activeShaderProgram->vertexIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0); //Specify a stride to avoid using the 4th element of the vector3
        // Enable attribute index as being used
        gl->glEnableVertexAttribArray(ShaderProgram::activeShaderProgram->vertexIndex);
    }
    if (normals.size() > 0) {
        // Bind VBO as being the active buffer and storing vertex attributes
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
        // Specify that our coordinate data is going into attribute index, and contains two floats per vertex
        gl->glVertexAttribPointer(ShaderProgram::activeShaderProgram->normalIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0); //Specify a stride to avoid using the 4th element of the vector3
        // Enable attribute index as being used
        gl->glEnableVertexAttribArray(ShaderProgram::activeShaderProgram->normalIndex);
    }
    if (textureCoordinates.size() > 0) {
        // Bind VBO as being the active buffer and storing vertex attributes
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVBO);
        // Specify that our coordinate data is going into attribute index, and contains two floats per vertex
        gl->glVertexAttribPointer(ShaderProgram::activeShaderProgram->textureCoordinateIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0); //Specify a stride to avoid using the 3rd and 4th elements of the vector3
        // Enable attribute index as being used
        gl->glEnableVertexAttribArray(ShaderProgram::activeShaderProgram->textureCoordinateIndex);
    }
    else
        gl->glUniform1i(ShaderProgram::activeShaderProgram->useTextureIndex, 0);
    // OPTIONAL: Unbind VBO
    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Invoke glDrawArrays telling that our data are triangles and we want to draw all the vertexes
    if (faces.size() > 0)
        gl->glDrawArrays(GL_TRIANGLES, 0, this->faces.size() * 3);


    // Disable attribute index as being used
    if (vertices.size() > 0)
        gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->vertexIndex);
    // Disable attribute index as being used
    if (normals.size() > 0)
        gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->normalIndex);
    // Disable attribute index as being used
    if (textureCoordinates.size() > 0)
        gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->textureCoordinateIndex);

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
