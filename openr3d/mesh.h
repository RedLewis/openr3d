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

    //Index or pointer?
    //Index allows acces to the vector laction for easier removal
    struct Face {
        int vertexIndex[3];
        int normalIndex[3];
        int textureCoordinateIndex[3];
        friend std::ostream& operator<<(std::ostream& out, const Face& f);
    };
    friend std::ostream& operator<<(std::ostream& out, const Mesh::Face& f);

    //Object should never be references out of this container
    //But we store pointer to vector3 instead of object beacause
    //the stl vector alocation is not memory aligned
    std::vector<Face> faces;
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<Vector3> textureCoordinates;

    GLuint verticesVbo;
    std::vector<Vector3> verticesBuffer;
    GLuint normalsVbo;
    std::vector<Vector3> normalsBuffer;
    GLuint textureCoordinatesVbo;
    std::vector<Vector3> textureCoordinatesBuffer;

    Mesh();
    Mesh(const std::string& fileName);
    //TODO: Handle the destruction case when no date have has been loaded
    ~Mesh();

    //OBJECT FILE
    int load(const std::string& fileName);

    void draw() const;

};

#endif // MESH_H
