#include "glwidget.h"
#include "utilities.h"
#include <cmath>

#include "light.h"
#include "meshrenderer.h"
#include "shaderprogram.h"
#include "boxcollider2d.h"
#include "circlecollider2d.h"
#include "polygoncollider2d.h"
#include "edgecollider2d.h"

void GLWidget::createScene() {
    this->scene = new Scene(this->width(), this->height());

    //TODO: Functionalize this process (the creation linking of the sceneObject and the component)

    //Setup Camera
    SceneObject* cameraObject = new SceneObject(scene);
    cameraObject->name = "camera";
    cameraObject->transform.setLocalPosition({0, 0.0f, 3.0f});
    cameraObject->transform.setLocalRotation({-0.2f, 0, 0});
    Camera* cameraComponent = new Camera(cameraObject);
    cameraComponent->setOrthographic(false);
    cameraComponent->setFOV(60);

    //Setup Light
    SceneObject* lightObject = new SceneObject(scene); //Rotate light with camera by having the light object a child of centerofrotation
    lightObject->transform.setLocalRotation({-0.4f, -0.4f, 0});
    Light* lightComponent = new Light(lightObject, Light::Type::DIRECTIONAL);
    lightComponent->color = {1.0, 1.0, 1.0};

    Mesh* mesh1 = new Mesh("../assets/sphere.obj");
    Mesh* mesh2 = new Mesh("../assets/cube.obj");
    Texture* texture1 = new Texture("../assets/texture.ppm");
    Texture* texture2 = new Texture("../assets/earth.ppm");
    scene->assets.push_back(mesh1);
    scene->assets.push_back(mesh2);
    scene->assets.push_back(texture1);
    scene->assets.push_back(texture2);

    {

        SceneObject* parent = new SceneObject(scene);
        parent->name = "scale";
        parent->transform.setLocalPosition({0.0f, 0.5f, 0.0f});
        parent->transform.setLocalScale({0.25f, 0.25f, 0.25f});
        parent->transform.setLocalRotation({0.0f, 0.0f, 0.25f});
        MeshRenderer* parentMeshRenderer = new MeshRenderer(parent);
        parentMeshRenderer->mesh = mesh2;
        parentMeshRenderer->texture = texture2;
        new BoxCollider2D(parent, Collider::DYNAMIC);

        {
            SceneObject* cube = new SceneObject(parent);
            cube->transform.setLocalPosition({0.425f, 0.425f, 0.0f});
            cube->transform.setLocalScale({0.25f, 0.25f, 0.25f});
            cube->transform.setLocalRotation({0.0f, 0.0f, 0.25f});
            MeshRenderer* cubeMeshRenderer = new MeshRenderer(cube);
            cubeMeshRenderer->mesh = mesh2;
            cubeMeshRenderer->texture = texture1;
            new BoxCollider2D(cube, Collider::DYNAMIC);
        }
        {
            SceneObject* cube = new SceneObject(parent);
            cube->transform.setLocalPosition({0.400f, -0.400f, 0.0f});
            cube->transform.setLocalScale({0.25f, 0.25f, 0.25f});
            MeshRenderer* cubeMeshRenderer = new MeshRenderer(cube);
            cubeMeshRenderer->mesh = mesh2;
            cubeMeshRenderer->texture = texture1;
            new BoxCollider2D(cube, Collider::DYNAMIC);

        }
        {
            SceneObject* cube = new SceneObject(parent);
            cube->transform.setLocalPosition({-0.400f, 0.425f, 0.0f});
            cube->transform.setLocalScale({0.25f, 0.25f, 0.25f});
            MeshRenderer* cubeMeshRenderer = new MeshRenderer(cube);
            cubeMeshRenderer->mesh = mesh2;
            cubeMeshRenderer->texture = texture1;
            new BoxCollider2D(cube, Collider::DYNAMIC);
        }
        {
            SceneObject* cube = new SceneObject(parent);
            cube->transform.setLocalPosition({-0.425f, -0.400f, 0.0f});
            cube->transform.setLocalScale({0.25f, 0.25f, 0.25f});
            MeshRenderer* cubeMeshRenderer = new MeshRenderer(cube);
            cubeMeshRenderer->mesh = mesh2;
            cubeMeshRenderer->texture = texture1;
            new BoxCollider2D(cube, Collider::DYNAMIC);
        }
    }
    {
        SceneObject* terrain = new SceneObject(scene);
        terrain->name = "terrain";
        terrain->transform.setLocalScale({3.0f, 1.0f, 1.0f});
        terrain->transform.setLocalPosition({0.0f, -1.25f, 0.0f});
        MeshRenderer* terrainMeshRenderer = new MeshRenderer(terrain);
        terrainMeshRenderer->mesh = mesh2;
        terrainMeshRenderer->texture = texture1;
        new BoxCollider2D(terrain, Collider::STATIC);
    }
    {
        SceneObject* sphere = new SceneObject(scene);
        sphere->name = "sphere";
        sphere->transform.setLocalScale({1.0f, 1.0f, 1.0f});
        sphere->transform.setLocalPosition({1.0f, -0.25f, 0.0f});
        MeshRenderer* sphereMeshRenderer = new MeshRenderer(sphere);
        sphereMeshRenderer->mesh = mesh1;
        sphereMeshRenderer->texture = texture2;
        new CircleCollider2D(sphere, Collider::STATIC);
    }

    scene->activeCamera = cameraObject;
    scene->activeLight = lightObject;

    this->controlledCameraPtr = scene->activeCamera;

    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>> SCENE CREATED <<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
}

