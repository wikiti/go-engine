#ifndef __COMPONENT_CAMERA_H_
#define __COMPONENT_CAMERA_H_

#include "_globals.h"
#include "components/_component.h"

namespace viewmode
{
  enum viewmode_t { perspective = 0, ortho };
}

class CComponent_Camera: public CComponent
{
  private:
    friend class CSystem_Render;
    GLdouble projMatrix[16];

  public:
    bool disable_gui;

    viewmode::viewmode_t viewmode;
    //viewport_t viewport;
    viewportf_t viewport;

    GLfloat field_of_view;
    GLfloat near_clip, far_clip;

    colorf_t background_color;
    bool clear;

    string skybox_texture;

    // Fallo: no apunta correctamente a hijos de padres
    // ¿solución? calcular la nueva posición o quitar esta opción
    CGameObject* target;  // Si hay target, se usará la posición del objetivo como pivote.
    CGameObject* pivot;  // Si no, se usará un pivote en la posición local (0, 0, -1)
    //string target_texture;

    function_t before_render, after_render;

    // Añadir fog, niebla, hdr, cosas por el estilo aquí

    inline void BeforeRender()
    {
      if(before_render) before_render(gameObject);
    }

    inline void AfterRender()
    {
      if(after_render) after_render(gameObject);
    }

  public:
    friend class CGameObject;

  private:
    static int GetID() { return components::camera; }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & boost::serialization::base_object<CComponent>(*this);
      ar & viewmode & viewport & field_of_view & near_clip & far_clip & background_color & clear & target /*& main_camera*/;
    }

  public:
    CComponent_Camera(){};
    CComponent_Camera(CGameObject* gameObject);
    ~CComponent_Camera();

    inline virtual void Set(input_t data);
    inline virtual output_t Get();

	  void SetTarget(CGameObject* t)
	  {
	    target = t;
	  }

	  void UnSetTarget()
	  {
	    SetTarget(NULL);
	  }

  private:
    void ApplyChanges();
    void SetViewport();
    void SetUp();
    void Clear();

    bool DrawSkybox();
};

BOOST_CLASS_EXPORT_KEY( CComponent_Camera );

#endif /* __COMPONENT_CAMERA_H_ */
