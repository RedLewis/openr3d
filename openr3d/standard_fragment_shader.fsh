uniform sampler2D textureSampler;
uniform int useTexture;

uniform highp mat4 modelMatrix;
uniform highp mat4 viewMatrix;
uniform highp mat4 normalMatrix;
uniform highp mat4 modelViewProjectionMatrix;
uniform mediump vec3 lightDirection;
uniform lowp vec4 lightColor;

varying mediump vec3 out_normal;
varying highp vec2 out_textureCoordinate;

void main(void)
{
    mediump float lightIntensity = max(dot(lightDirection, out_normal), 0.0);
    gl_FragColor = lightColor * lightIntensity;
    if (useTexture != 0) {
        //gl_FragColor.r = out_textureCoordinate.x;
        //gl_FragColor.g = out_textureCoordinate.y;
        gl_FragColor =  lightColor * lightIntensity * texture2D(textureSampler, out_textureCoordinate);
    }
    else {
        gl_FragColor = lightColor * lightIntensity;
    }
}
