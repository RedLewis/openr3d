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
    std::list<R_Polygon*> polygons;
    std::map<R_Polygon*, std::list<b2Fixture*>> polygonfixtureLists;

public:

    PolygonCollider2D(SceneObject *sceneObject, ColliderType type)
        : Collider2D(sceneObject, type) { init(); }
    PolygonCollider2D(SceneObject *sceneObject, ColliderType type, R_Polygon* polygon)
        : Collider2D(sceneObject, type), polygons(1, polygon) { init(); }
    template <typename C>
    PolygonCollider2D(SceneObject *sceneObject, ColliderType type, C& polygons)
        : Collider2D(sceneObject, type), polygons(polygons.begin(), polygons.end()) { init(); }

    void draw() const;

    void updateCollider();

    std::list<R_Polygon*>::const_iterator addPolygon(R_Polygon* polygon);
    const std::list<R_Polygon*>& getPolygons() const;
    void removePolygon(std::list<R_Polygon*>::const_iterator polygonIterator);
    void removePolygon(R_Polygon* polygon);

private:

    void addPolygonFixtures(R_Polygon* polygon);

};

#endif // POLYGONCOLLIDER2D_H
