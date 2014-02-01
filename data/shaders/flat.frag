/* FRAG */

uniform sampler2D texture;
uniform float textureFlag;

varying vec4 frag_Color;
varying vec2 frag_TexCoords;

void main(void)
{
  gl_FragColor = frag_Color;
  //gl_FragColor = texture2D(texture, frag_TexCoords).rgba * frag_Color;
  //mix(frag_Color, texture2D(texture, frag_TexCoords) * frag_Color, textureFlag);
}
