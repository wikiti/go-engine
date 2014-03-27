/**
 * @file
 * @brief Estructura de los objeto "GameObject".
 */

#ifndef __OBJECT_H_
#define __OBJECT_H_

#include "_globals.h"
#include "_components.h"
#include "systems/_debug.h"

// Inútil (de momento)
/**
 * Enum sin uso.
 */
enum gameObject_flags { gof_none = 0x00, gof_render = 0x01, gof_event = 0x02, gof_kevent = 0x04, gof_loop = 0x08 };
/**
 * Enum sin uso.
 */
enum gameObject_type {gameObject_empty = 0};//!< gameObject_empty

/**
 * Puntero a función miembro de un game object.
 *
 * El puntero se usará para funciones del tipo "void f(CGameObject* g)". Véase CGameObject::start,
 * CGameObject:: behaviour, CGameObject:: event_behaviour, CGameObject:: input_behaviour, CGameObject:: render.
 *
 * Se usará para guardar en el game object una serie de punteros a funciones externas para ser llamadas en determinados momentos,
 * usando como referencia al objeto el parámetro "self".
 *
 * @param self Referencia a un objeto dado.
 */
typedef void (*function_t)(CGameObject* self);

// Nota: añadir "CGameObject_NULL" que no haga nada en sus operaciones. Así, si el manager devuelve un NULL, y se trata de acceder a un método de ese NULL, no se hará nada
// -> Ya que devolverá CGameObject_NULL, y no NULL
// -> Si no, crear una instancia de CGameObject llamado "GAMEOBJECT_NULL", o algo por el estilo
// -> O un flag tipo "null_object" en los objetos actuales que no haga nada en las funciones si el objeto es nulo.

/**
 * Clase que representa a un objeto de juego.
 *
 * Para entender mejor esta parte que voy a explicar, recomiendo leer el artículo siguiente: http://www.genbetadev.com/programacion-de-videojuegos/diseno-de-videojuegos-orientado-a-entidades-y-componentes
 *
 * @image html diagrama1.png
 *
 * La unidad básica con la que trabajará nuestro motor gráfico de cara al usuario que desarrollará el juego será los objectos que realizarán una abstracción de cualquier cosa, a los que hemos llamado cariñosamente “Game Objects“. Un Game Object, por consiguiente, es la representación de una entidad en el mundo real. Cada objeto debe tener un nombre para referirnos a él ("coche1", "coche1_rueda1", etc.) y un identificador (que será usado de cara al sistema). Por ejemplo, un coche sería uno, y las ruedas que lo componen, también.
 *
 * Ahora bien, si el coche se mueve, se deben mover las ruedas, las luces, los ejes… por lo que diremos que esos objetos están emparentados (apartado 1 en la imagen adjunta). Creando esta jerarquía, conseguimos agrupar de forma lógica los objetos, dándose el caso de que los objetos puedan tener varios hijos (o children) y un único o ningún padre, creando una relación bidireccional entre el padre y sus hijos por medio de una lista de punteros y un único puntero, respectivamente. Evidentemente, no es posible que un padre sea padre de sí mismo, directa o indirectamente.
 *
 * De todas formas, no todos los objetos son iguales, ¿verdad?. Algunos están definidos con una serie de valores y propiedades distintas a los demás. Por ejemplo, una pelota tiene un modelo de una pelota 3D (una esfera), un material (plástico), una esfera de colisión… mientras que un temporizador sólo posee una parte lógica que determina si ha transcurrido un lapso de tiempo. ¿Cómo distinguimos pues unos objetos de otros? Lo haremos mediante componentes. Cada objeto puede tener, a lo sumo, un componente de cada tipo. Por ejemplo, podemos definir varios componentes:
 * <ul>
 * <li><b>Transformación:</b> Posición, rotación y escala de un objeto en el mundo 3D.
 * <li><b>Cámara:</b> Define una cámara desde la cual se hará un renderizado de una escena.
 * <li><b>Texto de interfaz gráfica de usuario</b> (GUI Text).
 * <li><b>Luz estática:</b> Usada para iluminar el escenario…
 * </ul>
 * Y así, se puede crear una lista muy larga de componentes que nos servirá para personalizar nuestros componentes. En el apartado 2 se pueden ver algunas formas de invocar funciones desde el objeto para obtener o modificar datos de los componentes. Intentaré dedicar un post a ello.
 *
 * Por si fuera poco, vamos a definir 4 nuevos punteros para personalizar los objetos, que serán punteros a funciones que nos permitirán hacer llamadas a funciones dependiendo de su tipo:
 * <ul>
 * <li><b>start:</b> Se llama al inicio, cuando se crea el objeto y se añade al sistema.
 * <li><b>behaviour:</b> Se llama en cada iteración, y determina el próximo estado del objeto.
 * <li><b>event:</b> Se llama cada vez que se ejecute un evento (movimiento del ratón, reescalado de ventana, etc.).
 * <li><b>input:</b> Se llama cada vez que se produce una entrada de teclado, joystick, ratón, etc. Usar antes que "event".
 * <li><b>rener:</b> Se llama cada vez que se va a redenderizar  una tecla del teclado.
 * </ul>
 * Por ejemplo, se define una función “behaviour” para un objeto de la siguiente forma:
 *
 @code
  void GO1_movimiento(CGameObject* gameObject)
  {
    // Operaciones para el objeto que vaya a usar esta función irán aquí.
  }

  CGameObject* GO1 = gSystem_GameObject_Manager.AddGameObject("GO1"); // Crear objeto
  GO1->SetKeyEventBehaviourFunction(&GO1_movimiento);                 // Cargar función behaviour
@endcode
 * Que será llamada en cada iteración de la ejecución de nuestra aplicación, pasando como parámetro el puntero del objeto que apunta a sí mismo (this, básicamente). El objeto llamado gSystem_GameObject_Manager será el sistema que se encargará de gestionar nuestros objetos, del que me encargaré de explicar en el próximo post.
 */
