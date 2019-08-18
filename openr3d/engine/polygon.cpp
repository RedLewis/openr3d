#include <iostream>
#include "polygon.h"
#include "physics2d.h"
#include "opengl.h"

static const float EPSILON=0.0000000001f;

float Triangulate::Area(const std::vector<Vector2> &contour)
{

  int n = contour.size();

  float A=0.0f;

  for(int p=n-1,q=0; q<n; p=q++)
    {
      A+= contour[p].x*contour[q].y - contour[q].x*contour[p].y;
    }
  return A*0.5f;
}

/*
     InsideTriangle decides if a point P is Inside of the triangle
     defined by A, B, C.
*/
bool Triangulate::InsideTriangle(float Ax, float Ay,
                                 float Bx, float By,
                                 float Cx, float Cy,
                                 float Px, float Py)

{
  float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
  float cCROSSap, bCROSScp, aCROSSbp;

  ax = Cx - Bx;  ay = Cy - By;
  bx = Ax - Cx;  by = Ay - Cy;
  cx = Bx - Ax;  cy = By - Ay;
  apx= Px - Ax;  apy= Py - Ay;
  bpx= Px - Bx;  bpy= Py - By;
  cpx= Px - Cx;  cpy= Py - Cy;

  aCROSSbp = ax*bpy - ay*bpx;
  cCROSSap = cx*apy - cy*apx;
  bCROSScp = bx*cpy - by*cpx;

  return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
};

bool Triangulate::Snip(const std::vector<Vector2> &contour,int u,int v,int w,int n,int *V)
{
  int p;
  float Ax, Ay, Bx, By, Cx, Cy, Px, Py;

  Ax = contour[V[u]].x;
  Ay = contour[V[u]].y;

  Bx = contour[V[v]].x;
  By = contour[V[v]].y;

  Cx = contour[V[w]].x;
  Cy = contour[V[w]].y;

  if ( EPSILON > (((Bx-Ax)*(Cy-Ay)) - ((By-Ay)*(Cx-Ax))) ) return false;

  for (p=0;p<n;p++)
    {
      if( (p == u) || (p == v) || (p == w) ) continue;
      Px = contour[V[p]].x;
      Py = contour[V[p]].y;
      if (InsideTriangle(Ax,Ay,Bx,By,Cx,Cy,Px,Py)) return false;
    }

  return true;
}

bool Triangulate::Process(const std::vector<Vector2> &contour,std::vector<Vector2> &result)
{
  /* allocate and initialize list of Vertices in polygon */

  int n = contour.size();
  if ( n < 3 ) return false;

  int *V = new int[n];

  /* we want a counter-clockwise polygon in V */

  if ( 0.0f < Area(contour) )
    for (int v=0; v<n; v++) V[v] = v;
  else
    for(int v=0; v<n; v++) V[v] = (n-1)-v;

  int nv = n;

  /*  remove nv-2 Vertices, creating 1 triangle every time */
  int count = 2*nv;   /* error detection */

  for(int m=0, v=nv-1; nv>2; )
    {
      /* if we loop, it is probably a non-simple polygon */
      if (0 >= (count--))
        {
          //** Triangulate: ERROR - probable bad polygon!
          delete V;
          std::cerr << "Triangulate: ERROR - probable bad polygon!" << std::endl;
          return false;
        }

      /* three consecutive vertices in current polygon, <u,v,w> */
      int u = v  ; if (nv <= u) u = 0;     /* previous */
      v = u+1; if (nv <= v) v = 0;     /* new v    */
      int w = v+1; if (nv <= w) w = 0;     /* next     */

      if ( Snip(contour,u,v,w,nv,V) )
        {
          int a,b,c,s,t;

          /* true names of the vertices */
          a = V[u]; b = V[v]; c = V[w];

          /* output Triangle */
          //Pushed in different order to conserve orientation
          result.push_back( contour[c] );
          result.push_back( contour[a] );
          result.push_back( contour[b] );

          m++;

          /* remove v from remaining polygon */
          for(s=v,t=v+1;t<nv;s++,t++) V[s] = V[t]; nv--;

          /* resest error detection counter */
          count = 2*nv;
        }
    }



  delete[] V;

  return true;
}



