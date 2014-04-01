#include "_systems.h"
#include "components/_component_dummys.h"
#include "components/_component_transform.h"

using namespace std;

vector3f direction;
float angle;

void CComponent_Dummy::OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix)
{
  vector3f pos = gameObject->Transform()->position;
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
  for(uint i = 0; i < 1000; i++)
  {
    glBegin(GL_LINES);
      glVertex3f(pos.x, pos.y, pos.z);
      glVertex3f(pos.x + random_array[i].x, pos.y + random_array[i].y, pos.z + random_array[i].z);
    glEnd();

  }
}

void CComponent_Dummy::OnEvent()
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
      random_array.resize(1000);
      for(uint i = 0; i < 1000; i++)
      {
        random_array[i] = gMath.random_vector(direction, angle);
        //cout << random_array[i].length() << endl;
      }
    }
    else if(event.key.keysym.sym == SDLK_PLUS)
    {
      angle += 15;
      gMath.NormalizeAngle(angle);
      cout << "ANGLE:" << angle << endl;
    }
    else if(event.key.keysym.sym == SDLK_MINUS)
    {
      angle -= 15;
      gMath.NormalizeAngle(angle);
      cout << "ANGLE:" << angle << endl;
    }
  }
}
