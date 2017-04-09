#ifndef POLYGON_H
#define POLYGON_H

#include "asset.h"
#include "vector2.h"
#include <vector>
#include <list>
#include <Box2D/Box2D.h>
#include <boost/geometry.hpp>
#include "opengl.h"



//triangulator from http://www.flipcode.com/archives/Efficient_Polygon_Triangulation.shtml
class Triangulate
{
public:

  // triangulate a contour/polygon, places results in STL vector
  // as series of triangles.
  static bool Process(const std::vector<Vector2> &contour,
                      std::vector<Vector2> &result);

  // compute area of a contour/polygon
  static float Area(const std::vector<Vector2> &contour);

  // decide if point Px/Py is inside triangle defined by
  // (Ax,Ay) (Bx,By) (Cx,Cy)
  static bool InsideTriangle(float Ax, float Ay,
                             float Bx, float By,
                             float Cx, float Cy,
                             float Px, float Py);


private:
  static bool Snip(const std::vector<Vector2> &contour,int u,int v,int w,int n,int *V);

};

//TODO: Create a Ring type.
//Otherwise other other boost concept of std::vector<Vector2> will conflict this one.
//TODO: Inner rings not supported. If polygon substractino creates inner rings, they will be ignored.
//TODO: Implement texture coordinates

class Polygon : public Asset
{

public:

    typedef std::vector<Vector2> Ring;
    //Outer ring must be counterclockwise
    Ring outerRing;
    //Inner order is opposite of outer (therefore clockwise)
    std::vector<Ring> innerRings;

    class ConvexFace : public std::vector<Vector2>
    {
        GLuint verticesVBO;
        GLuint textureCoordinatesVBO;

    public:
        ConvexFace();
        ~ConvexFace();
        //Upload data to gpu, call after convexFace modifications
        void update();
        void draw() const;
    };

    std::list<ConvexFace> convexFaces;

    Polygon();
    //TODO: Take generic container of points
    Polygon(const std::vector<Vector2>& outerPoints);
    ~Polygon();

    //TODO: Take generic container of points
    int load(const std::vector<Vector2>& outerPoints);
    //TODO: Polygon decomposition unstable
    void convexDecomposition();

    std::vector<Polygon>& substract(const Polygon& other, std::vector<Polygon>& results) const;
    std::vector<Polygon>& merge(const Polygon& other, std::vector<Polygon>& results) const;
    std::vector<Polygon>& intersect(const Polygon& other, std::vector<Polygon>& results) const;

    void draw() const;
    //TODO: ability to edit inner and outer rings and use an update function to update gpu data (same as Mesh)
    //void update();


};



/*
** Adaption of Polygon::Ring to Boost.Geometry
*/

//TODO: Create a innerRing with clockwise point order
//Polygon Concept: http://www.boost.org/doc/libs/1_58_0/libs/geometry/doc/html/geometry/reference/concepts/concept_polygon.html
//Ring Concept: http://www.boost.org/doc/libs/1_58_0/libs/geometry/doc/html/geometry/reference/concepts/concept_ring.html
namespace boost {
    namespace geometry {
        namespace traits {

            template<> struct tag<Polygon::Ring>
            { typedef ring_tag type; };

            template<> struct point_order<Polygon::Ring>
            { static const order_selector value = counterclockwise; };

            template<> struct closure<Polygon::Ring>
            { static const closure_selector value = open; };

        }
    }
}

/*
** Boost.Range for Polygon::Ring
** Useless right now. Adapt this code when Ring becomes it's own class so it can be used by boost.
*/

namespace boost {

    template <> struct range_iterator<Polygon::Ring>
    { typedef std::vector<Vector2>::iterator type; };

    template<> struct range_const_iterator<Polygon::Ring>
    { typedef std::vector<Vector2>::const_iterator type; };

}

inline std::vector<Vector2>::iterator range_begin(Polygon::Ring& r)
{ return r.begin(); }

inline std::vector<Vector2>::const_iterator range_begin(const Polygon::Ring& r)
{ return r.begin(); }

inline std::vector<Vector2>::iterator range_end(Polygon::Ring& r)
{ return r.end(); }

inline std::vector<Vector2>::const_iterator range_end(const Polygon::Ring& r)
{ return r.end(); }

/*
** Adaption of Polygon to Boost.Geometry
*/

namespace boost {
    namespace geometry {
        namespace traits {

            template<> struct tag<Polygon>
            { typedef polygon_tag type; };
            template<> struct ring_const_type<Polygon>
            { typedef const Polygon::Ring& type; };
            template<> struct ring_mutable_type<Polygon>
            { typedef Polygon::Ring& type; };
            template<> struct interior_const_type<Polygon>
            { typedef const std::vector<Polygon::Ring>& type; };
            template<> struct interior_mutable_type<Polygon>
            { typedef std::vector<Polygon::Ring>& type; };

            template<> struct exterior_ring<Polygon> {
                static Polygon::Ring& get(Polygon& p)
                { return p.outerRing; }
                static Polygon::Ring const& get(Polygon const& p)
                { return p.outerRing; }
            };

            template<> struct interior_rings<Polygon> {
                static std::vector<Polygon::Ring>& get(Polygon& p)
                { return p.innerRings; }
                static const std::vector<Polygon::Ring>& get(Polygon const& p)
                { return p.innerRings; }
            };

        }
    }
}


#endif // POLYGON_H
