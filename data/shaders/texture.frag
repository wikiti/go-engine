/* FRAG */

uniform sampler2D texture;
uniform float textureFlag;

void main(void)
{
  gl_FragColor = texture2D(texture, frag_TexCoords);
}
