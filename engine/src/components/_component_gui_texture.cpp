#include "components/_component_gui_texture.h"

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

void CComponent_GUI_Texture::OnRender()
{
  if(!enabled) return;

  vector3f position = gameObject->transform()->position;
  vector3f scale    = gameObject->transform()->scale;

  glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture(texture_name)->GetID());
  glColor4f(color.r, color.g, color.b, color.a);

  glTranslatef(position.x, position.y, position.z);
  glScalef(scale.x, scale.y, scale.z);

  int w, h;
  gSystem_Render.GetWindowSize(&w, &h);

  glBegin(GL_QUADS);
    glTexCoord2d(0,1); glVertex3f(0.f   + (float)pixel_offset_x/w,  0.f    + (float)pixel_offset_y/h, 0.f);
    glTexCoord2d(1,1); glVertex3f(width + (float)pixel_offset_x/w,  0.f    + (float)pixel_offset_y/h, 0.f);
    glTexCoord2d(1,0); glVertex3f(width + (float)pixel_offset_x/w, -height + (float)pixel_offset_y/h, 0.f);
    glTexCoord2d(0,0); glVertex3f(0.f   + (float)pixel_offset_x/w, -height + (float)pixel_offset_y/h, 0.f);
  glEnd();
}