R_Polygon::R_Polygon()
    : Asset(Asset::Type::POLYGON)
{
}

R_Polygon::R_Polygon(const std::vector<Vector2>& outerPoints)
    : Asset(Asset::Type::POLYGON)
{
    load(outerPoints);
}

R_Polygon::~R_Polygon()
{
}

//Can be optimised using only dot product and transpose to deduce if > 180 degree
bool isReflex(Vector2 c, Vector2 v1, Vector2 v2) {
    v1 -= c;
    v2 -= c;
    float dot = v1.x * v2.x + v1.y * v2.y;
    float det = v1.x * v2.y - v2.x * v1.y;
    float angle = atan2f(det, dot);
    return angle < 0;
}

bool inTriangle(Vector2 p, Vector2 v1, Vector2 v2, Vector2 v3) {
    //the coordinates of p can be expressed as a linear combination of p1, p2 and p3
    //using 3 scalars p can be defined as
    //p.x = a * v1.x + b * v2.x + c * v3.x
    //p.y = a * v1.y + b * v2.y + c * v3.y
    //p.z = a * v1.z + b * v2.z + c * v3.z
    //with a + b + c = 1
    float a = ((v2.y - v3.y)*(p.x - v3.x) + (v3.x - v2.x)*(p.y - v3.y)) / ((v2.y - v3.y)*(v1.x - v3.x) + (v3.x - v2.x)*(v1.y - v3.y));
    float b = ((v3.y - v1.y)*(p.x - v3.x) + (v1.x - v3.x)*(p.y - v3.y)) / ((v2.y - v3.y)*(v1.x - v3.x) + (v3.x - v2.x)*(v1.y - v3.y));
    float c = 1 - a - b;
    return (0 < a && a < 1) &&
           (0 < b && b < 1) &&
           (0 < c && c < 1);
}

//TODO: Check if aligned points work && Add support for superposed points and crossing edges
int R_Polygon::load(const std::vector<Vector2>& outerPoints)
{
    /* Verify Data
    */
    if (outerPoints.size() < 3) {
        std::cerr << "Polygon::load()\tInvalid polygon: less than 3 points." << std::endl;
        return -1;
    }
    innerRings.clear();

    /* Correct point order and store points
    */
    {
        //If points are clockwise reverse to counter clockwise
        outerRing.resize(outerPoints.size());
        //Compute polygon area using shoelace formula without the absolute sign
        //If area positive the points are ordered clockwise and vice versa
        float sum = 0;
        unsigned int j = outerPoints.size() - 1;
        for (unsigned int i = 0; i < outerPoints.size(); ++i) {
            sum += (outerPoints[i].x * outerPoints[j].y) - (outerPoints[j].x * outerPoints[i].y);
            j = i;
        }
        //Clockwise (needs to be reversed)
        if (sum > 0) {
#ifdef DEBUG
            std::cerr << "int Polygon::load()\tWarning: Points ordered clockwise. Automatically reordered counterclockwise." << std::endl;
#endif
            unsigned int i = 0;
            for (auto it = outerPoints.rbegin(); it != outerPoints.rend(); ++it) {
                outerRing[i].x = it->x;
                outerRing[i].y = it->y;
                ++i;
            }
        }
        //Counterclockwise (no reverse needed)
        else {
            unsigned int i = 0;
            for (auto it = outerPoints.begin(); it != outerPoints.end(); ++it) {
                outerRing[i].x = it->x;
                outerRing[i].y = it->y;
                ++i;
            }
        }
    }

    convexDecomposition();

    return 0;
}

void R_Polygon::convexDecomposition()
{
    std::vector<Vector2> result;
    Triangulate::Process(outerRing, result);
    int tcount = result.size()/3;
    for (int i=0; i<tcount; i++)
      {
        convexFaces.emplace_back();
        ConvexFace& convexFace = convexFaces.back();
        convexFace.resize(3);
        convexFace[0] = result[i*3+0];
        convexFace[1] = result[i*3+1];
        convexFace[2] = result[i*3+2];
        convexFace.update();
      }
}

