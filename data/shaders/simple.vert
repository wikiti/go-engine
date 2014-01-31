/* VERT */

uniform mat4 ProjMatrix;
uniform mat4 ModelViewMatrix;

attribute vec3 in_Position;
attribute vec3 in_Color;
varying vec3 frag_Color;

void main(void)
{
    //mat4 TranslatedModelViewMatrix = ModelViewMatrix;
	//TranslatedModelViewMatrix[3] = ModelViewMatrix[0] * in_Translation[0] + ModelViewMatrix[1] * in_Translation[1] + ModelViewMatrix[2] * in_Translation[2] + ModelViewMatrix[3];

    mat4 MVPMatrix = ProjMatrix * ModelViewMatrix;
    gl_Position = MVPMatrix * vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
    
    frag_Color = in_Color;
}

