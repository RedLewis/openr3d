uniform mat4 modelViewProjectionMatrix;

attribute lowp vec3 in_color;
attribute highp vec3 in_vertex;

varying lowp vec3 out_color;

void main(void)
{
    out_color = in_color;
    gl_Position = modelViewProjectionMatrix * vec4(in_vertex, 1.0);
}
