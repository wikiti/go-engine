/**
 * @file
 * @brief Estructura de los objeto "GameObject".
 */

#ifndef __OBJECT_H_
#define __OBJECT_H_

#include "_globals.h"
#include "_components.h"
#include "systems/_debug.h"

//namespace GO {

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
     * El objeto poseerá un nombre especificado por "name" y tendrá un componente inicial, CComponent_Transform, que indicará su siutación, orientación y tamaño en el espacio.
     *
     * @param name Nombre del game object.
     */
    CGameObject(std::string name);

    /**
     * Constructor vacío.
     *
     * Crea un objeto vacío, sin propiedades y sin usar atributos. Usado principalmente para copias y pruebas.
     */
    CGameObject();

    /**
     * Destructor principal.
     *
     * Desactiva por completo el objeto, además de eliminar todos sus componentes y atributos.
     */
    virtual ~CGameObject()
    {
      Close();
    }

    /**
     * Iniciar el objeto.
     *
     * Inicializa el gameObject, haciendo que pase a estar activado, además de añadir el componente CComponent_Transform (por si no ha sido añadido, claro).
     * Además, el objeto queda marcado como <b>iniciado</b> y como <b>activo.</b> Sólo podrá ser iniciado si no ha sido iniciado previamente, osi ha sido cerrado.
     */
    void Init();
      /** <span style="color: red; font-weight: bold;">Sin usar</span>. */
      virtual void InitCustom(){};
    /**
     * Cerrar el objeto.
     *
     * Una vez iniciado, el gameObject puede ser cerrado, eliminando todos sus <b>componentes</b> e <b>hijos</b>.  Además, el objeto es marcado como
     * <b>no iniciado</b> y como <b>desactivado</b>.
     */
    void Close();
      /** <span style="color: red; font-weight: bold;">Sin usar</span>. */
      virtual void CloseCustom(){};

    /** <span style="color: red; font-weight: bold;">Sin usar</span>. */
    flags_t Get_flags() {return flags; } // Los flags sólo deberían ser maniplados por el objeto, no por el usuario

    /**
     * Añadir un nuevo hijo.
     *
     * Añade un nuevo hijo al objeto actual dado otro objeto existente, siempre y cuando sea posible.
     * Sólo se podrá añadir un hijo si el padre no es hijo de alguno de los hijos del objeto a añadir, o si el onjeto no es idéntico al objeto a añadir.
     * Esto significa que no puede haber ciclos en la jerarquía de gameObjects. Finalmente, asigna al hijo añadido, si todo ha funcionado correctamente, un padre tal que sea el objeto actual.
     *
     * @warning En caso de añadir un objeto con un nombre ya existente, se perderá la referencia al objeto añadido anteriormente. Asegúrese primero de que el objeto no es hijo del padre antes de añadirlo.
     *
     * @param child Puntero a gameObject a añadir. Debe estar inicializado correctamente.
     * @return true si añade el hijo correctamente, false en cualquier otro caso.
     */
    bool AddChild(CGameObject* child);
    /**
     * Añadir una lista de hijos.
     *
     * Dada una lista de hijos, se pueden añadir al objeto actual todos los hijos de la lista.
     * Se intentará añadir hijo a hijo todos los posibles que esten en la lista, siempre y cuando cumplan las condiciones de CGameObject::AddChild().
     * A todo objeto añadido, se le asignará como padre el objeto actual.
     *
     * Por ejemplo, se podrá usar este método de la siguiente manera:
     *
     @code
     CGameObject* go1  = new CGameObject("go1");
     CGameObject* go11 = new CGameObject("go11");
     CGameObject* go12 = new CGameObject("go12");

     go1->AddChildren({go11, go12});
     @endcode
     * @bug Para listas de objetos de tamaño menor que 2, es posible que haya que especificar el tipo, tal que se represente como <span><i>std::vector<CGameObject*>{go11, go12}</i></span>
     *
     * @param children Array que contendrá los punteros a los hijos a añadir.
     * @return En caso de éxtio, devuelve el valor "1". En caso de fallo, devuelve el índice de la última posición en la que falló, con valor negativo (0, -1, -2, ..., -(n-1)).
     */
    short int AddChildren(const std::vector<CGameObject*>& children); // Returns 1 on succes. Otherwise, returns the negative index of the last element which failed at AddChild (0, -1, -2, ..., -N)
    //bool AddChildren(const std::initializer_list<CGameObject*>& init) { return }

    /**
     * Eliminar un hijo.
     *
     * Elimina un hijo del objeto de juego.
     * @warning El hijo será eliminado de la lista de hijos, pero no será borrado del sistema. Esto es, no se llamará a CGameObject::Close() ni se hará un **delete**.
     * @param name Nombre del hijo a eliminar.
     * @return En caso de borrarlo correctamente, devuelve true. En otro caso, retorna false.
     */
    bool RemoveChild(std::string name);
    /**
     * Eliminar a todos los hijos.
     *
     * Elimina a todos los hijos del objeto actual.
     * @warning Los hijos serám eliminados de la lista de hijos, pero no serám borrados del sistema. Esto es, no se llamará a CGameObject::Close() ni se harán **delete**.
     */
    void RemoveChildren();

    /**
     * Obtener un puntero a un hijo.
     *
     * Dado un nombre de un hijo, retorna un puntero a la estructura de datos que representa a ese hijo.
     * @param name Nombre del objeto a obtener.
     * @return Devuelve un puntero al objeto si es un hijo del objeto actual. En otro caso, devuelve NULL.
     */
    CGameObject* GetChild(std::string name);
    /**
     * Obtiene un puntero a un hijo.
     *
     * Dado un índice, se accederá a un hijo que ocupe dicho índice. Los hijos serán ordenados por orden alfabético (funcionamiento de un std::map).
     * @param index Valor entre 0 y el número de hijos actual. Véase CGameObject::GetNumChildren().
     * @return Devuelve el puntero al objeto si es un hijo del objeto actual. En otro caso, devuelve NULL.
     */
    CGameObject* GetChild(uint index);
    /**
     * Número de hijos.
     *
     * Retorna el número de hijos del objeto actual.
     * @return Número de hijos del objeto actual (tamaño de la lista de hijos).
     */
    inline uint GetNumChildren(){return children.size();}

    /**
     * Obtener el padre.
     *
     * @return Retorna un puntero al padre que lleva a este hijo. Si es huérfano, devolverá NULL.
     */
    inline CGameObject* GetParent()
    {
      return Parent;
    }

    /**
     * Elimina el padre.
     *
     * Si el objeto tiene un padre, tratará de volverse huérfano, eliminando el puntero a su padre, y limpiando la referencia del padre al hijo en el padre.
     */
    inline void RemoveParent()
    {
      Parent->RemoveChild(name);
    }

    /**
     * Cambiar el callback "start".
     *
     * Cambia el puntero del callback **Start**, que será una función que se llamará al iniciar el objeto.
     * @param f Puntero a función externa. Si es NULL, suponemos que no se usará ningún callback.
     */
    inline void SetStartFunction(function_t f)
    {
      start = f;
    }

    /**
     * Cambiar el callback "behaviour".
     *
     * Cambia el puntero del callback **Behaviour**, que será una función que se llamará en cada iteración del juego (actualización constante).
     * @param f Puntero a función externa. Si es NULL, suponemos que no se usará ningún callback.
     */
    inline void SetBehaviourFunction(function_t f)
    {
      behaviour = f;
    }

    /**
     * Cambiar el callback "event".
     *
     * Cambia el puntero del callback **Event**, que será una función que se llamará cada vez que se produzca un evento.
     * @param f Puntero a función externa. Si es NULL, suponemos que no se usará ningún callback.
     */
    inline void SetEventFunction(function_t f)
    {
      event_behaviour = f;
    }

    /**
     * Cambiar el callback "input".
     *
     * Cambia el puntero del callback **Event**, que será una función que se llamará en cada iteración del juego, que procesará datos de entrada (teclado, retón, joystick...). Véase CSystem_UserInput.
     * @param f Puntero a función externa. Si es NULL, suponemos que no se usará ningún callback.
     */
    inline void SetInputFunction(function_t f)
    {
      input_behaviour = f;
    }

    /**
     * Cambiar el callback "render".
     *
     * Cambia el puntero del callback **Render**, que será una función que se llamará en cada iteración del juego, a la hora de dibujar el objeto por pantalla.
     * @param f Puntero a función externa. Si es NULL, suponemos que no se usará ningún callback.
     */
    inline void SetRenderFunction(function_t f)
    {
      render = f;
    }

    /**
     * LLama al callback **Start**, siempre y cuando no sea NULL. Será llamado de manera automática desde CGameObject.Init().
     */
    inline void CallStartFunction()
    {
      if(start) start(this);
    }

    /**
     * LLama al callback **Behaviour**, siempre y cuando no sea NULL. Será llamado de manera automática desde CGameObject.OnLoop().
     */
    inline void CallBehaviourFunction()
    {
      if(behaviour) behaviour(this);
    }


    /**
     * LLama al callback *Event**, siempre y cuando no sea NULL. Será llamado de manera automática desde CGameObject.OnEvent().
     */
    inline void CallEventFunction()
    {
      if(event_behaviour) event_behaviour(this);
    }


    /**
     * LLama al callback **Input**, siempre y cuando no sea NULL. Será llamado de manera automática desde CGameObject.OnInput().
     */
    inline void CallInputFunction()
    {
      if(input_behaviour) input_behaviour(this);
    }

    /**
     * LLama al callback **Render**, siempre y cuando no sea NULL. Será llamado de manera automática desde CGameObject.OnRender().
     */
    inline void CallRenderFunction()
    {
      if(render) render(this);
    }

  public:
    /**
     * Registrar un objeto en el sistema.
     *
     * Registra un objeto de juego en la lista de objetos globales, administrado por el sistema CSystem_GameObject_Manager.
     * Un objeto no podrá ser añadido si ya existe un objeto con su nombre, o si su identificador es inválido (véase Utils::validateIdentifier).
     * @return Si se registra correctamente, devuelve un identificador numérico del objeto. En caso contrario, devuelve -1.
     */
    int AddToSystem();
    /**
     * Elimina un objeto del sistema.
     *
     * Elimina un objeto de la lista de objetos globales, administrado por el sistema CSystem_GameObject_Manager.
     * Un objeto no podrá ser eliminado si no está registrado.
     * @return Devuelve true si se ha eliminado del sistema correctamente, false en cualquier otro caso.
     */
    bool RemoveFromSystem();

    /**
     * Gestor de eventos.
     *
     * Función para gestionar los eventos del objeto y de todos sus componentes. Será llamado desde CSystem_GameObject_Manager::OnEvent(), al principio de cada iteración.
     *
     * Si el objeto no está en modo **activo** e **iniciado**, no se llamará a esta función.
     */
    void OnEvent();
    /**
     * Gestor de entrada.
     *
     * Función para gestionar la entrada (ratón, teclado, etc.) y de todos sus componentes. Será llamado desde CSystem_GameObject_Manager::OnInput(), después de CSystem_GameObject_Manager::OnEvent().
     *
     * Si el objeto no está en modo **activo** e **iniciado**, no se llamará a esta función.
     */
    void OnInput();
    /**
     * Gestor de comportamientos.
     *
     * Función para gestionar el comportamiento del objeto y de todos sus componentesen cada iteración. Será llamado desde CSystem_GameObject_Manager::OnLoop(), después de CSystem_GameObject_Manager::OnInput().
     *
     * Si el objeto no está en modo **activo** e **iniciado**, no se llamará a esta función.
     */
    void OnLoop();
    /**
     * Gestor de renderizado.
     *
     * Función para gestionar el dibujado del objeto y de todos sus componentes. Será llamado desde CSystem_GameObject_Manager::OnRender(), después de CSystem_GameObject_Manager::OnLoop().
     *
     * Si el objeto no está en modo **activo** e **iniciado**, no se llamará a esta función.
     */
    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);
    //void OnRenderDebug();

  protected:
    void Register(uint ID)
    {
      id = ID;
    }

    inline void SetParent(CGameObject* parent)
    {
      if(parent != NULL)
        Parent = parent;
    }

    inline void UnParent() // no usar nunca desde fuera
    {
      //Parent->RemoveChild(name);
      Parent = NULL;
    }

  public:

    /**
     * Getter del nombre del objeto.
     * @return Nombre del objeto. Si está registrado en el sistema, se asegura de que es un identificador válido.
     */
    inline std::string GetName()
    {
      return name;
    }
    /**
     * Getter del identificador del objeto en el sistema.
     * @return Si está registrado, el identificador tiene un valor mayor o igual que 0. En otro caso, vale -1.
     */
    inline int GetID()
    {
      return id;
    }

    /**
     * Activa el objeto.
     *
     * Cambia el estado del objeto actual por **activo**.
     * @param recursive Si es true, se activarán todos sus hijos de manera recursiva. En caso contrario, sólo se activará el objeto actual.
     */
    void Enable(bool recursive = false);
    /**
     * Desactiva el objeto.
     *
     * Cambia el estado del objeto actual por **desactivado**.
     * @param recursive Si es true, se desactivarán todos sus hijos de manera recursiva. En caso contrario, sólo se desactivará el objeto actual.
     */
    void Disable(bool recursive = false);
    /**
     * Cambiar el estado del objeto.
     *
     * Cambia el estado del objeto actual por el estado especificado por "state" (true -> **activo**, false -> **desactivado**.
     *
     * @param state Nuevo estado de los objetos.
     * @param recursive Si es true, se cambiará el estado todos sus hijos de manera recursiva. En caso contrario, sólo se cambiará el objeto actual.
     */
    void SetState(bool state, bool recursive = false);

    /**
     * Preservar el objeto entre estancias.
     *
     * Un objeto se conoce como que está preservado si mantiene sus datos intactos entre los cambios de estancia. Básicamente, son objetos que
     * no será borrados al cambiar de estancia o nivel, y que permanecerá con el mismo nombre y el mismo contenido que en su última estancia.
     *
     * Véase CEngine y CInstance.
     * @param recursive  Si es true, se marcarán como ***preservar*** todos sus hijos de manera recursiva. En caso contrario, sólo se cambiará el objeto actual.
     */
    void Preserve(bool recursive = false);
    /**
     * No preservar el objeto entre estancias.
     *
     * Antónimo de CGameObject::Preserve().
     *
     * Véase CEngine y CInstance.
     * @param recursive  Si es true, se marcarán como ***no preservar*** todos sus hijos de manera recursiva. En caso contrario, sólo se cambiará el objeto actual.
     */
    void UnPreserve(bool recursive = false);
    /**
     * Cambiar preservación del objeto entre estancias.
     *
     * Combina CGameObject::Preserve() y CGameObject::UnPreserve().
     *
     * Véase CEngine y CInstance.
     *
     * @param state Nuevo estado (true -> **preservar**, false -> **no preservar**)
     * @param recursive  Si es true, se marcarán como el estado indicado a todos sus hijos de manera recursiva. En caso contrario, sólo se cambiará el objeto actual.
     */
    void SetPreserve(bool state, bool recursive = false);

    /**
     * Preguntar si el objeto está **activado**.
     *
     * @return Retorna true si el objeto está **activado**. false en caso contrario.
     */
    inline bool IsEnabled()
    {
      return enabled;
    }
    /**
     * Preguntar si el objeto está **iniciado**.
     *
     * @return Retorna true si el objeto está **iniciado**. false en caso contrario.
     */
    inline bool IsInited()
    {
      return inited;
    }

    /**
     * Preguntar si el objeto está marcado como **preservado**.
     *
     * @return Retorna true si el objeto está marcado como **preservado**. false en caso contrario.
     */
    inline bool IsPreserved()
    {
      return preserve;
    }

    /**
     * Comprobador de cercanía.
     *
     * Dado otro objeto, el objeto actual y una distancia, se comprobará si la distancia entre el origen de ambos objetos es menor a la distancia propuesta. Se usará la función de distancia euclídea.
     * @param go Otro objeto.
     * @param distance Distancia entre ambos objetos.
     * @return Devuelve true si la distancia euclídea entre los 2 objetos es menor que "distance". false en caso contrario.
     */
    bool NearBy(CGameObject* go, double distance);

    /* PLANTILLAS */
    /**
     * Obtener componente.
     *
     * Dada la función plantilla actual, se podrá obtener un componente seleccionado, siempre y cuando exista.
     *
     * Por ejemplo, podemos usar:
     @code
     CGameObject go1 = new CGameObject("go1");
     go1->GetComponent<CComponent_Transform*>()->position.x = 0; // Acceder al componente "transform" y cambiar un atributo.
     @endcode
     *
     * Véase CComponent.
     * @return Devuelve un puntero del tipo del componente solicitado. NULL si no existe el componente.
     */
    template <class Type>
    Type* GetComponent();

    /**
     * Obtener componente (genérico).
     *
     * Obtiene un componente del tipo "CComponent*", siempre y cuando exista el identificador del componente asociado.
     *
     * Véase Components::components_t y CComponent.
     * @param c Tipo de componente (enum).
     * @return Puntero al componente, si existe. NULL en otro caso.
     */
    CComponent* GetComponent(Components::components_t c)
    {
      if(components.find(c) != components.end())
        return components[c];

      return NULL;
    }

    /**
     * Añadir un componente.
     *
     * Dada la función plantilla actual, se podrá añadir un componente del tipo indicado, siempre y cuando exista.
     *
     * @warning Solo puede haber un componente de cada tipo.
     *
     * Por ejemplo, podemos usar:
     @code
     CGameObject go1 = new CGameObject("go1");
     go1->AddComponent<CComponent_Camera*>(); // Añade el componente "camera".
     @endcode
     *
     * Véase CComponent.
     * @return Retorna true si se ha añadido el componente, false en caso contrario.
     */
    template <class Type>
    bool AddComponent();

    /**
     * Eliminar un componente.
     *
     * Dada la función plantilla actual, se podrá eliminar un componente seleccionado, siempre y cuando exista en el objeto. El componente será destruido por completo.
     *
     * @warning No se puede eliminar el componente CComponent_Transform.
     *
     * Por ejemplo, podemos usar:
     @code
     CGameObject go1 = new CGameObject("go1");
     go1->RemoveComponent<CComponent_Camera*>(); // Eliminar el componente "camera".
     @endcode
     *
     * Véase CComponent.
     * @return Retorna true si se ha borrado correctamente, false en caso contrario.
     */
    template <class Type>
    bool RemoveComponent();

    /**
     * Cambiar el estado de un componente.
     *
     * Dada la función plantilla actual, se podrá cambiar el estado de un componente seleccionado, siempre y cuando exista, por **activado** o **desactivado**.
     *
     * Por ejemplo, podemos usar:
     @code
     CGameObject go1 = new CGameObject("go1");
     go1->SetComponentState<CComponent_Camera*>(true); // Activar el componente cámara.
     @endcode
     *
     * Véase CComponent.
     * @return Devuelve un puntero del tipo del componente solicitado. NULL si no existe el componente.
     */
    template <class Type>
    void SetComponentStateComponent(bool state);
	
    /** <span style="color: red; font-weight: bold;">Sin usar</span>. **/
    template <class Type>
    void SetData(input_t data);

    /** <span style="color: red; font-weight: bold;">Sin usar</span>. **/
    template <class Type>
    output_t GetData();
    /* Funciones para acceso directo a los componentes aquí: */

    /* CComponent_Transform* transform() {return (CComponent_Transform*) components[components::transform]; }
     *
     * Y así para los componentes principales.
     * Si no existe un componente, se crea. A diferencia de GetComponent<...>(), si el componente no existe, devuelve NULL.
     */

    /**
     * Acceso directo para el componente **transform** (CComponent_Transform).
     *
     * Accede de manera sencilla al componente **transform**.
     *
     * Por ejemplo, usaremos:
     *
     @code
     CGameObject go1 = new CGameObject("go1");
     go1->Transform()->position.x; // Acceder a un atributo del componente.
     // go1->GetComponent<CComponent_Transform*>()->position.x;
     @endcode
     *
     * Véase CComponent_Transform.
     *
     * @return Puntero al componente **transform**.
     */
    inline CComponent_Transform* Transform()
    {
      // Siempre existe un componente Transform
      return (CComponent_Transform*)components[Components::transform];
    }

    /**
     * Acceso directo para el componente **camera** (CComponent_Camera).
     *
     * Accede de manera sencilla al componente **camera**.
     *
     * Por ejemplo, usaremos:
     *
     @code
     CGameObject go1 = new CGameObject("go1");
     go1->Camera()->field_of_view; // Acceder a un atributo del componente.
     @endcode
     *
     * Si el componente no existe, será creado, y luego será retornado.
     *
     * Véase CComponent_Camera.
     *
     * @return Puntero al componente **camera**.
     */
    inline CComponent_Camera* Camera()
    {
      if(components.find(Components::camera) == components.end())
        components.insert(std::pair<int, CComponent*>(Components::camera, new CComponent_Camera(this)));

      return (CComponent_Camera*)components[Components::camera];
    }

    /**
     * Acceso directo para el componente **mesh render** (CComponent_Mesh_render).
     *
     * Accede de manera sencilla al componente **mesh render**.
     *
     * Por ejemplo, usaremos:
     *
     @code
     CGameObject go1 = new CGameObject("go1");
     go1->Camera()->field_of_view; // Acceder a un atributo del componente.
     @endcode
     *
     * Si el componente no existe, será creado, y luego será retornado.
     *
     * Véase CComponent_Mesh_Render.
     *
     * @return Puntero al componente **mesh render**.
     */
    inline CComponent_Mesh_Render* MeshRender()
    {
      if(components.find(Components::mesh_render) == components.end())
        components.insert(std::pair<int, CComponent*>(Components::mesh_render, new CComponent_Mesh_Render(this)));

      return (CComponent_Mesh_Render*)components[Components::mesh_render];
    }

    /**
     * Acceso directo para el componente particle emitter (CComponent_Particle_Emitter).
     *
     * Accede de manera sencilla al componente **particle emitter**.
     *
     * Por ejemplo, usaremos:
     *
     @code
     CGameObject go1 = new CGameObject("go1");
     go1->ParticleEmitter()->color; // Acceder a un atributo del componente.
     @endcode
     *
     * Si el componente no existe, será creado, y luego será retornado.
     *
     * Véase CComponent_Particle_Emitter.
     *
     * @return Puntero al componente **particle emitter**.
     */
    inline CComponent_Particle_Emitter* ParticleEmitter()
    {
      if(components.find(Components::particle_emitter) == components.end())
        components.insert(std::pair<int, CComponent*>(Components::particle_emitter, new CComponent_Particle_Emitter(this)));

      return (CComponent_Particle_Emitter*)components[Components::particle_emitter];
    }

    /**
     * Acceso directo para el componente **gui texture** (CComponent_GUI_Texture).
     *
     * Accede de manera sencilla al componente **gui texture**.
     *
     * Por ejemplo, usaremos:
     *
     @code
     CGameObject go1 = new CGameObject("go1");
     go1->GUITexture()->width; // Acceder a un atributo del componente.
     @endcode
     *
     * Si el componente no existe, será creado, y luego será retornado.
     *
     * Véase CComponent_GUI_Texture.
     *
     * @return Puntero al componente **gui texture**.
     */
    inline CComponent_GUI_Texture* GUITexture()
    {
      if(components.find(Components::gui_texture) == components.end())
        components.insert(std::pair<int, CComponent*>(Components::gui_texture, new CComponent_GUI_Texture(this)));

      return (CComponent_GUI_Texture*)components[Components::gui_texture];
    }

    /**
     * Acceso directo para el componente **audio source** (CComponent_Audio_Source).
     *
     * Accede de manera sencilla al componente **audio source**.
     *
     * Por ejemplo, usaremos:
     *
     @code
     CGameObject go1 = new CGameObject("go1");
     go1->AudioSource()->volume; // Acceder a un atributo del componente.
     @endcode
     *
     * Si el componente no existe, será creado, y luego será retornado.
     *
     * Véase CComponent_Audio_Source.
     *
     * @return Puntero al componente **gui texture**.
     */
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

//}

#endif /* __OBJECT_H_ */

//using namespace GO;
