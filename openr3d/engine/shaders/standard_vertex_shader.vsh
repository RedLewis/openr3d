uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec3 lightDirection;
uniform vec3 lightColor;

attribute vec3 in_vertex;
attribute vec3 in_normal;
attribute vec2 in_textureCoordinate;
attribute vec4 in_vertexColor;

varying vec3 out_normal;
varying vec2 out_textureCoordinate;
varying vec4 out_vertexColor;

void main(void)
{
    out_normal = normalize((normalMatrix * vec4(in_normal, 0.0)).xyz);
    out_textureCoordinate = in_textureCoordinate;
    out_vertexColor = in_vertexColor;
    gl_Position = modelViewProjectionMatrix * vec4(in_vertex, 1.0);
}
