#include "components/_component_gui.h"

#include "systems/_resource.h"
#include "systems/_render.h"

CComponent_GUI_Texture::CComponent_GUI_Texture(CGameObject* gameObject): CComponent(gameObject)
{
  texture_name = "";

  pixel_offset_x = pixel_offset_y = 0;
  width = height = 1.f;

  color(1.f, 1.f, 1.f, 1.f);
}

CComponent_GUI_Texture::~CComponent_GUI_Texture()
{

}

bool CComponent_GUI_Texture::HitTest(GLfloat x, GLfloat y)
{
  int w, h;
  gSystem_Render.GetWindowSize(&w, &h);

  x /= w;
  y /= h;

  if(x >= 0.f + (float)pixel_offset_x/w and x <= width + (float)pixel_offset_x/w and y <= 0.f + (float)pixel_offset_y/h and  y >= -height + (float)pixel_offset_y/h)
    return true;

  return false;
}

void CComponent_GUI_Texture::OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix)
{
  if(!enabled) return;

  vector3f position = gameObject->Transform()->position;
  vector3f scale    = gameObject->Transform()->scale;
  //float rotation_z  = gameObject->Transform()->EulerAngles().z;

  glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture(texture_name)->GetID());
  glColor4f(color.r, color.g, color.b, color.a);

  glTranslatef(position.x, position.y, position.z);
  //glRotatef(rotation_z, 0.f, 0.f, 1.f);
  glMultMatrixf((const float*)glm::value_ptr(glm::toMat4(gameObject->Transform()->angle)));
  glScalef(scale.x, scale.y, scale.z);

  int w, h;
  gSystem_Render.GetWindowSize(&w, &h);

  glBegin(GL_QUADS);
    glTexCoord2d(0,1); glVertex3f(-width/2 + (float)pixel_offset_x/w,  height/2 + (float)pixel_offset_y/h, 0.f);
    glTexCoord2d(1,1); glVertex3f( width/2 + (float)pixel_offset_x/w,  height/2 + (float)pixel_offset_y/h, 0.f);
    glTexCoord2d(1,0); glVertex3f( width/2 + (float)pixel_offset_x/w, -height/2 + (float)pixel_offset_y/h, 0.f);
    glTexCoord2d(0,0); glVertex3f(-width/2 + (float)pixel_offset_x/w, -height/2 + (float)pixel_offset_y/h, 0.f);
  glEnd();
}

