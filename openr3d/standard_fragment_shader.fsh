uniform highp mat4 modelMatrix;
uniform highp mat4 viewMatrix;
uniform highp mat4 normalMatrix;
uniform highp mat4 modelViewProjectionMatrix;
uniform mediump vec3 lightDirection;

varying mediump vec3 out_normal;
varying lowp vec3 out_color;

void main(void)
{
    mediump float lightIntensity = max(dot(lightDirection, out_normal), 0.0);
    lowp vec4 color = vec4(0.0, 1.0, 0.0, 1.0) * lightIntensity;
    gl_FragColor = color;
}
