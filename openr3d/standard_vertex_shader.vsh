uniform highp mat4 modelMatrix;
uniform highp mat4 viewMatrix;
uniform highp mat4 normalMatrix;
uniform highp mat4 modelViewProjectionMatrix;
uniform mediump vec3 lightDirection;

attribute highp vec3 in_vertex;
attribute mediump vec3 in_normal;
attribute lowp vec3 in_color;

varying mediump vec3 out_normal;
varying lowp vec3 out_color;

void main(void)
{
    out_normal = normalize((normalMatrix * vec4(in_normal, 0.0)).xyz);
    gl_Position = modelViewProjectionMatrix * vec4(in_vertex, 1.0);
}
