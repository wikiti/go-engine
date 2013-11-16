#include "_systems.h"
#include "components/_component_dummys.h"
#include "components/_component_transform.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Dummy1);

CComponent_Dummy1::CComponent_Dummy1(CGameObject* go): CComponent(go)
{
  cube_side = 0.1f;
}

void CComponent_Dummy1::OnRender()
{
  glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture("textura1")->GetID());
  glColor3f(1.f, 1.f, 1.f);

  glBegin(GL_QUADS);
    // Delante
    glNormal3f( 0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.f, 0.0f); glVertex3f(-cube_side, -cube_side,  cube_side);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( cube_side, -cube_side,  cube_side);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.f); glVertex3f( cube_side,  cube_side,  cube_side);  // Top Right Of The Texture and Quad
    glTexCoord2f(1.f, 1.f); glVertex3f(-cube_side,  cube_side,  cube_side);  // Top Left Of The Texture and Quad
    // Detrás
    glNormal3f( 0.0f, 0.0f,-1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-cube_side, -cube_side, -cube_side);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-cube_side,  cube_side, -cube_side);  // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( cube_side,  cube_side, -cube_side);  // Top Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( cube_side, -cube_side, -cube_side);  // Bottom Left Of The Texture and Quad
    // Arriba
    glNormal3f( 0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-cube_side,  cube_side, -cube_side);  // Top Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-cube_side,  cube_side,  cube_side);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( cube_side,  cube_side,  cube_side);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( cube_side,  cube_side, -cube_side);  // Top Right Of The Texture and Quad
    // Abajo
    glNormal3f( 0.0f,-1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-cube_side, -cube_side, -cube_side);  // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( cube_side, -cube_side, -cube_side);  // Top Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( cube_side, -cube_side,  cube_side);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-cube_side, -cube_side,  cube_side);  // Bottom Right Of The Texture and Quad
    // Derecha
    glNormal3f( 1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( cube_side, -cube_side, -cube_side);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( cube_side,  cube_side, -cube_side);  // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( cube_side,  cube_side,  cube_side);  // Top Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( cube_side, -cube_side,  cube_side);  // Bottom Left Of The Texture and Quad
    // Izquierda
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-cube_side, -cube_side, -cube_side);  // Bottom Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-cube_side, -cube_side,  cube_side);  // Bottom Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-cube_side,  cube_side,  cube_side);  // Top Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-cube_side,  cube_side, -cube_side);  // Top Left Of The Texture and Quad
  glEnd();
}

void CComponent_Dummy2::OnRender()
{
  glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture("mesh1_texture")->GetID());
  glColor3f(1.f, 1.f, 1.f);

  CResource_Mesh* mesh = gSystem_Resources.GetMesh("mesh1");
  if(mesh) mesh->Render();
}
