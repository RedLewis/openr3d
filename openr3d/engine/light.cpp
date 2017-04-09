#include "light.h"
#include "opengl.h"
#include "sceneobject.h"
#include "matrix4.h"

Light::Light(SceneObject* sceneObject, Light::Type type)
    : Component(Component::Type::LIGHT, sceneObject), type(type), color(1.0f, 1.0f, 1.0f)
{
}

void Light::update(float deltaTime)
{
    (void)deltaTime;
    /* DEPRECATED
    //The configuration of the light only needs to be called once (or every time it changes)
    //Configure the light at the bindID
    gl->glEnable(bindID);
    //Configure the color of the light
    gl->glLightfv(bindID, GL_AMBIENT, this->ambientColor.ptr());
    gl->glLightfv(bindID, GL_DIFFUSE, this->diffuseColor.ptr());
    gl->glLightfv(bindID, GL_SPECULAR, this->specularColor.ptr());
    //Configure the attenuation properties of the light
    gl->glLightf(bindID, GL_CONSTANT_ATTENUATION, 1.0f);
    gl->glLightf(bindID, GL_LINEAR_ATTENUATION, 0.0f);
    gl->glLightf(bindID, GL_QUADRATIC_ATTENUATION, 0.0f);
    */
}
//TODO: Remove hard coded rotation and review openGL call architecture
//static float rot1 = 0;
void Light::draw() const
{
    /* DEPRECATED
    //The configuration of the light only needs to be called once (or every time it changes)
    gl->glPushMatrix();
    //rot1 += 0.5;
    //gl->glRotatef(rot1, 0, 1, 0);
    Vector3 direction = Matrix4().makeEulerRotation(this->sceneObject->transform.rotation.x, this->sceneObject->transform.rotation.y, this->sceneObject->transform.rotation.z)
            * Vector3(0.0f, 0.0f, 1.0f);
    gl->glLightfv(bindID, GL_POSITION, direction.asVector4(0.0f).ptr()); //Tranformed to vector4 with forth parameter 0.0f for directional light (otherwise the direction becomes the position of a point/spot light)
    gl->glPopMatrix();
    */
}
