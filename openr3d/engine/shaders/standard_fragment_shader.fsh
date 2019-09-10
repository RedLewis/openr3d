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
uniform mediump vec3 viewPosition;

varying highp vec3 out_vertex;
varying mediump vec3 out_normal;
varying mediump vec3 out_tangent;
varying mediump vec3 out_bitangent;
varying highp vec2 out_textureCoordinate;
varying lowp vec4 out_vertexColor;

void main(void)
{
    //SURFACE NORMAL
    mediump vec3 surfaceNormal;
    if (useNormalMap != 0) {
        //TODO: Use TBN matrix in vertex shader and make fragment light calculations in TBN space for better performance!
        //Rasterization of out_normal, out_tangent and out_bitangent makes them non unit vectors, therefore we must normalize them:
        mediump mat3 TBN = mat3(normalize(out_tangent), normalize(out_bitangent), normalize(out_normal));
        //Get texel representing surface normal and transform from range [0, 1] (used for colors) to [-1, 1] (used for vectors)
        surfaceNormal = texture2D(normalMapSampler, out_textureCoordinate).rgb * 2.0 - 1.0;
        //Move the normal from the texture space to world space using the TBN rotation matrix and normalize the vector (the color representing the normal might not be a unit vector)
        surfaceNormal = normalize(TBN * surfaceNormal);
    }
    else {
        //Rasterization of out_normal makes it a non unit vector, therefore we must normalize it:
        surfaceNormal = normalize(out_normal);
    }

    if (useLight != 0) {
        //AMBIENT COMPONENT
        lowp float ambientIntensity = 0.05;
        mediump vec3 ambientComponent = ambientIntensity * lightColor;

        //DIFFUSE COMPONENT: Calculate light hitting the texel
        lowp float diffuseStrength = 0.5;
        mediump float lightIntensity = max(dot(lightDirection, surfaceNormal), 0.0);
        mediump vec3 diffuseComponent = diffuseStrength * lightIntensity * lightColor;

        //SPECULAR COMPONENT
        lowp float specularStrength = 0.5;
        lowp float shininess = 32.0;
        mediump vec3 reflectionDirection = reflect(-lightDirection, surfaceNormal);
        mediump vec3 viewDirection = normalize(viewPosition - out_vertex);
        float specularIntensity = pow(max(dot(viewDirection, reflectionDirection), 0.0), shininess);
        vec3 specularComponent = specularStrength * specularIntensity * lightColor;

        gl_FragColor = vec4(ambientComponent + diffuseComponent + specularComponent, 1.0);
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
