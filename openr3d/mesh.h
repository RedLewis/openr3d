#ifndef MESH_H
#define MESH_H

#include "asset.h"
#include "vector3.h"
#include "opengl.h"
#include "aligned.h"
#include <vector>
#include <string>

class Mesh : public Asset
{

public:

    struct Face {
        int vertexIndex[3];
        int normalIndex[3];
        int textureCoordinateIndex[3];
        friend std::ostream& operator<<(std::ostream& out, const Face& f);
    };
    friend std::ostream& operator<<(std::ostream& out, const Mesh::Face& f);

    //Mesh Data
    std::vector<Face> faces;
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Vector3> textureCoordinates;

    //Mesh Data gpu memory locations
    GLuint verticesVBO;
    GLuint normalsVBO;
    GLuint textureCoordinatesVBO;

    Mesh();
    Mesh(const std::string& fileName);
    //TODO: Handle the destruction case when no date have has been loaded
    ~Mesh();

    int load(const std::string& fileName);

    //Upload data to gpu, call after mesh modifications
    void update();
    void draw() const;

};

#endif // MESH_H
