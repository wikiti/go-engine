/* FRAG */

varying vec3 frag_Color;

void main(void)
{
    gl_FragColor = vec4(frag_Color.x, frag_Color.y, frag_Color.z, 1.0);
}
