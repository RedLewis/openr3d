//OpenGL ES automatically #define GL_ES
#ifndef GL_ES
//Ignore precision specifiers when not using OpenGL ES for desktop OpenGL compatibility
#define lowp
#define mediump
#define highp
#endif

uniform sampler2D textureSampler;
uniform sampler2D normalMapSampler;
uniform int useTexture;
uniform int useNormalMap;
uniform int useVertexColor;
uniform int useLight;
uniform int useLightTwoSide;

uniform highp mat4 modelMatrix;
uniform highp mat4 viewMatrix;
uniform highp mat4 normalMatrix;
uniform highp mat4 modelViewProjectionMatrix;
uniform mediump vec3 lightDirection;
uniform lowp vec3 lightColor;

varying mediump vec3 out_normal;
varying mediump vec3 out_tangent;
varying mediump vec3 out_bitangent;
varying highp vec2 out_textureCoordinate;
varying lowp vec4 out_vertexColor;

void main(void)
{
    mediump vec3 surfaceNormal;

    if (useNormalMap != 0) {
        mediump mat3 TBN = mat3(out_tangent, out_bitangent, out_normal);
        //Get texel representing surface normal and transform from range [0, 1] (used for colors) to [-1, 1] (used for vectors)
        surfaceNormal = texture2D(normalMapSampler, out_textureCoordinate).rgb * 2.0 - 1.0;
        //Move the normal from the texture space to world space using the TBN rotation matrix and normalize the vector (the color representing the normal might not be a unit vector)
        surfaceNormal = normalize(TBN * surfaceNormal);
    }
    else {
        surfaceNormal = out_normal;
    }

    //Calculate light hitting the texel
    if (useLight != 0) {
        mediump float lightIntensity;
        if (useLightTwoSide != 0 && gl_FrontFacing == false)
            lightIntensity = max(dot(lightDirection, surfaceNormal), 0.0);
          else
            lightIntensity = max(dot(lightDirection, surfaceNormal), 0.0);
        gl_FragColor = vec4(lightColor * lightIntensity, 1.0);
    }
    else {
        gl_FragColor = vec4(1, 1, 1, 1);
    }

    //Add color to the texel
    if (useTexture != 0) {
        gl_FragColor *= texture2D(textureSampler, out_textureCoordinate);
    }
    if (useVertexColor != 0) {
        gl_FragColor *= out_vertexColor;
    }
}
