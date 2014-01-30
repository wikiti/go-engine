/* VERT */

uniform mat4 ProjMatrix;
uniform mat4 ModelViewMatrix;

//uniform vec3 in_Translation;

void main(void)
{
    //mat4 TranslatedModelViewMatrix = ModelViewMatrix;
	//TranslatedModelViewMatrix[3] = ModelViewMatrix[0] * in_Translation[0] + ModelViewMatrix[1] * in_Translation[1] + ModelViewMatrix[2] * in_Translation[2] + ModelViewMatrix[3];

    mat4 MVPMatrix = ProjMatrix * ModelViewMatrix;
    gl_Position = MVPMatrix * gl_Vertex; /* <- deprecated!! */
}

