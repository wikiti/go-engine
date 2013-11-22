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
  if(!enabled) return;

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
  if(!enabled) return;

  glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture("mesh1_texture")->GetID());
  glColor3f(1.f, 1.f, 1.f);

  CResource_Mesh* mesh = gSystem_Resources.GetMesh("mesh1");
  if(mesh) mesh->Render();
}



vector3f direction;
float angle;

void CComponent_Dummy3::OnRender()
{
  vector3f pos = gameObject->transform()->position;
  glBindTexture(GL_TEXTURE_2D, 0);

  glColor3f(1.f, 0.5f, 0.f); // Naranja
  glBegin(GL_LINES);
    glVertex3f(pos.x, pos.y, pos.z);
    glVertex3f(pos.x + direction.x, pos.y + direction.y, pos.z + direction.z);
  glEnd();

  glColor3f(1.f, 1.f, 0.f); // Amarillo
  glBegin(GL_LINES);
    glVertex3f(pos.x, pos.y, pos.z);
    glVertex3f(pos.x + current_random.x, pos.y + current_random.y, pos.z + current_random.z);
  glEnd();

  glColor3f(1.f, 0.f, 1.f); // Rosa
  glBegin(GL_LINES);
    glVertex3f(pos.x, pos.y, pos.z);
    glVertex3f(pos.x + another_random.x, pos.y + another_random.y, pos.z + another_random.z);
  glEnd();

  glColor3f(0.f, 1.f, 0.f); // Verde
  for(uint i = 0; i < __RANDOM_ARRAY_SIZE; i++)
  {
    glBegin(GL_LINES);
      glVertex3f(pos.x, pos.y, pos.z);
      glVertex3f(pos.x + random_array[i].x, pos.y + random_array[i].y, pos.z + random_array[i].z);
    glEnd();

  }
}

void CComponent_Dummy3::OnEvent()
{
  if(event.type == SDL_KEYDOWN)
  {
    if(event.key.keysym.sym == SDLK_F5)
    {
      current_random = gMath.random_vector(direction, angle);
      //cout << "New random vector around cone: " << current_random << endl;
    }
    else if(event.key.keysym.sym == SDLK_F6)
    {
      another_random = gMath.random_vector();
      //cout << "New random vector: " << another_random << endl;
    }
    else if(event.key.keysym.sym == SDLK_F7)
    {
      random_array.resize(__RANDOM_ARRAY_SIZE);
      for(uint i = 0; i < __RANDOM_ARRAY_SIZE; i++)
      {
        random_array[i] = gMath.random_vector(direction, angle);
        //cout << random_array[i].length() << endl;
      }
    }
    else if(event.key.keysym.sym == SDLK_PLUS)
    {
      angle += 15;
      gMath.NormalizeAngle(angle);
    }
    else if(event.key.keysym.sym == SDLK_MINUS)
    {
      angle -= 15;
      gMath.NormalizeAngle(angle);
    }
  }
}
