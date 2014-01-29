/* VERT */

void main(void)
{
    mat4 MVPMatrix = ProjMatrix * ModelViewMatrix;
    gl_Position = MVPMatrix * gl_Vertex; /* <- deprecated!! */
}

