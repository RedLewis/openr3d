//OpenGL ES automatically #define GL_ES
#ifndef GL_ES
//Ignore precision specifiers when not using OpenGL ES for desktop OpenGL compatibility
#define lowp
#define mediump
#define highp
#endif

uniform highp mat4 modelMatrix;
uniform highp mat4 viewMatrix;
uniform highp mat4 normalMatrix;
uniform highp mat4 modelViewProjectionMatrix;
uniform mediump vec3 lightDirection;
uniform lowp vec3 lightColor;
uniform mediump vec3 viewPosition;

attribute highp vec3 in_vertex;
attribute mediump vec3 in_normal;
attribute mediump vec3 in_tangent;
attribute mediump vec3 in_bitangent;
attribute highp vec2 in_textureCoordinate;
attribute lowp vec4 in_vertexColor;

varying highp vec3 out_vertex;
varying mediump vec3 out_normal;
varying mediump vec3 out_tangent;
varying mediump vec3 out_bitangent;
varying highp vec2 out_textureCoordinate;
varying lowp vec4 out_vertexColor;

void main(void)
{
    out_vertex = (modelMatrix * vec4(in_vertex, 1.0)).xyz;
    out_normal = (normalMatrix * vec4(in_normal, 0.0)).xyz;
    out_tangent = (normalMatrix * vec4(in_tangent, 0.0)).xyz;
    out_bitangent = (normalMatrix * vec4(in_bitangent, 0.0)).xyz;
    out_textureCoordinate = in_textureCoordinate;
    out_vertexColor = in_vertexColor;
    gl_Position = modelViewProjectionMatrix * vec4(in_vertex, 1.0);
}
