#ifndef __COMPONENT_CAMERA_H_
#define __COMPONENT_CAMERA_H_

#include "_globals.h"
#include "components/_component.h"

namespace Viewmode
{
  enum viewmode_t { perspective = 0, ortho, ortho_screen, __viewmode_not_defined };

  extern const char* viewmode_s[];

  const char* viewmode_to_string(viewmode_t c);
  int string_to_viewmode(const string& c);
}

class CComponent_Camera: public CComponent
{
  private:
    friend class CSystem_Render;
    //GLdouble projMatrix[16];
    glm::mat4 projMatrix;
    glm::mat4 modelViewMatrix;

    CGameObject* pivot;  // Si no, se usará un pivote en la posición local (0, 0, -1)

  public:
    // ->PORHACER ¿Qué se supone que hace CComponent_Camera::disable_gui?
    bool disable_gui;

    Viewmode::viewmode_t viewmode;
    //viewport_t viewport;
    viewportf_t viewport;

    GLfloat field_of_view;
    GLfloat near_clip, far_clip;

    colorf_t background_color;
    bool clear;

    string skybox_texture;

    // Fallo: no apunta correctamente a hijos de padres
    // ¿solución? calcular la nueva posición o quitar esta opción
    string target;  // Si hay target, se usará la posición del objetivo como pivote.
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
    static int GetID() { return Components::camera; }

    void parseDebug(string command);
    void printDebug();

    /*friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & boost::serialization::base_object<CComponent>(*this);
      ar & viewmode & viewport & field_of_view & near_clip & far_clip & background_color & clear & target;
    }*/

  public:
    CComponent_Camera(){};
    CComponent_Camera(CGameObject* gameObject);
    ~CComponent_Camera();

    inline virtual void Set(input_t data);
    inline virtual output_t Get();

	  void SetTarget(string t)
	  {
	    target = t;
	  }

    void SetTarget(CGameObject* obj);

	  void UnSetTarget()
	  {
	    SetTarget("");
	  }

  private:
    void ApplyChanges();
    void SetViewport();
    void SetUp();
    void Clear();

    bool DrawSkybox();
};

//BOOST_CLASS_EXPORT_KEY( CComponent_Camera );

#endif /* __COMPONENT_CAMERA_H_ */
