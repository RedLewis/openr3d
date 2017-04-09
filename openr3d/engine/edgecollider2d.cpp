#include "edgecollider2d.h"
#include "sceneobject.h"
#include "scene.h"
#include "physics2d.h"
#include "vector2.h"

EdgeCollider2D::EdgeCollider2D(SceneObject *sceneObject, ColliderType type, const std::vector<Vector2>& paramPoints)
    : Collider2D(sceneObject, type)
{
    // Store the points
    points.resize(paramPoints.size());
    for (unsigned int i = 0; i < paramPoints.size(); ++i) {
        points[i].x = paramPoints[i].x;
        points[i].y = paramPoints[i].y;
    }

    // Define body fixture
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.75f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.userData = this;
    updateCollider();
}

void EdgeCollider2D::updateCollider() {

    //Delete existing fixture
    if (body->GetFixtureList() != NULL)
        body->DestroyFixture(body->GetFixtureList());

    //Create new fixture
    std::vector<b2Vec2> tmpPoints(points.size());
    for (unsigned int i = 0; i < tmpPoints.size(); ++i) {
        tmpPoints[i].x = this->points[i].x * this->sceneObject->transform.getWorldScale().x * BOX2DSCALE;
        tmpPoints[i].y = this->points[i].y * this->sceneObject->transform.getWorldScale().y * BOX2DSCALE;
    }
    b2ChainShape shape;
    shape.CreateChain(tmpPoints.data(), tmpPoints.size());
    fixtureDef.shape = &shape;
    body->CreateFixture(&fixtureDef);
}
