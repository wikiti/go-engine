/* VERT */

uniform mat4 ProjMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 NormalMatrix;

attribute vec4 in_Position;
attribute vec4 in_Normal;
attribute vec4 in_Color;
attribute vec3 in_TexCoords;

varying vec4 frag_Color;
varying vec3 frag_TexCoords;

varying vec3 frag_diffuseColor; 
            // the diffuse Phong lighting computed in the vertex shader
varying vec3 frag_specularColor; 
            // the specular Phong lighting computed in the vertex shader
 
void main()
{
  //mat4 NormalMatrix = transpose(inverse(modelView));
  mat4 MVPMatrix = ProjMatrix * ModelViewMatrix;

  vec3 normalDirection =  normalize(NormalMatrix * in_Normal);
  vec3 viewDirection = -normalize(vec3(ModelViewMatrix * in_Position)); 
  vec3 lightDirection;
  float attenuation;
 
  if (0.0 == gl_LightSource[0].position.w) 
  // directional light?
  {
    attenuation = 1.0; // no attenuation
    lightDirection = 
    normalize(vec3(gl_LightSource[0].position));
  } 
  else // point light or spotlight (or other kind of light) 
  {
    vec3 vertexToLightSource = vec3(gl_LightSource[0].position - ModelViewMatrix * in_Position);
    float distance = length(vertexToLightSource);
    attenuation = 1.0 / distance; // linear attenuation 
    lightDirection = normalize(vertexToLightSource);
 
    if (gl_LightSource[0].spotCutoff <= 90.0) // spotlight?
    {
      float clampedCosine = max(0.0, dot(-lightDirection, 
      gl_LightSource[0].spotDirection));
      if (clampedCosine < gl_LightSource[0].spotCosCutoff) 
      // outside of spotlight cone?
      {
        attenuation = 0.0;
      }
      else
      {
        attenuation = attenuation * pow(clampedCosine, 
        gl_LightSource[0].spotExponent);
      }
    }
  }
 
  vec3 ambientLighting = vec3(gl_LightModel.ambient); 
  // without material color!
  vec3 diffuseReflection = attenuation * vec3(gl_LightSource[0].diffuse) * max(0.0, dot(normalDirection, lightDirection)); 
  // without material color!
 
  vec3 specularReflection;
  if (dot(normalDirection, lightDirection) < 0.0) 
  // light source on the wrong side?
  {
    specularReflection = vec3(0.0, 0.0, 0.0); 
    // no specular reflection
  }
  else // light source on the right side
  {
    specularReflection = attenuation * vec3(gl_LightSource[0].specular)  * vec3(gl_FrontMaterial.specular)  * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), gl_FrontMaterial.shininess);
  }
 
  frag_diffuseColor = ambientLighting + diffuseReflection;
  frag_specularColor = specularReflection;
  frag_TexCoords = in_TexCoords; // Cambio
  gl_Position = MVPMatrix * in_Position;
}