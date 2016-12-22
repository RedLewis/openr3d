#include "polygoncollider2d.h"
#include "sceneobject.h"
#include "scene.h"
#include "physics2d.h"


PolygonCollider2D::PolygonCollider2D(SceneObject *sceneObject, const std::vector<Vector3>& paramPoints, bool isStatic)
    : Collider2D(sceneObject, isStatic), polygon(paramPoints)
{
    // Define body fixture
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.75f;
    fixtureDef.restitution = 0.0f;
    fixtureDef.userData = this;
    updateCollider();
}

void PolygonCollider2D::draw() const
{
    polygon.draw();
}

void PolygonCollider2D::updateCollider() {

    // Definition of the shape
    //TODO: Need to subdivide points in multiple fixtures and shapes:
    // - so that none of the shapes are concave
    // - to overcome the 8 points shape limit

    //Delete existing fixtures
    b2Fixture* fixture = body->GetFixtureList();
    b2Fixture* nextFixture;
    while (fixture != NULL) {
        nextFixture = fixture->GetNext();
        body->DestroyFixture(fixture);
        fixture = nextFixture;
    }

    //Create new fixtures
    for (const Polygon::ConvexFace& cf : polygon.convexFaces) {
        std::vector<b2Vec2> tmpPoints(cf.points);
        for (auto& tmpPoint : tmpPoints) {
            tmpPoint.x *= this->sceneObject->transform.getWorldScale().x * BOX2DSCALE;
            tmpPoint.y *= this->sceneObject->transform.getWorldScale().y * BOX2DSCALE;
        }
        b2PolygonShape shape;
        shape.Set(tmpPoints.data(), tmpPoints.size());
        fixtureDef.shape = &shape;
        body->CreateFixture(&fixtureDef);
    }
}
