#ifndef __OBJECT_H_
#define __OBJECT_H_

#include "_globals.h"
#include "_components.h"
#include "systems/_debug.h"

// Inútil (de momento)
enum gameObject_flags { gof_none = 0x00, gof_render = 0x01, gof_event = 0x02, gof_kevent = 0x04, gof_loop = 0x08 };
enum gameObject_type {gameObject_empty = 0};

typedef void (*function_t)(CGameObject* self);

// Nota: añadir "CGameObject_NULL" que no haga nada en sus operaciones. Así, si el manager devuelve un NULL, y se trata de acceder a un método de ese NULL, no se hará nada
// -> Ya que devolverá CGameObject_NULL, y no NULL
// -> Si no, crear una instancia de CGameObject llamado "GAMEOBJECT_NULL", o algo por el estilo
// -> O un flag tipo "null_object" en los objetos actuales que no haga nada en las funciones si el objeto es nulo.

class CGameObject
{
  public:
    //typedef void (CGameObject::*callfunc_ptr)(input_t, output_t);
    friend class CComponent;
    friend class CComponent_Transform;
    friend class CSystem_GameObject_Manager;
    friend class CSystem_Debug;
    friend class boost::serialization::access;

  protected:
    flags_t flags;
    bool inited;
    bool enabled;
    //bool DontDeleteOnLoad y void DontDeleteOnLoad();

    string name;
    int id;

    map<int, CComponent*> components;
    map<string, CGameObject*> children;

    CGameObject* Parent;

    function_t start;
    function_t behaviour;
    function_t event_behaviour;
    function_t keyevent_behaviour;
    function_t render;

  private:
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & flags;
      ar & inited;
      ar & enabled;
      ar & name;
      ar & id;

      ar & components; // fallo al guardar objetos instanciados por punteros (boost::serialization)
      ar & children;

