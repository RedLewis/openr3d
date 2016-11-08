varying mediump vec3 out_normal;
varying lowp vec3 out_color;

void main(void)
{
    gl_FragColor = vec4(out_normal, 1.0);
    //gl_FragColor = vec4(out_color, 1.0);
}
