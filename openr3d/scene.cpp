#include "scene.h"
#include "opengl.h"
#include "camera.h"
#include "light.h"
#include "meshrenderer.h"
#include "shaderprogram.h"

Scene::Scene()
{
    //TODO: Functionalize this process (the creation linking of the sceneObject and the component)

    //Setup Camera
    SceneObject* cameraObject = new SceneObject(this);
    //cameraObject->transform.position = {0, 1, 2};
    //cameraObject->transform.rotation = {-0.5f, 0, 0};
    cameraObject->transform.position = {0, 0, 5.0f};
    cameraObject->transform.rotation = {0, 0, 0};
    Camera* cameraComponent = new Camera;
    cameraComponent->sceneObject = cameraObject;
    cameraObject->components[Component::CAMERA] = cameraComponent;
    this->sceneObjects.push_back(cameraObject);

    //Setup Light
    SceneObject* lightObject = new SceneObject(this);
    lightObject->transform.rotation = {0, 1.57f/2.0f, 0};
    Light* lightComponent = new Light(Light::Type::DIRECTIONAL);
    lightComponent->sceneObject = lightObject;
    lightComponent->color.set(1.0, 1.0, 1.0, 1.0);
    lightObject->components[Component::LIGHT] = lightComponent;
    this->sceneObjects.push_back(lightObject);

    //Setup Mesh
    SceneObject* meshObjectParent = new SceneObject(this);
    meshObjectParent->transform.position = {0, -1.5f, 0};
    meshObjectParent->transform.rotation = {0, 1.0f, 0};
    meshObjectParent->transform.scale = {1, 1, 1};
    MeshRenderer* meshRendererComponent = new MeshRenderer;
    meshRendererComponent->sceneObject = meshObjectParent;
    meshRendererComponent->mesh = new Mesh("../assets/Tree/Tree.obj");
    meshRendererComponent->texture = new Texture("../assets/Tree/Tree.ppm");
    meshObjectParent->components[Component::MESHRENDERER] = meshRendererComponent;
    this->sceneObjects.push_back(meshObjectParent);

    SceneObject* meshObjectChild = new SceneObject(this);
    meshObjectChild->transform.position = {2, 1, 0};
    meshObjectChild->transform.rotation = {0, 0, 0};
    meshObjectChild->transform.scale = {1, 1, 1};
    meshRendererComponent = new MeshRenderer;
    meshRendererComponent->sceneObject = meshObjectChild;
    meshRendererComponent->mesh = new Mesh("../assets/bunny.obj");
    meshObjectChild->components[Component::MESHRENDERER] = meshRendererComponent;
    meshObjectChild->parent = meshObjectParent;
    meshObjectParent->children.push_back(meshObjectChild);

    //TODO: Clean way of setting up shader
    this->activeCamera = cameraObject;
    this->activeLight = lightObject;
    standardShader.load("../openr3d/standard_vertex_shader.vsh", ShaderProgram::VERTEX);
    standardShader.load("../openr3d/standard_fragment_shader.fsh", ShaderProgram::FRAGMENT);
    standardShader.link();
}

