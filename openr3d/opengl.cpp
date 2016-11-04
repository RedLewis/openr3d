#include "opengl.h"
#include <iostream>

void GLPrintContext()
{
    std::cout << "GPU Adapter:\t" << glGetString(GL_RENDERER) << std::endl
        << "OpenGL Version:\t" << glGetString(GL_VERSION) << std::endl
        << "GLSL Version:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl
        << std::endl;
}

void GLConfigure()
{
    //OpenGL Behavior (GL_FASTEST, GL_NICEST, GL_DONT_CARE)
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //quality of color, texture coordinate, and fog coordinate interpolation
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); //sampling quality of antialiased lines
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST); //sampling quality of antialiased polygons
    glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST); //quality and performance of the compressing texture images

    //Polygon
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Either GL_FILL, GL_LINE or GL_POINT

    //Depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); //specifies the conditions under which the pixel will be drawn (default is GL_LESS, pixels with less depth will be drawn first)

    //Culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //Lighting
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH); //Shading model (GL_FLAT or GL_SMOOTH)
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE); //GL_TRUE Enables calculating correct surface normals for backface light
    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); //Specular reflection can be either by view direction (GL_TRUE) or by -z axis (GL_FALSE)
    glEnable(GL_NORMALIZE); //Automatically normalise surface normals

    //Buffers
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Color set in the buffer atfer a glClear
    glClearDepth(1.0f); //Default depth buffer value atfer a glClear
}

//http://nehe.gamedev.net/article/replacement_for_gluperspective/21002/
void GLPerspectiveProjetion(GLdouble fov, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble fW, fH;
    fH = tan( fov / 360 * M_PI ) * zNear;
    fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void GLDrawCube()
{
    glBegin(GL_QUADS);

        //Top
        glNormal3f(0, 1, 0);
        glColor3ub(255,  0,  0); //red
        glVertex3f( 0.5f, 0.5f, 0.5f);
        glVertex3f( 0.5f, 0.5f,-0.5f);
        glVertex3f(-0.5f, 0.5f,-0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);

        //Bottom
        glNormal3f(0, -1, 0);
        glColor3ub(0  ,0  ,255); //blue
        glVertex3f(-0.5f,-0.5f, 0.5f);
        glVertex3f(-0.5f,-0.5f,-0.5f);
        glVertex3f( 0.5f,-0.5f,-0.5f);
        glVertex3f( 0.5f,-0.5f, 0.5f);

        //Left
        glNormal3f(-1, 0, 0);
        glColor3ub(255,255,  0); //yellow
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f,-0.5f);
        glVertex3f(-0.5f,-0.5f,-0.5f);
        glVertex3f(-0.5f,-0.5f, 0.5f);

        //Right
        glNormal3f(1, 0, 0);
        glColor3ub(  0,255,  0); //green
        glVertex3f( 0.5f,-0.5f, 0.5f);
        glVertex3f( 0.5f,-0.5f,-0.5f);
        glVertex3f( 0.5f, 0.5f,-0.5f);
        glVertex3f( 0.5f, 0.5f, 0.5f);

        //Back
        glNormal3f(0, 0, -1);
        glColor3ub(  0,255,255); //cyan
        glVertex3f( 0.5f, 0.5f,-0.5f);
        glVertex3f( 0.5f,-0.5f,-0.5f);
        glVertex3f(-0.5f,-0.5f,-0.5f);
        glVertex3f(-0.5f, 0.5f,-0.5f);

        //Front
        glNormal3f(0, 0, 1);
        glColor3ub(255,  0,255); //pink
        glVertex3f( 0.5f,-0.5f, 0.5f);
        glVertex3f( 0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f,-0.5f, 0.5f);

    glEnd();
}

void GLDrawSphere()
{
    static const float radius = 0.5f;
    static const int numStacks = 100;
    static const int numSides = 100;
    static const int numVerts = (numStacks-1)*numSides;
    static float points[numVerts][3];

    // Vector3 points[sides * (sides-1)];
    float curRadius, curTheta, curRho, deltaTheta, deltaRho, curX,curY,curZ;
    int curStack, curSlice;
    int curVert = 0;
    int t;

    deltaTheta = (float)(2*M_PI) / numSides;
    deltaRho = (float)M_PI / numStacks;

        for (curStack=1; curStack<numStacks; curStack++)
        {
            curRho = (3.141/2.0) - curStack*deltaRho;
            curY = sin(curRho) * radius;
            curRadius = cos(curRho) * radius;
            for (curSlice=0; curSlice<numSides; curSlice++)
            {
                curTheta = curSlice * deltaTheta;
                curX = curRadius * cos(curTheta);
                curZ = -curRadius * sin(curTheta);
                points[curVert][0] = curX;
                points[curVert][1] = curY;
                points[curVert][2] = curZ;
                curVert++;
            }
        }

    // part A - draw the top 'lid' (tris)
    glBegin(GL_TRIANGLE_FAN);
        glNormal3d(0,1,0);
        glVertex3d(0,radius,0);
        for (t=0; t<numSides; t++)
        {
            curX = points[t][0];
            curY = points[t][1];
            curZ = points[t][2];
            glNormal3d(curX, curY, curZ);
            glVertex3d(curX, curY, curZ);
        }
            curX = points[0][0];
            curY = points[0][1];
            curZ = points[0][2];
        glNormal3d(curX, curY, curZ);
        glVertex3d(curX, curY, curZ);
    glEnd();

    // part B - draw the 'sides' (quads)
    int vertIndex;
    for (curStack=0; curStack<numStacks-2; curStack++)
    {
        vertIndex = curStack * numSides;
        glBegin(GL_QUAD_STRIP);
            for (curSlice=0; curSlice<numSides; curSlice++)
            {
                glNormal3d(points[vertIndex+curSlice][0], points[vertIndex+curSlice][1], points[vertIndex+curSlice][2]);
                glVertex3d(points[vertIndex+curSlice][0], points[vertIndex+curSlice][1], points[vertIndex+curSlice][2]);

                glNormal3d(points[vertIndex+numSides+curSlice][0], points[vertIndex+numSides+curSlice][1], points[vertIndex+numSides+curSlice][2]);
                glVertex3d(points[vertIndex+numSides+curSlice][0], points[vertIndex+numSides+curSlice][1], points[vertIndex+numSides+curSlice][2]);
            }
            glNormal3d(points[vertIndex][0], points[vertIndex][1], points[vertIndex][2]);
            glVertex3d(points[vertIndex][0], points[vertIndex][1], points[vertIndex][2]);
            glNormal3d(points[vertIndex+numSides][0], points[vertIndex+numSides][1], points[vertIndex+numSides][2]);
            glVertex3d(points[vertIndex+numSides][0], points[vertIndex+numSides][1], points[vertIndex+numSides][2]);
        glEnd();
    }

    // part C - draw the bottom 'lid' (tris)
    glBegin(GL_TRIANGLE_FAN);
        glNormal3d(0,-1,0);
        glVertex3d(0,-radius,0);
        for (t=0; t<numSides-1; t++)
        {
            curX = points[numVerts-1-t][0];
            curY = points[numVerts-1-t][1];
            curZ = points[numVerts-1-t][2];
            glNormal3d(curX, curY, curZ);
            glVertex3d(curX, curY, curZ);
        }
            curX = points[numVerts-1][0];
            curY = points[numVerts-1][1];
            curZ = points[numVerts-1][2];
        glNormal3d(curX, curY, curZ);
        glVertex3d(curX, curY, curZ);
    glEnd();

}
