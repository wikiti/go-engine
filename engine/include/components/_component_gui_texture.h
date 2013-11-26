#ifndef __COMPONENT_GUI_TEXTURE_H_
#define __COMPONENT_GUI_TEXTURE_H_

#include "_globals.h"
#include "components/_component.h"

class CComponent_GUI_Texture: public CComponent
{
  private:
    friend class CSystem_Render;
    friend class CGameObject;

  public:
    string texture_name;

    GLuint pixel_offset_x, pixel_offset_y;
    GLfloat width, height;

    colorf_t color;

  private:
    static int GetID() { return components::gui_texture; }

  public:
    CComponent_GUI_Texture(){};
    CComponent_GUI_Texture(CGameObject* gameObject);
    ~CComponent_GUI_Texture();

    inline virtual void Set(input_t data);
    inline virtual output_t Get();

    bool HitTest(GLfloat pixel_x, GLfloat pixel_y);
    void OnRender();
};

#endif /* __COMPONENT_GUI_TEXTURE_H_ */
