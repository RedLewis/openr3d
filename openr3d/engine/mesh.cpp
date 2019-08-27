#include "mesh.h"
#include "opengl.h"
#include "shaderprogram.h"
#include <fstream>
#include <list>
#include <sstream>
#include <stdexcept>
#include "vector2.h"

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
    if (tangents.size() > 0)
        gl->glDeleteBuffers(1, &tangentsVBO);
    if (bitangents.size() > 0)
        gl->glDeleteBuffers(1, &bitangentsVBO);
    if (textureCoordinates.size() > 0)
        gl->glDeleteBuffers(1, &textureCoordinatesVBO);
}

int Mesh::load(const std::string& fileName)
{
    std::vector<Face> tmpFaces;
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> tmpVertices;
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> tmpNormals;
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> tmpTextureCoordinates;
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> tmpTangents; //orthogonals to normals and bitangents
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> tmpBitangents; //orthogonals to normals and tangents


    /* Parse File into tmp data
    */
    {
        std::list<Face> faceList;
        std::list<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> vertexList;
        std::list<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> normalList;
        std::list<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> textureCoordinateList;

        //Open file
        std::ifstream file(fileName);
        if (!file.is_open()) {
            std::cerr << "Mesh::load(\"" << fileName << "\")\tFile not found." << std::endl;
            return -1;
        }
        std::cout << "Mesh::load(\"" << fileName << "\")\tLoading file..." << std::endl;

        //Load vextrices, normals and texture coordinates into the lists
        std::istringstream ssLine;
        std::string strLine;
        std::getline(file, strLine);
        while (!file.eof()) {
            ssLine.str(strLine);
            std::string lineType;
            ssLine >> lineType;

            //Load vertex
            if (lineType == "v") {
                Vector3 vertex;
                ssLine >> vertex.x;
                ssLine >> vertex.y;
                if (!ssLine.eof())
                    ssLine >> vertex.z;
                else
                    vertex.z = 0;
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
                normalList.push_back(normal.normalized());
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

            ssLine.clear();
            std::getline(file, strLine);
        }// End of file loop
        if (faceList.size() == 0) {
            std::cerr << "Mesh::load\tInvalid file: No faces found." << std::endl;
            return -1;
        }
        if (vertexList.size() == 0) { //Must have either v or vp
            std::cerr << "Mesh::load\tInvalid file: No vertices found." << std::endl;
            return -1;
        }

        // Store list content into vectors
        tmpFaces.assign(faceList.begin(), faceList.end());
        tmpVertices.assign(vertexList.begin(), vertexList.end());
        tmpNormals.assign(normalList.begin(), normalList.end());
        tmpTextureCoordinates.assign(textureCoordinateList.begin(), textureCoordinateList.end());
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

    /* If no normals : generate face normals
    */
    if (tmpNormals.size() == 0) {
        std::cout << "Mesh::load(\"" << fileName << "\")\tNo normals found. Generating per face normals." << std::endl;
        unsigned int vi[3];
        tmpNormals.resize(tmpFaces.size());
        for (unsigned int i = 0; i < tmpFaces.size(); ++i) {
            vi[0] = tmpFaces[i].vertexIndex[0];
            vi[1] = tmpFaces[i].vertexIndex[1];
            vi[2] = tmpFaces[i].vertexIndex[2];
            tmpNormals[i] = ((tmpVertices[vi[1]] - tmpVertices[vi[0]]).cross(tmpVertices[vi[2]] - tmpVertices[vi[0]])).normalized();
            tmpFaces[i].normalIndex[0] = i;
            tmpFaces[i].normalIndex[1] = i;
            tmpFaces[i].normalIndex[2] = i;
        }
    }

    // Generate tangent and bitangent for each normal (orthogonals to normals), this requieres normals and t exture coordinates
    if (tmpNormals.size() > 0 && tmpTextureCoordinates.size() > 0) {
        tmpTangents.resize(tmpNormals.size());
        tmpBitangents.resize(tmpNormals.size());
        for (Face& face : tmpFaces) {
            //Face vertices
            const Vector3& v0 = tmpVertices[face.vertexIndex[0]];
            const Vector3& v1 = tmpVertices[face.vertexIndex[1]];
            const Vector3& v2 = tmpVertices[face.vertexIndex[2]];

            // Edges of the triangle
            Vector3 edge1 = v1-v0;
            Vector3 edge2 = v2-v0;;

            //Face texture coordinates
            const Vector2& uv0(tmpTextureCoordinates[face.textureCoordinateIndex[0]]);
            const Vector2& uv1(tmpTextureCoordinates[face.textureCoordinateIndex[1]]);
            const Vector2& uv2(tmpTextureCoordinates[face.textureCoordinateIndex[2]]);

            //Face texture coordinates deltas
            Vector2 deltaUV1 = uv1 - uv0;
            Vector2 deltaUV2 = uv2 - uv0;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            //For each face's normal: compute tangent and bitangent
            //TODO: Avoid recalculating tangent and bitangent for normals shared by multiple faces
            for (int normalIndex : face.normalIndex) {

                Vector3 faceTangent = ((edge1 * deltaUV2.y   - edge2 * deltaUV1.y)*f);
                Vector3 faceBitangent = ((edge2 * deltaUV1.x   - edge1 * deltaUV2.x)*f);

                //Tangent and bitangent are calculated based on triangle vertices, they are orthogonal to the face's normal, but not to the loaded vertex normals which can differ from the face's normal.
                //Therefore we make the tangent and bitangent orthogonal the loaded normals and not the face's normal by applying the rotation from face normal to vertex normal.
                const Vector3& vertexNormal = tmpNormals[normalIndex];
                Vector3 faceNormal = edge1.cross(edge2).normalized();
                //Test if faceNormal and vertexNormal are on the same line (pointing the same direction or opposite direction) using the mangitude of their cross product:
                //Greater than zero: they are not parallel => calculate a new vertexTangent and vertexBitangent orthogonal to vertexNormal
                //Equal to zero: they are parallel => faceTangent and faceBitangent already orthogonal to vertexNormal (and faceNormal)
                if (std::fabs(faceNormal.cross(vertexNormal).magnitude()) > 0.002f) {
                    //Find rotation axis and rotation angle to rotate faceNormal onto vertexNormal
                    float rotationAngle = std::acos( (faceNormal.dot(vertexNormal)) / (faceNormal.magnitude()*vertexNormal.magnitude()) );
                    Vector3 rotationAxis = faceNormal.cross(vertexNormal).normalized();
                    //Rotate faceTangent and faceBitangent to vertexTangent and vertexBitangent
                    Vector3 vertexTangent = faceTangent.rotated(rotationAngle, rotationAxis).normalized();
                    Vector3 vertexBitangent = faceBitangent.rotated(rotationAngle, rotationAxis).normalized();
                    //Save vertexTangent and vertexBitangent
                    tmpTangents[normalIndex] = vertexTangent;
                    tmpBitangents[normalIndex] = vertexBitangent;

                    /*
                    std::cout << "--- New tangent and bitangent! ---" << std::endl;
                    std::cout << "vertexNormal.cross(faceNormal)=" << vertexNormal.cross(faceNormal) << std::endl;
                    std::cout << "vertexNormal.cross(faceNormal).magnitude()=" << vertexNormal.cross(faceNormal) << std::endl;
                    std::cout << "vertexNormal =" << vertexNormal << std::endl;
                    std::cout << "faceNormal =" << faceNormal << std::endl;
                    std::cout << "faceTangent =" << faceTangent << std::endl;
                    std::cout << "faceBitangent =" << faceBitangent << std::endl;
                    std::cout << "vertexTangent =" << vertexTangent << std::endl;
                    std::cout << "vertexBitangent =" << vertexBitangent << std::endl;

                    if (std::fabs(faceTangent.dot(faceNormal)) < 0.001)
                        std::cout << "faceTangent ORTO faceNormal" << std::endl;
                    else
                        std::cout << "faceTangent NOT ORTO faceNormal" << std::endl;
                    if (std::fabs(faceBitangent.dot(faceNormal)) < 0.001)
                        std::cout << "faceBitangent ORTO faceNormal" << std::endl;
                    else
                        std::cout << "faceBitangent NOT ORTO faceNormal" << std::endl;
                    if (std::fabs(faceTangent.dot(faceBitangent)) < 0.001)
                        std::cout << "faceTangent ORTO faceBitangent" << std::endl;
                    else
                        std::cout << "faceTangent NOT ORTO faceBitangent" << std::endl;

                    if (std::fabs(vertexTangent.dot(vertexNormal)) < 0.001)
                        std::cout << "vertexTangent ORTO vertexNormal" << std::endl;
                    else
                        std::cout << "vertexTangent NOT ORTO vertexNormal" << std::endl;
                    if (std::fabs(vertexBitangent.dot(vertexNormal)) < 0.001)
                        std::cout << "vertexBitangent ORTO vertexNormal" << std::endl;
                    else
                        std::cout << "vertexBitangent NOT ORTO vertexNormal" << std::endl;
                    if (std::fabs(vertexTangent.dot(vertexBitangent)) < 0.001)
                        std::cout << "vertexTangent ORTO vertexBitangent" << std::endl;
                    else
                        std::cout << "vertexTangent NOT ORTO vertexBitangent" << std::endl;

                    std::cout << "---------------------------------" << std::endl;
                    */

                }
                else {
                    //Save faceTangent and faceBitangent
                    tmpTangents[normalIndex] = faceTangent;
                    tmpBitangents[normalIndex] = faceBitangent;

                    /*
                    std::cout << "--- New tangent and bitangent! ---" << std::endl;
                    std::cout << "faceNormal =" << faceNormal << std::endl;
                    std::cout << "faceTangent =" << faceTangent << std::endl;
                    std::cout << "faceBitangent =" << faceBitangent << std::endl;

                    if (std::fabs(faceTangent.dot(faceNormal)) < 0.001)
                        std::cout << "faceTangent ORTO faceNormal" << std::endl;
                    else
                        std::cout << "faceTangent NOT ORTO faceNormal" << std::endl;
                    if (std::fabs(faceBitangent.dot(faceNormal)) < 0.001)
                        std::cout << "faceBitangent ORTO faceNormal" << std::endl;
                    else
                        std::cout << "faceBitangent NOT ORTO faceNormal" << std::endl;
                    if (std::fabs(faceTangent.dot(faceBitangent)) < 0.001)
                        std::cout << "faceTangent ORTO faceBitangent" << std::endl;
                    else
                        std::cout << "faceTangent NOT ORTO faceBitangent" << std::endl;

                    std::cout << "---------------------------------" << std::endl;
                    */

                }

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

    if (tmpTangents.size() > 0 && this->tangents.size() == 0)
        gl->glGenBuffers(1, &(this->tangentsVBO));
    else if (tmpTangents.size() == 0 && this->tangents.size() > 0)
        gl->glDeleteBuffers(1, &(this->tangentsVBO));

    if (tmpBitangents.size() > 0 && this->bitangents.size() == 0)
        gl->glGenBuffers(1, &(this->bitangentsVBO));
    else if (tmpBitangents.size() == 0 && this->bitangents.size() > 0)
        gl->glDeleteBuffers(1, &(this->bitangentsVBO));

    if (tmpTextureCoordinates.size() > 0 && this->textureCoordinates.size() == 0)
        gl->glGenBuffers(1, &(this->textureCoordinatesVBO));
    else if (tmpTextureCoordinates.size() == 0 && this->textureCoordinates.size() > 0)
        gl->glDeleteBuffers(1, &(this->textureCoordinatesVBO));

    //Move tmp data to mesh
    this->faces = std::move(tmpFaces);
    this->vertices = std::move(tmpVertices);
    this->normals = std::move(tmpNormals);
    this->tangents = std::move(tmpTangents);
    this->bitangents = std::move(tmpBitangents);
    this->textureCoordinates = std::move(tmpTextureCoordinates);

    //Update opengl data
    this->update();

    //Finish
    std::cout << "Mesh::load(\"" << fileName << "\")\tFile loaded." << std::endl;
    this->fileName = fileName;
    return 0;
}

void Mesh::update() {
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> tmpVerticesBuffer;
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> tmpNormalsBuffer;
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> tmpTangentsBuffer;
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> tmpBitangentsBuffer;
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> tmpTextureCoordinatesBuffer;

    if (this->vertices.size() > 0)
        tmpVerticesBuffer.reserve(this->faces.size() * 3);
    if (this->normals.size() > 0)
        tmpNormalsBuffer.reserve(this->faces.size() * 3);
    if (this->tangents.size() > 0)
        tmpTangentsBuffer.reserve(this->faces.size() * 3);
    if (this->bitangents.size() > 0)
        tmpBitangentsBuffer.reserve(this->faces.size() * 3);
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

        //Copy face data into tmp buffers
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
        if (this->tangents.size() > 0) {
            tmpTangentsBuffer.push_back(this->tangents[face.normalIndex[0]]);
            tmpTangentsBuffer.push_back(this->tangents[face.normalIndex[1]]);
            tmpTangentsBuffer.push_back(this->tangents[face.normalIndex[2]]);
        }
        if (this->bitangents.size() > 0) {
            tmpBitangentsBuffer.push_back(this->bitangents[face.normalIndex[0]]);
            tmpBitangentsBuffer.push_back(this->bitangents[face.normalIndex[1]]);
            tmpBitangentsBuffer.push_back(this->bitangents[face.normalIndex[2]]);
        }
        if (this->textureCoordinates.size() > 0) {
            tmpTextureCoordinatesBuffer.push_back(this->textureCoordinates[face.textureCoordinateIndex[0]]);
            tmpTextureCoordinatesBuffer.push_back(this->textureCoordinates[face.textureCoordinateIndex[1]]);
            tmpTextureCoordinatesBuffer.push_back(this->textureCoordinates[face.textureCoordinateIndex[2]]);
        }
    }

    if (this->vertices.size() > 0) {
        //TODO: Why regenerate a buffer? Already generated by constructor! Now it is commented, make sure it still works.
        //gl->glGenBuffers(1, &(this->verticesVBO));
        // Bind VBO as being the active buffer and storing vertex attributes
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
        // Copy the vertex data to our buffer
        gl->glBufferData(GL_ARRAY_BUFFER, tmpVerticesBuffer.size() * sizeof(Vector3), tmpVerticesBuffer.data(), GL_STATIC_DRAW);
    }

    if (this->normals.size() > 0) {
        //TODO: Why regenerate a buffer? Already generated by constructor! Now it is commented, make sure it still works.
        //gl->glGenBuffers(1, &(this->normalsVBO));
        // Bind VBO as being the active buffer and storing normal attributes
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
        // Copy the vertex data to our buffer
        gl->glBufferData(GL_ARRAY_BUFFER, tmpNormalsBuffer.size() * sizeof(Vector3), tmpNormalsBuffer.data(), GL_STATIC_DRAW);
    }

    if (this->tangents.size() > 0) {
        // Bind VBO as being the active buffer and storing tangent attributes
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->tangentsVBO);
        // Copy the vertex data to our buffer
        gl->glBufferData(GL_ARRAY_BUFFER, tmpTangentsBuffer.size() * sizeof(Vector3), tmpTangentsBuffer.data(), GL_STATIC_DRAW);
    }

    if (this->bitangents.size() > 0) {
        // Bind VBO as being the active buffer and storing bitangent attributes
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->bitangentsVBO);
        // Copy the vertex data to our buffer
        gl->glBufferData(GL_ARRAY_BUFFER, tmpBitangentsBuffer.size() * sizeof(Vector3), tmpBitangentsBuffer.data(), GL_STATIC_DRAW);
    }

    if (this->textureCoordinates.size() > 0) {
        //TODO: Why regenerate a buffer? Already generated by constructor! Now it is commented, make sure it still works.
        //gl->glGenBuffers(1, &(this->textureCoordinatesVBO));
        // Bind VBO as being the active buffer and storing texture coordinate attributes
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
    if (tangents.size() > 0) {
        // Bind VBO as being the active buffer and storing vertex attributes
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->tangentsVBO);
        // Specify that our coordinate data is going into attribute index, and contains two floats per vertex
        gl->glVertexAttribPointer(ShaderProgram::activeShaderProgram->tangentIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0); //Specify a stride to avoid using the 4th element of the vector3
        // Enable attribute index as being used
        gl->glEnableVertexAttribArray(ShaderProgram::activeShaderProgram->tangentIndex);
    }
    if (bitangents.size() > 0) {
        // Bind VBO as being the active buffer and storing vertex attributes
        gl->glBindBuffer(GL_ARRAY_BUFFER, this->bitangentsVBO);
        // Specify that our coordinate data is going into attribute index, and contains two floats per vertex
        gl->glVertexAttribPointer(ShaderProgram::activeShaderProgram->bitangentIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0); //Specify a stride to avoid using the 4th element of the vector3
        // Enable attribute index as being used
        gl->glEnableVertexAttribArray(ShaderProgram::activeShaderProgram->bitangentIndex);
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
    if (faces.size() > 0) {
        gl->glDrawArrays(GL_TRIANGLES, 0, this->faces.size() * 3);
    }

    // Disable attribute index as being used
    if (vertices.size() > 0)
        gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->vertexIndex);
    // Disable attribute index as being used
    if (normals.size() > 0)
        gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->normalIndex);
    // Disable attribute index as being used
    if (tangents.size() > 0)
        gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->tangentIndex);
    // Disable attribute index as being used
    if (bitangents.size() > 0)
        gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->bitangentIndex);
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
