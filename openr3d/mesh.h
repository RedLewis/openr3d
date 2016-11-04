#ifndef MESH_H
#define MESH_H

#include "asset.h"
#include "vector3.h"
#include <vector>
#include <string>

class Mesh : public Asset {

private:

    //Index or pointer?
    //Index allows acces to the vector laction for easier removal
    struct Face {
        int vertexIndex[3];
        int normalIndex[3];
        int colorIndex[3];
        friend std::ostream& operator<<(std::ostream& out, const Face& f);
    };
    friend std::ostream& operator<<(std::ostream& out, const Mesh::Face& f);

    //Object should never be references out of this container
    //But we store pointer to vector3 instead of object beacause
    //the stl vector alocation is not memory aligned
    std::vector<Face*> faces;
    std::vector<Vector3*> vertices;
    std::vector<Vector3*> normals;
    std::vector<Vector3*> colors;

public:

    Mesh();
    Mesh(const std::string& fileName);

    //OBJECT FILE
    int load(const std::string& fileName);

    void draw() const;

};

#endif // MESH_H