      // Inservible
      //ar & start & behaviour & event_behaviour & keyevent_behaviour;
    }

  public:
    CGameObject(string name);
    CGameObject();

    virtual ~CGameObject()
    {
      Close();
    }

    void Init();
      virtual void InitCustom(){};
    void Close();
      virtual void CloseCustom(){};

    flags_t Get_flags() {return flags; } // Los flags sólo deberían ser maniplados por el objeto, no por el usuario

    bool AddChild(CGameObject* child);
    short int AddChildren(const vector<CGameObject*>& children); // Returns 1 on succes. Otherwise, returns the negative index of the last element which failed at AddChild (0, -1, -2, ..., -N)
    //bool AddChildren(const std::initializer_list<CGameObject*>& init) { return }


    bool RemoveChild(string name);
    void RemoveChildren();
    CGameObject* GetChild(string name);
    CGameObject* GetChild(uint index);
    inline uint GetNumChildren(){return children.size();}

    inline CGameObject* GetParent()
    {
      return Parent;
    }
    inline void SetParent(CGameObject* parent)
    {
      if(parent != NULL)
        Parent = parent;
    }
    inline void RemoveParent()
    {
      Parent->RemoveChild(name);
    }
    inline void UnParent() // no usar nunca desde fuera
    {
      //Parent->RemoveChild(name);
      Parent = NULL;
    }

    inline void SetStartFunction(function_t f)
    {
      start = f;
    }

    inline void SetBehaviourFunction(function_t f)
    {
      behaviour = f;
    }

    inline void SetEventFunction(function_t f)
    {
      event_behaviour = f;
    }

    inline void SetKeyEventFunction(function_t f)
    {
      keyevent_behaviour = f;
    }

    inline void SetRenderFunction(function_t f)
    {
      render = f;
    }

    inline void CallStartFunction()
    {
      if(start) start(this);
    }

    inline void CallBehaviourFunction()
    {
      if(behaviour) behaviour(this);
    }

    inline void CallEventFunction()
    {
      if(event_behaviour) event_behaviour(this);
    }

    inline void CallKeyEventFunction()
    {
      if(keyevent_behaviour) keyevent_behaviour(this);
    }

    inline void CallRenderFunction()
    {
      if(render) render(this);
    }

  public:
    int AddToSystem();
    bool RemoveFromSystem();

    void OnEvent();
    void OnKeyEvent();
    void OnLoop();
    void OnRender();
    //void OnRenderDebug();

  protected:
    void Register(uint ID)
    {
      id = ID;
    }

  public:

    inline string GetName()
    {
      return name;
    }
    inline int GetID()
    {
      return id;
    }

    void Enable(bool recursive = false);
    void Disable(bool recursive = false);
    void SetState(bool state, bool recursive = false);

    inline bool IsEnabled()
    {
      return enabled;
    }

    inline bool IsInited()
    {
      return inited;
    }

    /** PLANTILLAS **/
    template <class Type>
    Type* GetComponent();

    CComponent* GetComponent(components::components c)
    {
      if(components.find(c) != components.end())
        return components[c];

      return NULL;
    }

    template <class Type>
    bool AddComponent();

    template <class Type>
    bool RemoveComponent();

    template <class Type>
    void SetComponentStateComponent(bool state);
	
    template <class Type>
    void SetData(input_t data);

    template <class Type>
    output_t GetData();
    /**
     * Funciones para acceso directo a los componentes aquí:
     */

    /**
     * CComponent_Transform* transform() {return (CComponent_Transform*) components[components::transform]; }
     *
     * Y así para los componentes principales.
     * Si no existe un componente, se crea. A diferencia de GetComponent<...>(), si el componente no existe, devuelve NULL.
     * **/

    inline CComponent_Transform* Transform()
    {
      // Siempre existe un componente Transform
      return (CComponent_Transform*)components[components::transform];
    }

    inline CComponent_Camera* Camera()
    {
      if(components.find(components::camera) == components.end())
        components.insert(pair<int, CComponent*>(components::camera, new CComponent_Camera(this)));

      return (CComponent_Camera*)components[components::camera];
    }

    inline CComponent_Mesh_Render* MeshRender()
    {
      if(components.find(components::mesh_render) == components.end())
        components.insert(pair<int, CComponent*>(components::mesh_render, new CComponent_Mesh_Render(this)));

      return (CComponent_Mesh_Render*)components[components::mesh_render];
    }

    inline CComponent_Particle_Emitter* ParticleEmitter()
    {
      if(components.find(components::particle_emitter) == components.end())
        components.insert(pair<int, CComponent*>(components::particle_emitter, new CComponent_Particle_Emitter(this)));

      return (CComponent_Particle_Emitter*)components[components::particle_emitter];
    }

    inline CComponent_GUI_Texture* GUITexture()
    {
      if(components.find(components::gui_texture) == components.end())
        components.insert(pair<int, CComponent*>(components::gui_texture, new CComponent_GUI_Texture(this)));

      return (CComponent_GUI_Texture*)components[components::gui_texture];
    }

    inline CComponent_Audio_Source* AudioSource()
    {
      if(components.find(components::audio_source) == components.end())
        components.insert(pair<int, CComponent*>(components::audio_source, new CComponent_Audio_Source(this)));

      return (CComponent_Audio_Source*)components[components::audio_source];
    }
};

template <class Type>
Type* CGameObject::GetComponent()
{
  if(components.find(Type::GetID()) != components.end())
  {
    return (Type*)components[Type::GetID()];
  }
  //gSystem_Debug.console_error_msg("From CGameObject \"%s\": Could not find component \"%s\"", name.c_str(), components::component_to_string((components::components)Type::GetID()));
  return NULL;
}

template <class Type>
bool CGameObject::AddComponent()
{
  int id = Type::GetID();
  if(components.find(id) != components.end())
    return false;

  components.insert(pair<int, CComponent*>(id, new Type(this)));
  return true;
}

template <class Type>
bool CGameObject::RemoveComponent()
{
  int id = Type::GetID();
  if(id == components::transform)
	  return false;
	
  map<int, CComponent*>::iterator it = components.find(id);
  if(it != components.end())
  {
    delete it->second;
    components.erase(it);

    return true;
  }

  return false;
}

template <class Type>
void CGameObject::SetComponentStateComponent(bool state)
{
  if(components.find(Type::GetID()) != components.end())
    components[Type::GetID()]->SetState(state);
}

template <class Type>
void CGameObject::SetData(input_t data)
{
  int id = Type::GetID();
  if(components.find(id) != components.end() && data)
    components[id]->Set(data);
}

template <class Type>
output_t CGameObject::GetData()
{
  int id = Type::GetID();
  if(components.find(id) != components.end())
    return components[id]->Get();
  else
    return NULL;
}

#endif /* __OBJECT_H_ */
