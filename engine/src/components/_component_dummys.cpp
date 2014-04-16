#include "_systems.h"
#include "components/_component_dummys.h"
#include "components/_component_transform.h"

using namespace std;

vector3f direction;
float angle;

void CComponent_Dummy::OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix)
{
  glBindTexture(GL_TEXTURE_2D, /*gResources.GetTexture("crate1")->GetID()*/ 0);
  glUseProgram(0);

  glColor3f(1.f, 1.f, 1.f);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(glm::value_ptr(projMatrix));

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(glm::value_ptr(modelViewMatrix));

  glColor3f(   1.0,  1.0, 1.0 );

  // White side - BACK
  glBegin(GL_POLYGON);
  glVertex3f(  0.5, -0.5, 0.5 );
  glVertex3f(  0.5,  0.5, 0.5 );
  glVertex3f( -0.5,  0.5, 0.5 );
  glVertex3f( -0.5, -0.5, 0.5 );
  glEnd();

  // Purple side - RIGHT
  glBegin(GL_POLYGON);
  glVertex3f( 0.5, -0.5, -0.5 );
  glVertex3f( 0.5,  0.5, -0.5 );
  glVertex3f( 0.5,  0.5,  0.5 );
  glVertex3f( 0.5, -0.5,  0.5 );
  glEnd();

  // Green side - LEFT
  glBegin(GL_POLYGON);
  glVertex3f( -0.5, -0.5,  0.5 );
  glVertex3f( -0.5,  0.5,  0.5 );
  glVertex3f( -0.5,  0.5, -0.5 );
  glVertex3f( -0.5, -0.5, -0.5 );
  glEnd();

  // Blue side - TOP
  glBegin(GL_POLYGON);
  glVertex3f(  0.5,  0.5,  0.5 );
  glVertex3f(  0.5,  0.5, -0.5 );
  glVertex3f( -0.5,  0.5, -0.5 );
  glVertex3f( -0.5,  0.5,  0.5 );
  glEnd();

  // Red side - BOTTOM
  glBegin(GL_POLYGON);
  glVertex3f(  0.5, -0.5, -0.5 );
  glVertex3f(  0.5, -0.5,  0.5 );
  glVertex3f( -0.5, -0.5,  0.5 );
  glVertex3f( -0.5, -0.5, -0.5 );
  glEnd();
}

void CComponent_Dummy::OnEvent()
{
  // ...
}
