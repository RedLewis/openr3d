#include "polygoncollider2d.h"
#include "sceneobject.h"
#include "scene.h"
#include "physics2d.h"

void PolygonCollider2D::init()
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
#ifdef DEBUG
    /*
    for (Polygon* polygon : polygons)
        polygon->draw();
        */
#endif
}

void PolygonCollider2D::updateCollider()
{

    // Definition of the shape
    //TODO: Need to subdivide points in multiple fixtures and shapes:
    // - so that none of the shapes are concave
    // - to overcome the 8 points shape limit

    //Delete existing fixtures
    /*b2Fixture* fixture = body->GetFixtureList();
    b2Fixture* nextFixture;
    while (fixture != NULL) {
        nextFixture = fixture->GetNext();
        body->DestroyFixture(fixture);
        fixture = nextFixture;
    }
    //Clear polygon-fixtures association map
    polygonfixtureLists.clear();*/


    //TOREMOVE (and keep the block above): delete all fixtures that are in map (avoid deleting manually added fixtures)
    for (std::pair<Polygon*, std::list<b2Fixture*>> polygonFixtures: polygonfixtureLists) {
        for (b2Fixture* polygonFixture : polygonFixtures.second) {
            b2Fixture* fixture = body->GetFixtureList();
            b2Fixture* nextFixture;
            while (fixture != NULL) {
                nextFixture = fixture->GetNext();
                if (fixture == polygonFixture)
                    body->DestroyFixture(fixture);
                fixture = nextFixture;
            }
        }
    }
#ifdef DEBUG
    if (body->GetFixtureList() != NULL)
        std::cerr << "PolygonCollider2D::updateCollider()\tWarning: unkown fixtures detected." << std::endl;
#endif
    //Clear polygon-fixtures association map
    polygonfixtureLists.clear();


    //Create new fixtures
    for (Polygon* polygon : polygons) {
        addPolygonFixtures(polygon);
    }
}

void PolygonCollider2D::addPolygonFixtures(Polygon* polygon)
{
    //Create new fixtures
    std::list<b2Fixture*>& polygonfixtureList = polygonfixtureLists[polygon];

    std::vector<b2Vec2> tmpPoints(polygon->outerRing.size());
    for (unsigned int i = 0; i < polygon->outerRing.size(); ++i) {
        tmpPoints[i].x = polygon->outerRing[i].x * this->sceneObject->transform.getWorldScale().x * BOX2DSCALE;
        tmpPoints[i].y = polygon->outerRing[i].y * this->sceneObject->transform.getWorldScale().y * BOX2DSCALE;
    }
    b2Polygon concaveShape(tmpPoints.data(), tmpPoints.size());
    b2Polygon convexShapes[16];
    int convexShapeNbr = DecomposeConvex(&concaveShape, convexShapes, 16);
    if (convexShapeNbr > 16) {
        std::cerr << "PolygonCollider2D::addPolygonFixtures()\tWarning: Polygon too complex, convex decomposition incomplete." << std::endl;
        convexShapeNbr = 16;
    }

    //TODO: REPLACE THIS DEBUG CODE (allow the terrain to draw using per convex face color)
    //This code replaces the classic polygon convex decomposition method that must be called after having updated the polygon
    {
        polygon->convexFaces.clear();
    }

    for (int i = 0; i < convexShapeNbr; ++i) {

        //TODO: REPLACE THIS DEBUG CODE (allow the terrain to draw using per convex face color)
        //This code replaces the classic polygon convex decomposition method that must be called after having updated the polygon
        {
            polygon->convexFaces.emplace_back();
            Polygon::ConvexFace& convexFace = polygon->convexFaces.back();
            convexFace.resize(convexShapes[i].nVertices);
            for (int j = 0; j < convexShapes[i].nVertices; ++j) {
                convexFace[j].x = convexShapes[i].x[j] / (this->sceneObject->transform.getWorldScale().x * BOX2DSCALE);
                convexFace[j].y = convexShapes[i].y[j] / (this->sceneObject->transform.getWorldScale().y * BOX2DSCALE);
            }
            convexFace.update();
        }

        if (convexShapes[i].IsUsable() == false) {
            //std::cerr << "PolygonCollider2D::addPolygonFixtures()\tWarning: Unsable polygon resulted from convex decomposition." << std::endl;
            continue;
        }
        if (convexShapes[i].GetArea() < 0.2f) {
            //std::cerr << "PolygonCollider2D::addPolygonFixtures()\tWarning: Polygon convex fragment area too small: ignored." << std::endl;
            continue;
        }

        convexShapes[i].AddTo(fixtureDef);
        polygonfixtureList.push_back(body->CreateFixture(&fixtureDef));
        //The fixtureDef.shape is allocated by b2Polygon.AddTo(fixtureDef) and no longer needed after it is passed to CreateFixture
        delete fixtureDef.shape;
    }
}

std::list<Polygon*>::const_iterator PolygonCollider2D::addPolygon(Polygon* polygon)
{
    auto polygonIt = polygons.insert(polygons.end(), polygon);
    addPolygonFixtures(polygon);
    return polygonIt;
}

const std::list<Polygon*>& PolygonCollider2D::getPolygons() const
{
    return polygons;
}

void PolygonCollider2D::removePolygon(std::list<Polygon*>::const_iterator polygonIterator)
{
    auto polygonfixtureListIterator = polygonfixtureLists.find(*polygonIterator);
    for (b2Fixture* fixture : polygonfixtureListIterator->second)
        body->DestroyFixture(fixture);
    polygonfixtureLists.erase(polygonfixtureListIterator);
    polygons.erase(polygonIterator);
}

void PolygonCollider2D::removePolygon(Polygon* polygon)
{
    auto polygonfixtureListIterator = polygonfixtureLists.find(polygon);
    for (b2Fixture* fixture : polygonfixtureListIterator->second)
        body->DestroyFixture(fixture);
    polygonfixtureLists.erase(polygonfixtureListIterator);
    polygons.remove(polygon);
}









