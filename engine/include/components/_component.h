#ifndef __COMPONENT_H_
#define __COMPONENT_H_

#include "_globals.h"

namespace components
{
  //   enum components { base = 0, camera = 0x01, dummy1 = 0x02, transform = 0x04};
  // ¿Usar esto como flags para pasarle a CGameObject?
  enum components { base = 0, camera, mesh_render, particle_emitter, gui_texture, audio_source, transform, dummy1, dummy2, dummy3, __not_defined};

  extern const char* components_s[];
  // Añadir componente "script" al final, para tener script+1, script+2, script+3... Y diferenciarlos por sus nombres, o por su orden de añadido
  // Añadiendo pues una variable tipo "num_scripts" dentro del gameObject

  const char* component_to_string(components c);
  int string_to_component(const string& c);
}

class CGameObject;

typedef void (*function_t)(CGameObject* self);

class CComponent
{
  friend class CGameObject;
  friend class CSystem_Debug;

  friend const char* components::component_to_string(components c);
  friend int components::string_to_component(const string& c);

  protected:
    bool enabled;
    CGameObject* gameObject;

    virtual void parseDebug(string command);

  private:
    static int GetID() { return components::base; }

    /*friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & enabled;
      //ar & id;
    }*/

  public:
    CComponent(){};
    CComponent(CGameObject* container);
    virtual ~CComponent();

    virtual void Set(input_t data){};
    virtual output_t Get(){return NULL;};

    inline virtual void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix){};
    inline virtual void OnLoop(){};
    inline virtual void OnEvent(){};
    inline virtual void OnInput(){};

    //virtual bool AddFuncs(CGameObject* obj){return true;};
    //virtual bool RemoveFuncs(CGameObject* obj){return true;};

    inline bool GetState()
    {
      return enabled;
    }

    inline void SetState(bool state = true)
    {
      enabled = state;
    }

    inline void Enable()
    {
      enabled = true;
    }

    inline void Disable()
    {
      enabled = false;
    }

    inline CGameObject* GetGameObject()
    {
      return gameObject;
    }
};

//BOOST_SERIALIZATION_ASSUME_ABSTRACT( CComponent );
//BOOST_CLASS_EXPORT_KEY( CComponent );


#endif /* __COMPONENT_H_ */
