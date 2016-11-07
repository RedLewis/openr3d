#include "mesh.h"
#include "opengl.h"
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
    std::list<Face*> faceList;
    std::list<Vector3*> vertexList;
    std::list<Vector3*> normalList;
    std::list<Vector3*> textureCoordinateList;

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
            vertexList.push_back(new Vector3(vertex));
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
            textureCoordinateList.push_back(new Vector3(textureCoordinate));
            //std::cout << "textureCoordinate = " << textureCoordinate << std::endl;
        }
        //Load normal
        else if (lineType == "vn") {
            Vector3 normal;
            ssLine >> normal.x;
            ssLine >> normal.y;
            ssLine >> normal.z;
            normalList.push_back(new Vector3(normal));
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
                    face.colorIndex[cornerNbr] = textureCoordinateIndex;
                    face.normalIndex[cornerNbr] = normalIndex;
                }
                if (cornerNbr == 0) {
                    faceQuadToTriangle.vertexIndex[0] = vertexIndex;
                    faceQuadToTriangle.colorIndex[0] = textureCoordinateIndex;
                    faceQuadToTriangle.normalIndex[0] = normalIndex;
                }
                else if (cornerNbr == 2) {
                    faceQuadToTriangle.vertexIndex[1] = vertexIndex;
                    faceQuadToTriangle.colorIndex[1] = textureCoordinateIndex;
                    faceQuadToTriangle.normalIndex[1] = normalIndex;
                }
                else if (cornerNbr == 3) {
                    faceQuadToTriangle.vertexIndex[2] = vertexIndex;
                    faceQuadToTriangle.colorIndex[2] = textureCoordinateIndex;
                    faceQuadToTriangle.normalIndex[2] = normalIndex;
                }

                ++cornerNbr;
            }
            //std::cout << "face = " << face << std::endl;
            faceList.push_back(new Face(face));
            if (cornerNbr == 4) {
                //std::cout << "quad to triangle face = " << faceQuadToTriangle << std::endl;
                faceList.push_back(new Face(faceQuadToTriangle));
            }
        }

    }//End of file loop

    //Tranfers std::list vector pointers to their respective std::vector class tables
    int index;

    this->vertices.resize(vertexList.size());
    index = 0;
    for (Vector3* vertex : vertexList) {
        this->vertices[index++] = vertex;
    }

    this->colors.resize(textureCoordinateList.size());
    index = 0;
    for (Vector3* textureCoordinate : textureCoordinateList) {
        this->colors[index++] = textureCoordinate;
    }

    this->normals.resize(normalList.size());
    index = 0;
    for (Vector3* normal : normalList) {
        this->normals[index++] = normal;
    }

    this->faces.resize(faceList.size());
    index = 0;
    for (Face* face : faceList) {
        this->faces[index++] = face;
    }

    std::cout << "Mesh::load(\"" << fileName << "\")\tFile loaded." << std::endl;
    this->fileName = fileName;
    return 0;
}

void Mesh::draw() const
{
    /* DEPRECATED
    gl->glBegin(GL_TRIANGLES);

    Vector3* tmpVertex;
    Vector3* tmpNormal;
    for (const Face* face : this->faces) {
        //Calculate normals automatically
        //tmpVertex[0] = this->vertices[face->vertexIndex[0]];
        //tmpVertex[1] = this->vertices[face->vertexIndex[1]];
        //tmpVertex[2] = this->vertices[face->vertexIndex[2]];
        //tmpNormal = cross(*tmpVertex[1] - *tmpVertex[0], *tmpVertex[2] - *tmpVertex[0]).normalize();
        //gl->glNormal3f(tmpNormal.x, tmpNormal.y, tmpNormal.z);
        //gl->glVertex3f(tmpVertex[0]->x, tmpVertex[0]->y, tmpVertex[0]->z);
        //gl->glNormal3f(tmpNormal.x, tmpNormal.y, tmpNormal.z);
        //gl->glVertex3f(tmpVertex[1]->x, tmpVertex[1]->y, tmpVertex[1]->z);
        //gl->glNormal3f(tmpNormal.x, tmpNormal.y, tmpNormal.z);
        //gl->glVertex3f(tmpVertex[2]->x, tmpVertex[2]->y, tmpVertex[2]->z);

        //Used stored normals
        for (int i = 0; i < 3; ++i) {
            tmpVertex = this->vertices[face->vertexIndex[i]];
            tmpNormal = this->normals[face->normalIndex[i]];
            gl->glNormal3f(tmpNormal->x, tmpNormal->y, tmpNormal->z);
            gl->glVertex3f(tmpVertex->x, tmpVertex->y, tmpVertex->z);
        }
    }

    gl->glEnd();
    */
}

std::ostream& operator<<(std::ostream& out, const Mesh::Face& f) {
    for (int i = 0; i < 3; ++i) {
        if (i > 0)
            out << " ";
        if (f.vertexIndex[i] != -1)
           out <<  f.vertexIndex[i];
        out << "/";
        if (f.colorIndex[i] != -1)
           out <<  f.colorIndex[i];
        out << "/";
        if (f.normalIndex[i] != -1)
           out <<  f.normalIndex[i];
    }
    return out;
}
