#ifndef __OBJECT_H_
#define __OBJECT_H_

#include "_globals.h"
#include "_components.h"
#include "systems/_debug.h"

// Inútil (de momento)
enum gameObject_flags { gof_none = 0x00, gof_render = 0x01, gof_event = 0x02, gof_kevent = 0x04, gof_loop = 0x08 };
enum gameObject_type {gameObject_empty = 0};

typedef void (*function_t)(CGameObject* self);

class CGameObject
{
  public:
    //typedef void (CGameObject::*callfunc_ptr)(input_t, output_t);
    friend class CComponent;
    friend class CComponent_Transform;
    friend class CSystem_GameObject_Manager;
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
    CGameObject(){};
    CGameObject(string name);
    virtual ~CGameObject(){}

    void Init();
      virtual void InitCustom(){};
    void Close();
      virtual void CloseCustom(){};

    flags_t Get_flags() {return flags; } // Los flags sólo deberían ser maniplados por el objeto, no por el usuario

    bool AddChild(CGameObject* children);
    bool RemoveChild(string name);
    void RemoveChildren();
    CGameObject* GetChild(string name);
    CGameObject* GetChild(uint index);
    uint GetNumChildren(){return children.size();}

    CGameObject* GetParent()
    {
      return Parent;
    }
    void SetParent(CGameObject* parent)
    {
      if(parent != NULL)
        Parent = parent;
    }
    void RemoveParent()
    {
      Parent->RemoveChild(name);
    }
    void UnParent() // no usar nunca desde fuera
    {
      //Parent->RemoveChild(name);
      Parent = NULL;
    }

    void SetStartFunction(function_t f)
    {
      start = f;
    }

    void SetBehaviourFunction(function_t f)
    {
      behaviour = f;
    }

    void SetEventBehaviourFunction(function_t f)
    {
      event_behaviour = f;
    }

    void SetKeyEventBehaviourFunction(function_t f)
    {
      keyevent_behaviour = f;
    }

    void CallStartFunction()
    {
      if(start) start(this);
    }

    void CallBehaviourFunction()
    {
      if(behaviour) behaviour(this);
    }

    void CallEventBehaviourFunction()
    {
      if(event_behaviour) event_behaviour(this);
    }

    void CallKeyEventBehaviourFunction()
    {
      if(keyevent_behaviour) keyevent_behaviour(this);
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

    inline void Enable()
    {
      enabled = true;
    }
    inline void Disable()
    {
      enabled = false;
    }
    inline void SetState(bool state)
    {
      enabled = state;
    }

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

    inline CComponent_Transform* transform()
    {
      // Siempre existe un componente Transform
      return (CComponent_Transform*)components[components::transform];
    }

    inline CComponent_Camera* camera()
    {
      if(components.find(components::camera) == components.end())
        components.insert(pair<int, CComponent*>(components::camera, new CComponent_Camera(this)));

      return (CComponent_Camera*)components[components::camera];
    }

    inline CComponent_Mesh_Render* meshRender()
    {
      if(components.find(components::mesh_render) == components.end())
        components.insert(pair<int, CComponent*>(components::mesh_render, new CComponent_Mesh_Render(this)));

      return (CComponent_Mesh_Render*)components[components::mesh_render];
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
