/* FRAG */

uniform sampler2D TextureMap;

varying vec2 out_TexCoord;
varying vec3 out_Normal;

void main(void)
{
    vec4 tex0 = texture2D(TextureMap, out_TexCoord);
    //gl_FragColor = vec4 (tex0.x, tex0.y, tex0.z, tex0.w);
    //vec4 tex =  texture(texture1, pass_TextureCoord);
    gl_FragColor = vec4(tex0.r*(gl_Color[0]),tex0.g*(gl_Color[1]),tex0.b*(gl_Color[2]),tex0.a*(gl_Color[3])) ;
    //gl_FragColor = gl_Color;
}
