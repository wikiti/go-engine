/* VERT */

uniform mat4 ProjMatrix;
uniform mat4 ModelViewMatrix;

attribute vec4 in_Position;
attribute vec4 in_Color;
attribute vec2 in_TexCoords;

varying vec4 frag_Color;
varying vec2 frag_TexCoords;

void main(void)
{
    //mat4 TranslatedModelViewMatrix = ModelViewMatrix;
	//TranslatedModelViewMatrix[3] = ModelViewMatrix[0] * in_Translation[0] + ModelViewMatrix[1] * in_Translation[1] + ModelViewMatrix[2] * in_Translation[2] + ModelViewMatrix[3];

    mat4 MVPMatrix = ProjMatrix * ModelViewMatrix;
    gl_Position = MVPMatrix * in_Position;
    
    frag_Color = in_Color;
    frag_TexCoords = in_TexCoords;
}

