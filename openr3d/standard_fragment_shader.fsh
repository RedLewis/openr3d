uniform highp mat4 modelMatrix;
uniform highp mat4 viewMatrix;
uniform highp mat4 normalMatrix;
uniform highp mat4 modelViewProjectionMatrix;
uniform mediump vec3 lightDirection;
uniform lowp vec4 lightColor;

varying mediump vec3 out_normal;

void main(void)
{
    mediump float lightIntensity = max(dot(lightDirection, out_normal), 0.0);
    lowp vec4 color = lightColor * lightIntensity;
    gl_FragColor = color;
}
