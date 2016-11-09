uniform highp mat4 modelMatrix;
uniform highp mat4 viewMatrix;
uniform highp mat4 normalMatrix;
uniform highp mat4 modelViewProjectionMatrix;
uniform mediump vec3 lightDirection;
uniform lowp vec4 lightColor;

attribute highp vec3 in_vertex;
attribute mediump vec3 in_normal;
attribute highp vec2 in_textureCoordinate;

varying mediump vec3 out_normal;
varying highp vec2 out_textureCoordinate;

void main(void)
{
    out_normal = normalize((normalMatrix * vec4(in_normal, 0.0)).xyz);
    out_textureCoordinate = in_textureCoordinate;
    gl_Position = modelViewProjectionMatrix * vec4(in_vertex, 1.0);
}
