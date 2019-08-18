uniform sampler2D textureSampler;
uniform int useNormal;
uniform int useTexture;
uniform int useVertexColor;
uniform int useLight;
uniform int useLightTwoSide;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec3 lightDirection;
uniform vec3 lightColor;

varying vec3 out_normal;
varying vec2 out_textureCoordinate;
varying vec4 out_vertexColor;

void main(void)
{
    //Calculate light hitting the texel
    if (useLight != 0) {
        float lightIntensity;
        if (useLightTwoSide != 0 && gl_FrontFacing == false)
            lightIntensity = max(dot(lightDirection, out_normal), 0.0);
          else
            lightIntensity = max(dot(lightDirection, out_normal), 0.0);
        gl_FragColor = vec4(lightColor * lightIntensity, 1.0);
    }
    else {
        gl_FragColor = vec4(1, 1, 1, 1);
    }

    //Add color to the texel
    if (useTexture != 0) {
        gl_FragColor *=  texture2D(textureSampler, out_textureCoordinate);
    }
    if (useVertexColor != 0) {
        gl_FragColor *= out_vertexColor;
    }
}
