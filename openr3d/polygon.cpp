#include "polygon.h"
#include "physics2d.h"
#include "opengl.h"
#include "shaderprogram.h"

Polygon::Polygon()
    : Asset(Asset::Type::POLYGON)
{
}

Polygon::Polygon(const std::vector<Vector3>& paramPoints)
    : Asset(Asset::Type::POLYGON)
{
    load(paramPoints);
}

Polygon::~Polygon()
{
}

//Can be optimised using only dot product and transpose to deduce if > 180 degree
bool isReflex(b2Vec2 c, b2Vec2 v1, b2Vec2 v2) {
    v1 -= c;
    v2 -= c;
    float dot = v1.x * v2.x + v1.y * v2.y;
    float det = v1.x * v2.y - v2.x * v1.y;
    float angle = atan2f(det, dot);
    return angle < 0;
}

bool inTriangle(b2Vec2 p, b2Vec2 v1, b2Vec2 v2, b2Vec2 v3) {
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
int Polygon::load(const std::vector<Vector3>& paramPoints)
{
    /* Correct point order and store points
    */
    {
        //If points are clockwise reverse to counter clockwise
        points.resize(paramPoints.size());
        //Compute polygon area using shoelace formula without the absolute sign
        //If area positive the points are ordered clockwise and vice versa
        float sum = 0;
        unsigned int j = paramPoints.size() - 1;
        for (unsigned int i = 0; i < paramPoints.size(); ++i) {
            sum += (paramPoints[i].x * paramPoints[j].y) - (paramPoints[j].x * paramPoints[i].y);
            j = i;
        }
        //Clockwise (needs to be reversed)
        if (sum > 0) {
            std::cerr << "int Polygon::load(const std::vector<Vector3>&)\tWarning: Points ordered clockwise. Automatically reordered counterclockwise." << std::endl;
            unsigned int i = 0;
            for (auto it = paramPoints.rbegin(); it != paramPoints.rend(); ++it) {
                points[i].x = it->x;
                points[i].y = it->y;
                ++i;
            }
        }
        //Counterclockwise (no reverse needed)
        else {
            unsigned int i = 0;
            for (auto it = paramPoints.begin(); it != paramPoints.end(); ++it) {
                points[i].x = it->x;
                points[i].y = it->y;
                ++i;
            }
        }
    }


    /* Split points into convex faces
    */
    {
        std::vector<b2Vec2> tmpPoints(points);

        while (tmpPoints.size() >= 3) {
            convexFaces.emplace_back();
            ConvexFace& convexFace = convexFaces.back();

            //Find a triangle (hopefully the first one)
            for (auto it = tmpPoints.begin(); it != tmpPoints.end();) {
                b2Vec2& prevPoint = (it == tmpPoints.begin()) ? tmpPoints.back() : *(std::prev(it));
                b2Vec2& currPoint = *it;
                b2Vec2& nextPoint = (it == (--tmpPoints.end())) ? tmpPoints.front() : *(std::next(it));

                //Check if the origin of the triangle is reflex
                if (!isReflex(currPoint, nextPoint, prevPoint)) {
                    //Check if triangle contain another polygon point
                    bool containsPoint = false;
                    for (b2Vec2& otherPoint : tmpPoints) {
                        if (&otherPoint == &prevPoint || &otherPoint == &currPoint || &otherPoint == &nextPoint) continue;
                        if (inTriangle(otherPoint, prevPoint, currPoint, nextPoint)) { containsPoint = true; break; }
                    }
                    //Triangle is valid
                    if (!containsPoint) {
                        convexFace.points.resize(3);
                        convexFace.points[0] = prevPoint;
                        convexFace.points[1] = currPoint;
                        convexFace.points[2] = nextPoint;
                        tmpPoints.erase(it++);
                        break;
                    }
                    //Triangle is invalid
                    else
                        ++it;
                }
                //Triangle is invalid
                else
                    ++it;
            }
            //Found a triangle
        }
    }


    return 0;
}

void Polygon::draw() const
{
    for (const Polygon::ConvexFace& cf : convexFaces) {
        std::vector<Vector3> colors(cf.points.size());

        for (Vector3& color : colors) {
            color.x = ((((size_t)&cf) >> 0) % 256) / 255.0f;
            color.y = ((((size_t)&cf) >> 8) % 256) / 255.0f;
            color.z = ((((size_t)&cf) >> 16) % 256) / 255.0f;
        }

        GLuint vbo_vertex, vbo_colors;

        gl->glGenBuffers(1, &vbo_vertex);
        gl->glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
        gl->glBufferData(GL_ARRAY_BUFFER, cf.points.size() * sizeof(b2Vec2), cf.points.data(), GL_STATIC_DRAW);
        gl->glVertexAttribPointer(ShaderProgram::activeShaderProgram->vertexIndex, 2, GL_FLOAT, GL_FALSE, sizeof(b2Vec2), (GLvoid*)0);
        gl->glEnableVertexAttribArray(ShaderProgram::activeShaderProgram->vertexIndex);

        gl->glGenBuffers(1, &vbo_colors);
        gl->glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
        gl->glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Vector3), colors.data(), GL_STATIC_DRAW);
        gl->glVertexAttribPointer(ShaderProgram::activeShaderProgram->vertexColorIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (GLvoid*)0);
        gl->glEnableVertexAttribArray(ShaderProgram::activeShaderProgram->vertexColorIndex);

        gl->glBindBuffer(GL_ARRAY_BUFFER, 0);

        gl->glUniform1i(ShaderProgram::activeShaderProgram->useLightIndex, 0);
        gl->glUniform1i(ShaderProgram::activeShaderProgram->useVertexColorIndex, 1);
        gl->glDrawArrays(GL_TRIANGLE_FAN, 0, cf.points.size());
        gl->glUniform1i(ShaderProgram::activeShaderProgram->useVertexColorIndex, 0);
        gl->glUniform1i(ShaderProgram::activeShaderProgram->useLightIndex, 1);

        gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->vertexIndex);
        gl->glDisableVertexAttribArray(ShaderProgram::activeShaderProgram->vertexColorIndex);
        gl->glDeleteBuffers(1, &vbo_vertex);
        gl->glDeleteBuffers(1, &vbo_colors);
    }
}
