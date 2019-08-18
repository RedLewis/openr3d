uniform highp mat4 modelMatrix;
uniform highp mat4 viewMatrix;
uniform highp mat4 normalMatrix;
uniform highp mat4 modelViewProjectionMatrix;
uniform highp vec3 lightDirection;
uniform highp vec3 lightColor;

attribute highp vec3 in_vertex;
attribute highp vec3 in_normal;
attribute highp vec2 in_textureCoordinate;
attribute highp vec4 in_vertexColor;

varying highp vec3 out_normal;
varying highp vec2 out_textureCoordinate;
varying highp vec4 out_vertexColor;

void main(void)
{
    out_normal = normalize((normalMatrix * vec4(in_normal, 0.0)).xyz);
    out_textureCoordinate = in_textureCoordinate;
    out_vertexColor = in_vertexColor;
    gl_Position = modelViewProjectionMatrix * vec4(in_vertex, 1.0);
}