std::vector<R_Polygon>& R_Polygon::substract(const R_Polygon& other, std::vector<R_Polygon>& results) const
{
    boost::geometry::difference(*this, other, results);
    return results;
}

std::vector<R_Polygon>& R_Polygon::merge(const R_Polygon& other, std::vector<R_Polygon>& results) const
{
    boost::geometry::union_(*this, other, results);
    return results;
}

std::vector<R_Polygon>& R_Polygon::intersect(const R_Polygon& other, std::vector<R_Polygon>& results) const
{
    boost::geometry::intersection(*this, other, results);
    return results;
}


#include "vector3.h"
#include "shaderprogram.h"
void R_Polygon::draw() const
{
    //TODO: Polygons have no normal data therefor light must be inactive for them to be visible
    // Set Shader Settings
    GLint useLightIndexValue;
    gl->glGetUniformiv(ShaderProgram::activeShaderProgram->program,
                       ShaderProgram::activeShaderProgram->useLightIndex,
                       &useLightIndexValue);
    if (useLightIndexValue != 0)
        gl->glUniform1i(ShaderProgram::activeShaderProgram->useLightIndex, 0);

    for (const R_Polygon::ConvexFace& convexFace : convexFaces)
        convexFace.draw();

    // Reset Shader Settings
    if (useLightIndexValue != 0)
        gl->glUniform1i(ShaderProgram::activeShaderProgram->useLightIndex, useLightIndexValue);
}

R_Polygon::ConvexFace::ConvexFace() {
    //Create VBOs
    gl->glGenBuffers(1, &(this->verticesVBO));
    gl->glGenBuffers(1, &(this->textureCoordinatesVBO));
}

R_Polygon::ConvexFace::~ConvexFace() {
    //Delete VBOs
    gl->glDeleteBuffers(1, &(this->verticesVBO));
    gl->glDeleteBuffers(1, &(this->textureCoordinatesVBO));
}

void R_Polygon::ConvexFace::update() {
    //Bind VBO as being the active buffer
    gl->glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
    //Copy the vertex data to the active buffer
    gl->glBufferData(GL_ARRAY_BUFFER, this->size() * sizeof(Vector2), this->data(), GL_STATIC_DRAW);

    //TODO: The texture coordinates are only valid if polygon is in boundry of -0.5 to 0.5.
    //Should be made dynamic by finding the ranges on x and on y.
    //OR TO REMOVE: find another way of texturing polygon
    std::vector<Vector2> textureCoordinates(*this);
    //Transform from range -0.5 to 0.5 to a range of 0 to 1
    for (Vector2& v : textureCoordinates) { v.x += 0.5f; v.y += 0.5f; }
    //Bind VBO as being the active buffer
    gl->glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVBO);
    //Copy the vertex data to the active buffer
    gl->glBufferData(GL_ARRAY_BUFFER, this->size() * sizeof(Vector2), textureCoordinates.data(), GL_STATIC_DRAW);

    //OPTIONAL: Unbind VBO
    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void R_Polygon::ConvexFace::draw() const {

    //Bind VBO as being the active buffer
    gl->glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
    // Specify that our coordinate data is going into attribute index, and contains two floats per vertex
    gl->glVertexAttribPointer(ShaderProgram::activeShaderProgram->vertexIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (GLvoid*)0);

    //Bind VBO as being the active buffer
    gl->glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVBO);
    // Specify that our coordinate data is going into attribute index, and contains two floats per vertex
    gl->glVertexAttribPointer(ShaderProgram::activeShaderProgram->textureCoordinateIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (GLvoid*)0);

    //OPTIONAL: Unbind VBO
    gl->glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Enable attribute index as being used
    gl->glEnableVertexAttribArray(ShaderProgram::activeShaderProgram->vertexIndex);
    gl->glEnableVertexAttribArray(ShaderProgram::activeShaderProgram->textureCoordinateIndex);

    // Invoke glDrawArrays telling that our data are triangles and we want to draw all the vertexes
    gl->glDrawArrays(GL_TRIANGLE_FAN, 0, this->size());

    // Disable attribute index as being used
    gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->vertexIndex);
    gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->textureCoordinateIndex);

}