void GLWidget::deleteScene() {
    delete scene;
    controlledCameraPtr = nullptr;
    scene = nullptr;
    std::cout << ">>>>>>>>>>>>>>>>>>>>>>>> SCENE DELETED <<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
}

GLWidget::GLWidget(float framesPerSecond, QWidget *parent)
    : QOpenGLWidget(parent)
{
    /*
    ** Specify Surface Format
    */

    QSurfaceFormat format;

    //QSurfaceFormat::DefaultRenderableType	: The default unspecified rendering method
    //QSurfaceFormat::OpenGL : Desktop OpenGL rendering
    //QSurfaceFormat::OpenGLES : OpenGL ES 2.0 rendering
    format.setRenderableType(QSurfaceFormat::OpenGLES);
    format.setMajorVersion(2);
    format.setMinorVersion(0);
    //format.setRenderableType(QSurfaceFormat::OpenGL);
    //format.setMajorVersion(3);
    //format.setMinorVersion(1);

    //QSurfaceFormat::CompatibilityProfile : Functionality from earlier OpenGL versions is available.
    //QSurfaceFormat::CoreProfile : Functionality deprecated in OpenGL version 3.0 is not available.
    format.setProfile(QSurfaceFormat::CoreProfile);
    //Used to request that deprecated functions be included in the OpenGL context profile. If not specified, you should get a forward compatible context without support functionality marked as deprecated. This requires OpenGL version 3.0 or higher.
    format.setOption(QSurfaceFormat::DeprecatedFunctions, true);
    //Used to request a debug context with extra debugging information.
    format.setOption(QSurfaceFormat::DebugContext, false);

    //Set the preferred number of samples per pixel when multisampliQng is enabled
    format.setSamples(4);

    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setDepthBufferSize(24);

    this->setFormat(format);

    QObject::connect(&timer, SIGNAL(tick(float)), this, SLOT(update(float)));
    this->timer.start(framesPerSecond);
}

GLWidget::~GLWidget()
{
    if (scene)
        deleteScene();
}

void GLWidget::initializeGL()
{
    /*
    ** OpenGL rendering context is made current when paintGL(), resizeGL(), or initializeGL() is called.
    ** From this context an already initialized, ready-to-be-used QOpenGLFunctions instance is retrievable by calling QOpenGLContext::functions().
    */
    //Make OpenGL Context Functions available
    gl = new GL(*this->context());

    std::cout << "Program compiled for " << sizeof(size_t) * 8 << "bit systems." << std::endl;
    gl->printInfo();
    gl->configure();

    this->createScene();
}

void GLWidget::resizeGL(int width, int height)
{
    if (scene) {
        scene->screen.width = width;
        scene->screen.height = height;
    }
}

void GLWidget::paintGL()
{
    //The scene cannot be created or destroyed outside the render thread (initializeGL, resizeGL and paintGL) as the OpenGL context and version might differ or not be present
    //Therefore we destroy and rebuilt the scene in paintGL.
    if (resetScene) {
        resetScene = false;
        if (scene)
            deleteScene();
        createScene();
    }

    //Clear off-screen buffer
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Draw Scene
    if (this->scene)
        this->scene->draw();

    //Swap off-screen buffer with on-screen buffer
    //...
}



void GLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
        case Qt::Key_R:
            resetScene = true;
            break;
        case Qt::Key_W:
            if (controlCamera) cameraMoveForward = true;
            break;
        case Qt::Key_A:
            if (controlCamera) cameraMoveLeft = true;
            break;
        case Qt::Key_S:
            if (controlCamera) cameraMoveBackward = true;
            break;
        case Qt::Key_D:
            if (controlCamera) cameraMoveRight = true;
            break;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
        case Qt::Key_W:
            if (controlCamera) cameraMoveForward = false;
            break;
        case Qt::Key_A:
            if (controlCamera) cameraMoveLeft = false;
            break;
        case Qt::Key_S:
            if (controlCamera) cameraMoveBackward = false;
            break;
        case Qt::Key_D:
            if (controlCamera) cameraMoveRight = false;
            break;
    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    switch(event->button())
    {
        case Qt::LeftButton:
            //Enable/Disable camera control
            controlCamera = !controlCamera;
            {
                QCursor cursor = this->cursor();
                if (controlCamera) {
                    this->setMouseTracking(true);
                    cursor.setShape(Qt::BlankCursor);
                    //Move mouse to center
                    QPoint widgetCenter(this->width()/2, this->height()/2);
                    cursor.setPos(mapToGlobal(widgetCenter));
                    cameraMouseDeltaX = 0;
                    cameraMouseDeltaY = 0;
                } else {
                    this->setMouseTracking(false);
                    cursor.setShape(Qt::ArrowCursor);
                    cameraMoveForward = false;
                    cameraMoveBackward = false;
                    cameraMoveLeft = false;
                    cameraMoveRight = false;
                }
                this->setCursor(cursor);
            }
            break;
        default:
            break;
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (controlCamera) {
        //Record mouse movement delta to use for camera rotation
        QPoint widgetCenter(this->width()/2, this->height()/2);
        int newCameraMouseDeltaX = event->x() - widgetCenter.x();
        int newCameraMouseDeltaY = event->y() - widgetCenter.y();
        //If mouse events are faster than GLWidget::update, the largest mouse mouvement is taken into account
        if (fabs(newCameraMouseDeltaX) > fabs(cameraMouseDeltaX))
            cameraMouseDeltaX = newCameraMouseDeltaX;
        if (fabs(newCameraMouseDeltaY) > fabs(cameraMouseDeltaY))
            cameraMouseDeltaY = newCameraMouseDeltaY;
        //Recenter mouse
        QCursor cursor = this->cursor();
        cursor.setPos(mapToGlobal(widgetCenter));
        this->setCursor(cursor);
    }
}

void GLWidget::update(float deltaTime)
{
    //Update camera position based on user input
    if (controlCamera)
        moveCamera(deltaTime);

    //Update scene (including matrices, positions, physics etc)
    if (this->scene)
        this->scene->update(deltaTime);

    //Trigger a repaint (paintGL)
    this->QOpenGLWidget::update();

    //Calculate and send new FPS value
    renderedFrames += 1;
    timeSinceLastFrameRateUpdate += deltaTime;
    if (timeSinceLastFrameRateUpdate > frameRateUpdateFrequency) {
        emit frameRateUpdate(std::roundf(renderedFrames / timeSinceLastFrameRateUpdate));
        timeSinceLastFrameRateUpdate = 0.0f;
        renderedFrames = 0;
    }
}

void GLWidget::moveCamera(float deltaTime)
{
    if (!controlledCameraPtr)
        return;

    //Translate camera
    {
        float distance = 3.f * deltaTime;
        if (cameraMoveForward) {
            Vector3 forwardVector = controlledCameraPtr->transform.getLocalToWorldMatrix() * Vector3(0, 0, -1) * distance;
            controlledCameraPtr->transform.setWorldPosition(controlledCameraPtr->transform.getWorldPosition() + forwardVector);
        }
        if (cameraMoveBackward) {
            Vector3 forwardVector = controlledCameraPtr->transform.getLocalToWorldMatrix() * Vector3(0, 0, 1) * distance;
            controlledCameraPtr->transform.setWorldPosition(controlledCameraPtr->transform.getWorldPosition() + forwardVector);
        }
        if (cameraMoveLeft) {
            Vector3 forwardVector = controlledCameraPtr->transform.getLocalToWorldMatrix() * Vector3(-1, 0, 0) * distance;
            controlledCameraPtr->transform.setWorldPosition(controlledCameraPtr->transform.getWorldPosition() + forwardVector);
        }
        if (cameraMoveRight) {
            Vector3 forwardVector = controlledCameraPtr->transform.getLocalToWorldMatrix() * Vector3(1, 0, 0) * distance;
            controlledCameraPtr->transform.setWorldPosition(controlledCameraPtr->transform.getWorldPosition() + forwardVector);
        }
    }
    //Rotate camera
    {
        Vector3 rotationVector = controlledCameraPtr->transform.getWorldRotation();
        float rotationSpeed = 0.3f * deltaTime;
        //Rotate around y axis (left and right)
        rotationVector.y -= rotationSpeed * cameraMouseDeltaX;
        if (rotationVector.y < 0.f)
            rotationVector.y += 2.f*M_PI;
        if (rotationVector.y >= 2.f*M_PI)
            rotationVector.y -= 2.f*M_PI;
        //Rotate around x axis (up and down)
        rotationVector.x = wrapFloat(rotationVector.x, -M_PI, M_PI); //Wrap rotationVector.x to the range of -M_PI to M_PI (for example the range of 0 to 2*M_PI)
        float limitDowm = -M_PI/2.f + 0.2f;
        float limitUp = M_PI/2.f - 0.2f;
        rotationVector.x -= rotationSpeed * cameraMouseDeltaY;
        if (rotationVector.x < limitDowm)
            rotationVector.x = limitDowm;
        if (rotationVector.x > limitUp)
            rotationVector.x = limitUp;
        //Apply rotation
        controlledCameraPtr->transform.setWorldRotation(rotationVector);
        //Clear camera mouse deltas
        cameraMouseDeltaX = 0;
        cameraMouseDeltaY = 0;
    }
}