void Scene::draw() const
{
    /*
     * ARCHITECTURE
     *

    //initialization of shader program
    //create shader...
    glGetAttribLocation or glBindAttribLocation
    glGetUniformLocation

    //initialization of object
    glGenBuffers
    glBindBuffer
    glBufferData

    ...

    //rendering of object
    glBindBuffer
    glVertexAttribPointer
    glEnableVertexAttribArray
    glUseProgram
    glUniformMatrix4fv (camera matrix)
    glDraw...
    glDisableVertexAttribArray

    */

//    /*
//    ** <EXPERIMENTAL DRAW>
//    */

//    /*
//    ** Initialization
//    */

//    /* Create handles for our Vertex Array Object and two Vertex Buffer Objects (vertices and colors) */
//    GLuint vbo[2];

//    /* We're going to create a simple flat multicolored diamond */
//    const GLfloat diamond[4][3] = {
//    {  0.0,  1.0, 0.0 }, /* Top point */
//    {  1.0,  0.0, 0.0 }, /* Right point */
//    {  0.0, -1.0, 0.0 }, /* Bottom point */
//    { -1.0,  0.0, 0.0 } }; /* Left point */
//    const GLfloat colors[4][3] = {
//    {  1.0,  0.0,  0.0 }, /* Red */
//    {  0.0,  1.0,  0.0 }, /* Green */
//    {  0.0,  0.0,  1.0 }, /* Blue */
//    {  1.0,  1.0,  1.0 } }; /* White */

//    /* Allocate and assign 2 Vertex Buffer Objects to our handle */
//    gl->glGenBuffers(2, vbo);
//    /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
//    gl->glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//    /* Copy the vertex data from diamond to our buffer */
//    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(diamond), diamond, GL_STATIC_DRAW);
//    /* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
//    gl->glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//    /* Copy the color data from colors to our buffer */
//    /* 12 * sizeof(GLfloat) is the size of the colors array, since it contains 12 GLfloat values */
//    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

//    /*
//    ** Draw Loop
//    */

//    /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
//    gl->glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
//    /* Specify that our coordinate data is going into attribute index 0, and contains two floats per vertex */
//    gl->glVertexAttribPointer(standardShader.vertexIndex, 3, GL_FLOAT, GL_FALSE, 0, 0); //Make stride of 4 bytes when using SSE Vector3
//    /* Enable attribute index as being used */
//    gl->glEnableVertexAttribArray(standardShader.vertexIndex);


//    /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
//    gl->glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//    /* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
//    gl->glVertexAttribPointer(standardShader.colorIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
//    /* Enable attribute index as being used */
//    gl->glEnableVertexAttribArray(standardShader.colorIndex);

//    /* OPTIONAL: Unbind current bounded buffer so no buffer are bounded */
//    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);

//    /* Activate Shader porgram */
//    standardShader.bind();
//    /* Load camera matrix */
//    gl->glUniformMatrix4fv(standardShader.mvpMatrixIndex, 1, GL_FALSE, this->activeCamera->pci.ptr());


//    /* Invoke glDrawArrays telling that our data is a line loop and we want to draw 4 vertexes */
//    gl->glDrawArrays(GL_LINE_LOOP, 0, 4);
//    gl->glDrawArrays(GL_TRIANGLES, 0, 3);


//    /* OPTIONAL: Deactivate last active shader program */
//    ShaderProgram::unbind();

//    /* Disable arrays, if left enabled OpenGL will try to read from arrays which may be bound to a invalid pointer */
//    /* For instance, if another draw call uses only 1 array intead of 2 for the diamond draw call */
//    gl->glDisableVertexAttribArray(standardShader.vertexIndex);
//    gl->glDisableVertexAttribArray(standardShader.colorIndex);

//    /* Cleanup */
//    gl->glDeleteBuffers(2, vbo);

//    /*
//    ** </EXPERIMENTAL DRAW>
//    */


    standardShader.bind();
    gl->glUniformMatrix4fv(ShaderProgram::activeShaderProgram->viewMatrixIndex, 1, GL_FALSE, static_cast<Camera*>(activeCamera->components[Component::CAMERA])->ci.ptr());
    //TODO Store light direction in light component
    Vector3 lightDirection = Vector3(0, 0, 1);
    Matrix4 rotation;
    rotation.makeEulerRotation(this->activeLight->transform.rotation);
    lightDirection = (rotation * lightDirection.toVector4(0.0f)).toVector3().normalize();
    gl->glUniform3fv(ShaderProgram::activeShaderProgram->lightDirectionIndex, 1, lightDirection.ptr());
    gl->glUniform4fv(ShaderProgram::activeShaderProgram->lightColorIndex, 1, static_cast<Light*>(activeLight->components[Component::LIGHT])->color.ptr());


    for (SceneObject* sceneObject : sceneObjects)
        if (sceneObject->enabled) {
            sceneObject->draw(static_cast<Camera*>(activeCamera->components[Component::CAMERA])->pci);
        }

    ShaderProgram::unbind();
}


void Scene::update()
{
    for (SceneObject* sceneObject : sceneObjects)
        if (sceneObject->enabled) sceneObject->update();
}

void Scene::setAspectRatio(float a)
{
    if (this->activeCamera)
        static_cast<Camera*>(activeCamera->components[Component::CAMERA])->setAspectRatio(a);
}
