#ifndef POLYGONCOLLIDER2D_H
#define POLYGONCOLLIDER2D_H

#include <list>
#include <map>
#include "collider2d.h"
#include "polygon.h"
#include "box2dUtilities/b2Polygon.h"

class PolygonCollider2D : public Collider2D
{

    void init();
    std::list<Polygon*> polygons;
    std::map<Polygon*, std::list<b2Fixture*>> polygonfixtureLists;

public:

    PolygonCollider2D(SceneObject *sceneObject, ColliderType type)
        : Collider2D(sceneObject, type) { init(); }
    PolygonCollider2D(SceneObject *sceneObject, ColliderType type, Polygon* polygon)
        : Collider2D(sceneObject, type), polygons(1, polygon) { init(); }
    template <typename C>
    PolygonCollider2D(SceneObject *sceneObject, ColliderType type, C& polygons)
        : Collider2D(sceneObject, type), polygons(polygons.begin(), polygons.end()) { init(); }

    void draw() const;

    void updateCollider();

    std::list<Polygon*>::const_iterator addPolygon(Polygon* polygon);
    const std::list<Polygon*>& getPolygons() const;
    void removePolygon(std::list<Polygon*>::const_iterator polygonIterator);
    void removePolygon(Polygon* polygon);

private:

    void addPolygonFixtures(Polygon* polygon);

};

#endif // POLYGONCOLLIDER2D_H
