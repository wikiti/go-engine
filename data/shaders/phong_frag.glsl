/* FRAG */

uniform sampler2D texture;
uniform float textureFlag;

varying vec3 diffuseColor;
varying vec3 specularColor;

varying vec4 frag_Color;
varying vec3 frag_TexCoords;

void main(void)
{
  //gl_FragColor = texture2D(texture, texCoords);
  
  vec2 longitudeLatitude = vec2(
    (atan(frag_TexCoords.y, frag_TexCoords.x) / 3.1415926 + 1.0) * 0.5, 
     1.0 - acos(frag_TexCoords.z) / 3.1415926);
     // unusual processing of texture coordinates
 
  gl_FragColor = vec4(diffuseColor 
    * vec3(texture2D(texture, longitudeLatitude))
    + specularColor, 1.0);
}
