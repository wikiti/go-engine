/* FRAG */

uniform sampler2D texture;
uniform float textureFlag;

varying vec3 frag_normal;
varying vec3 frag_vertex_to_light_vector;

varying vec4 frag_Color;
varying vec2 frag_TexCoords;

void main(void)
{
    // Defining The Material Colors
    const vec4 AmbientColor = vec4(0.1, 0.0, 0.0, 1.0);
    const vec4 DiffuseColor = vec4(1.0, 0.0, 0.0, 1.0);
 
    // Scaling The Input Vector To Length 1
    vec3 normalized_normal = normalize(frag_normal);
    vec3 normalized_vertex_to_light_vector = normalize(frag_vertex_to_light_vector);
 
    // Calculating The Diffuse Term And Clamping It To [0;1]
    float DiffuseTerm = clamp(dot(frag_normal, frag_vertex_to_light_vector), 0.0, 1.0);
 
    // Calculating The Final Color
    gl_FragColor = AmbientColor + DiffuseColor * DiffuseTerm;

    //gl_FragColor = frag_Color;
    //gl_FragColor = texture2D(texture, frag_TexCoords).rgba * frag_Color;
    //mix(frag_Color, texture2D(texture, frag_TexCoords) * frag_Color, textureFlag);
}
