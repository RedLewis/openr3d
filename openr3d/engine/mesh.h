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
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> vertices;
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> normals;
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> tangents; //orthogonals to normals and bitangents
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> bitangents; //orthogonals to normals and tangents
    std::vector<Vector3, AlignedAllocator<Vector3, Vector3::alignment_size>> textureCoordinates;

    //Mesh Data gpu memory locations
    GLuint verticesVBO;
    GLuint normalsVBO;
    GLuint tangentsVBO;
    GLuint bitangentsVBO;
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
