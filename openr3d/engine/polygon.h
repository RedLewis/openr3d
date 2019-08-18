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

class R_Polygon : public Asset
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

    R_Polygon();
    //TODO: Take generic container of points
    R_Polygon(const std::vector<Vector2>& outerPoints);
    ~R_Polygon();

    //TODO: Take generic container of points
    int load(const std::vector<Vector2>& outerPoints);
    //TODO: Polygon decomposition unstable
    void convexDecomposition();

    std::vector<R_Polygon>& substract(const R_Polygon& other, std::vector<R_Polygon>& results) const;
    std::vector<R_Polygon>& merge(const R_Polygon& other, std::vector<R_Polygon>& results) const;
    std::vector<R_Polygon>& intersect(const R_Polygon& other, std::vector<R_Polygon>& results) const;

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

            template<> struct tag<R_Polygon::Ring>
            { typedef ring_tag type; };

            template<> struct point_order<R_Polygon::Ring>
            { static const order_selector value = counterclockwise; };

            template<> struct closure<R_Polygon::Ring>
            { static const closure_selector value = open; };

        }
    }
}

/*
** Boost.Range for Polygon::Ring
** Useless right now. Adapt this code when Ring becomes it's own class so it can be used by boost.
*/

namespace boost {

    template <> struct range_iterator<R_Polygon::Ring>
    { typedef std::vector<Vector2>::iterator type; };

    template<> struct range_const_iterator<R_Polygon::Ring>
    { typedef std::vector<Vector2>::const_iterator type; };

}

inline std::vector<Vector2>::iterator range_begin(R_Polygon::Ring& r)
{ return r.begin(); }

inline std::vector<Vector2>::const_iterator range_begin(const R_Polygon::Ring& r)
{ return r.begin(); }

inline std::vector<Vector2>::iterator range_end(R_Polygon::Ring& r)
{ return r.end(); }

inline std::vector<Vector2>::const_iterator range_end(const R_Polygon::Ring& r)
{ return r.end(); }

/*
** Adaption of Polygon to Boost.Geometry
*/

namespace boost {
    namespace geometry {
        namespace traits {

            template<> struct tag<R_Polygon>
            { typedef polygon_tag type; };
            template<> struct ring_const_type<R_Polygon>
            { typedef const R_Polygon::Ring& type; };
            template<> struct ring_mutable_type<R_Polygon>
            { typedef R_Polygon::Ring& type; };
            template<> struct interior_const_type<R_Polygon>
            { typedef const std::vector<R_Polygon::Ring>& type; };
            template<> struct interior_mutable_type<R_Polygon>
            { typedef std::vector<R_Polygon::Ring>& type; };

            template<> struct exterior_ring<R_Polygon> {
                static R_Polygon::Ring& get(R_Polygon& p)
                { return p.outerRing; }
                static R_Polygon::Ring const& get(R_Polygon const& p)
                { return p.outerRing; }
            };

            template<> struct interior_rings<R_Polygon> {
                static std::vector<R_Polygon::Ring>& get(R_Polygon& p)
                { return p.innerRings; }
                static const std::vector<R_Polygon::Ring>& get(R_Polygon const& p)
                { return p.innerRings; }
            };

        }
    }
}


#endif // POLYGON_H
