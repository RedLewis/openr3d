uniform mat4 modelViewProjectionMatrix;

attribute highp vec3 in_vertex;
attribute mediump vec3 in_normal;
attribute lowp vec3 in_color;

varying mediump vec3 out_normal;
varying lowp vec3 out_color;

void main(void)
{
    out_normal = in_normal;
    out_color = in_color;
    gl_Position = modelViewProjectionMatrix * vec4(in_vertex, 1.0);
}
