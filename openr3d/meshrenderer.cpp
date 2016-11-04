#include "meshrenderer.h"
#include "opengl.h"

MeshRenderer::MeshRenderer()
    : Component(Component::Type::MESHRENDERER)
{

}

void MeshRenderer::update()
{

}


//TODO: USE MESH AND MATERIAL INSTEAD OF HARD CODING
//TODO: Remove hard coded rotation

void MeshRenderer::draw() const
{
    //GLDrawSphere() must be called at every frame to keep the sphere.

    float objAmbientColor[4] = { 0.0f, 0.1f, 0.1f, 1.0f };
    float objDiffuseColor[4] = { 0.0f, 0.75f, 0.75f, 1.0f };
    float objSpecularColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
    float objEmissionColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float objShininess = 256.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, objAmbientColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, objDiffuseColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, objSpecularColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, objEmissionColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &objShininess);


    //glRotatef(rot1, 0, 1, 0);
    mesh->draw();
    //GLDrawCube();
}
