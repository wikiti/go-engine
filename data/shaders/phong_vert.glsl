/* VERT */

uniform mat4 ProjMatrix;
uniform mat4 ModelViewMatrix;

attribute vec4 in_Position;
attribute vec4 in_Color;
attribute vec2 in_TexCoords;

varying vec4 frag_Color;
varying vec2 frag_TexCoords;

varying vec3 frag_normal;
varying vec3 frag_vertex_to_light_vector;

void main(void)
{
    gl_Position = ProjMatrix * ModelViewMatrix * in_Position;
 
    // Transforming The Normal To ModelView-Space
    frag_normal = gl_NormalMatrix * gl_Normal;
 
    // Transforming The Vertex Position To ModelView-Space
    vec4 vertex_in_modelview_space = ModelViewMatrix * in_Position;
 
    // Calculating The Vector From The Vertex Position To The Light Position
    frag_vertex_to_light_vector = vec3(vec3(0, 0, 0) * vertex_in_modelview_space);
    
    frag_Color = in_Color;
    frag_TexCoords = in_TexCoords;
}