class CGameObject
{
  friend class CComponent;
  friend class CComponent_Transform;
  friend class CSystem_GameObject_Manager;
  friend class CSystem_Debug;
  friend class CSystem_Render;

  protected:
    flags_t flags;
    bool inited;
    bool enabled;
    bool preserve;
    //bool DontDeleteOnLoad y void DontDeleteOnLoad();

    std::string name;
    int id;

    std::map<int, CComponent*> components;
    std::map<std::string, CGameObject*> children;

    CGameObject* Parent;

    function_t start;
    function_t behaviour;
    function_t event_behaviour;
    function_t input_behaviour;
    function_t render;

  private:
    /*template<class Archive>
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
    }*/

  public:
    /**
     * Constructor principal.
     *
     * Construye un objeto "vacío" dado un nombre. Crear un objeto no implica que se añada al sistema gestor de objetos CSystem_GameObject_Manager.
     *
     * El objeto
     *
     * @param name
     */
    CGameObject(std::string name);
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
    short int AddChildren(const std::vector<CGameObject*>& children); // Returns 1 on succes. Otherwise, returns the negative index of the last element which failed at AddChild (0, -1, -2, ..., -N)
    //bool AddChildren(const std::initializer_list<CGameObject*>& init) { return }


    bool RemoveChild(std::string name);
    void RemoveChildren();
    CGameObject* GetChild(std::string name);
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

    inline void SetInputFunction(function_t f)
    {
      input_behaviour = f;
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
      if(input_behaviour) input_behaviour(this);
    }

    inline void CallRenderFunction()
    {
      if(render) render(this);
    }

  public:
    int AddToSystem();
    bool RemoveFromSystem();

    void OnEvent();
    void OnInput();
    void OnLoop();
    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);
    //void OnRenderDebug();

  protected:
    void Register(uint ID)
    {
      id = ID;
    }

  public:

    inline std::string GetName()
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

    void Preserve(bool recursive = false);
    void UnPreserve(bool recursive = false);
    void SetPreserve(bool state, bool recursive = false);

    inline bool IsEnabled()
    {
      return enabled;
    }

    inline bool IsInited()
    {
      return inited;
    }

    inline bool IsPreserved()
    {
      return preserve;
    }

    bool NearBy(CGameObject* go, double distance);

    /** PLANTILLAS **/
    template <class Type>
    Type* GetComponent();

    CComponent* GetComponent(Components::components_t c)
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
      return (CComponent_Transform*)components[Components::transform];
    }

    inline CComponent_Camera* Camera()
    {
      if(components.find(Components::camera) == components.end())
        components.insert(std::pair<int, CComponent*>(Components::camera, new CComponent_Camera(this)));

      return (CComponent_Camera*)components[Components::camera];
    }

    inline CComponent_Mesh_Render* MeshRender()
    {
      if(components.find(Components::mesh_render) == components.end())
        components.insert(std::pair<int, CComponent*>(Components::mesh_render, new CComponent_Mesh_Render(this)));

      return (CComponent_Mesh_Render*)components[Components::mesh_render];
    }

    inline CComponent_Particle_Emitter* ParticleEmitter()
    {
      if(components.find(Components::particle_emitter) == components.end())
        components.insert(std::pair<int, CComponent*>(Components::particle_emitter, new CComponent_Particle_Emitter(this)));

      return (CComponent_Particle_Emitter*)components[Components::particle_emitter];
    }

    inline CComponent_GUI_Texture* GUITexture()
    {
      if(components.find(Components::gui_texture) == components.end())
        components.insert(std::pair<int, CComponent*>(Components::gui_texture, new CComponent_GUI_Texture(this)));

      return (CComponent_GUI_Texture*)components[Components::gui_texture];
    }

    inline CComponent_Audio_Source* AudioSource()
    {
      if(components.find(Components::audio_source) == components.end())
        components.insert(std::pair<int, CComponent*>(Components::audio_source, new CComponent_Audio_Source(this)));

      return (CComponent_Audio_Source*)components[Components::audio_source];
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

  components.insert(std::pair<int, CComponent*>(id, new Type(this)));
  return true;
}

template <class Type>
bool CGameObject::RemoveComponent()
{
  int id = Type::GetID();
  if(id == Components::transform)
	  return false;
	
  std::map<int, CComponent*>::iterator it = components.find(id);
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
