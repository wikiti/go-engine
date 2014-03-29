/**
 * @file
 * @brief Fichero que incluye la clase abstracta CComponent para definir componentes.
 */

#ifndef __COMPONENT_H_
#define __COMPONENT_H_

#include "_globals.h"

/** @addtogroup Componentes */
/*@{*/

/**
 * @brief Componentes.
 *
 * Espacio de nombres para guardar enumeraciones, variables y métodos referentes a los componentes.
 *
 * @warning Esto debería en otro sitio más generico, no en el fichero de definición del componente abstracto.
 */
namespace Components // ->Nota El namespace Components debería ir en un lugar más genérico.
{
  /**
   * @brief Enumeración o identificadores de componentes.
   *
   * Enum para identificar a todos los tipos de componentes. Cada clase heredada de CComponent debe tener un valor en esta enumeración.
   */
  enum components_t { base = 0, camera, mesh_render, particle_emitter, gui_texture, audio_source, transform, dummy1, dummy2, dummy3, __component_not_defined};

  /**
   * @brief Nombres de componentes.
   *
   * Array de strings que contienen, por cada enum de components_t, una representación con caracteres.
   * La representación como cadena de caracteres será idéntica al nombre del valor del enum, salvo para el caso especial components_t.__component_not_defined, que será *"not_defined"*.
   */
  extern const char* components_s[];

  /**
   * @brief Transformar un componente en una string.
   *
   * Dado un enum de un componente, transforma dicho valor a un string.
   * @param c Valor del enum.
   * @return Representación textual del componente.
   */
  const char* component_to_string(components_t c);

  /**
   * @brief Transformar un string a un enum de un componente.
   *
   * Dado un nombre, trata de transformar dicho nombre a una enum de componentes.
   * @param c Nombre del componente.
   * @return En caso de encontrar un componente, devuelve su valor de enum. En caso contrario, devuelve components_t.__component_not_defined.
   */
  components_t string_to_component(const std::string& c);
}

class CGameObject;

/**
 * @brief Puntero a función miembro de un game object.
 *
 * El puntero se usará para funciones del tipo "void f(CGameObject* g)".
 *
 * @see CGameObject::start
 * @see CGameObject::behaviour
 * @see CGameObject::event_behaviour
 * @see CGameObject::input_behaviour
 * @see CGameObject::render
 *
 * Se usará para guardar en el game object una serie de punteros a funciones externas para ser llamadas en determinados momentos,
 * usando como referencia al objeto el parámetro "self".
 *
 * @param self Referencia a un objeto dado.
 */
typedef void (*function_t)(CGameObject* self);

/**
 * @brief Clase abstracta de un componente.
 *
 * Se usará para representar y hacer de base a la hora de definir un componente.
 *
 * @see CGameObject.
 *
 * Un componente estará asociado a un único objeto de juego, ignorando completamente componentes compartidos (imposible implementarlos por la asociación bidireccional entre objeto y componente).
 *
 * Con respecto a la asociación de los componentes con un objeto de juego, usamos polimorfimos para ello.
 * De cara a poder tener una gran variedad de componentes, sería lógico usar un vector de “componentes” (o CComponentes) con el fin de almacenar nuestros componentes únicos en un Game Object.
 *
 * @image html components/componentes.png
 *
 * Como se puede apreciar, cada componente tiene 3 partes:
 *
 * - Contenido o atributos del componente.
 * - Métodos del componente.
 * - Identificador único para ese componente.
 *
 * Para facilitar el acceso, puesto que un objeto de nuestro juego podría llegar a tener una cantidad elevada de componentes (dependiendo del número de componentes implementados),
 * hemos decidido usar un std::map de la librería estándar de plantillas de C++. Además, como se ha mencionado en el post anterior, un objeto sólo puede tener un componente de cada tipo.
 * En este caso, para nuestro mapa, las claves serían unos identificadores numéricos, mientras que el valor contenido serán punteros a dichos componentes,
 * que a su vez guardan un objeto al puntero que los contiene, intentando garantizar que los componentes no puedan ser compartidos
 * (tal vez interese implementar algo por el estilo más adelante).
 * Por tanto, tendremos un std::map con una lista de punteros del tipo CComponent que, por medio de polimorfismo y herencia,
 * crearemos una gama de clases hijas con distintas propiedades y ejecuciones de métodos distintos. Se pueden realizar una serie de operaciones sobre los componentes desde
 * los Game Objects, entre ellas, las siguientes:
 *
 @code
    template
    Type* GetComponent();

    template
    bool AddComponent();

    template
    bool RemoveComponent();

    template
    void SetComponentStateComponent(bool state);

    // Funciones para acceso directo a los componentes:
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

  @endcode
  *
  * - Obtener componente: Devuelve un puntero al componente del tipo seleccionado. Si no existe, devolverá NULL.
  * - Añadir componente:  Intenta añadir un componente. Si no existe, se creará y devolverá true. Si existe, no se hará nada y devolverá false.
  * - Eliminar componente:  Intenta eliminar un componente. Si existe, lo elimina y devuelve true. Si no existe, no se hará nada y devolverá false.
  * - Cambiar estado:  Cambia el estado del componente (activado o desactivado) en función del argumento state.
  * - Accesos directos: Permiten acceder de forma rápida y cómoda al componente. Si el componente no existe, lo crea.
  *
  * Además, se creará un acceso directo en la clase GameObject (por ejemplo, CComponente_Transform* Transform() )
  * para cada componente, declarado, que actuará de la siguiente forma: si el componente existe, devuelve un puntero.
  * Si no, crea el componente y lo devuelve. Así podemos garantizar que no se produzcan accesos indebidos a memoria por no comprobar la existencia de los componentes.
  *
  * @warning Un componente debe crearse usando los métodos expuestos en la clase CGameObject. En principio, no deberían usarse de manera explícita a la hora de operar (no hacemos un *new* de un componente).
 */
class CComponent
{
  friend class CGameObject;
  friend class CSystem_Debug;

  friend const char* Components::component_to_string(components_t c);
  friend Components::components_t  Components::string_to_component(const std::string& c);

  protected:
    bool enabled;
    CGameObject* gameObject;

    virtual void parseDebug(std::string command);
    virtual void printDebug();

  private:
    static int GetID() { return Components::base; }

    /*friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & enabled;
      //ar & id;
    }*/

  public:
    /**
     * @brief Constructor.
     */
    CComponent(){};
    /** Constructor con objeto asociado.
     *
     * Asocia al objeto pasado como argumento el componente creado.
     * @param container Objeto que guardará el componente.
     */
    CComponent(CGameObject* container);

    /**
     * @brief Destructor.
     */
    virtual ~CComponent();

    /** @no_use */
    virtual void Set(input_t data){};
    /** @no_use */
    virtual output_t Get(){return NULL;};

    /**
     * @brief Renderizar el componente.
     *
     * Función de renderizado. Renderiza el componente.
     *
     * @see CGameObject::OnRender().
     * @param projMatrix       Matriz de proyección. Interna al sistema.
     * @param modelViewMatrix  Matriz de modelview. Interna al sistema.
     *
     * @warning Esta función no debe ser llamada de manera explícita.
     */
    inline virtual void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix){};
    /**
     * @brief Gestión del comportamiento.
     *
     * Función de comportamiento. Actualiza el componente.
     *
     * @see CGameObject::OnLoop().
     *
     * @warning Esta función no debe ser llamada de manera explícita.
     */
    inline virtual void OnLoop(){};
    /**
     * @brief Gestión de eventos.
     *
     * Función de gestión de eventos. Gestiona eventos de entrada importantes para el componente.
     *
     * @see CGameObject::OnEvent().
     *
     * @warning Esta función no debe ser llamada de manera explícita.
     */
    inline virtual void OnEvent(){};
    /**
     * @brief Gestión de entrada.
     *
     * Función de gestión de entrada. Gestiona entradas importantes para el componente.
     *
     * @see CGameObject::OnInput().
     *
     * @warning Esta función no debe ser llamada de manera explícita.
     */
    inline virtual void OnInput(){};

    //virtual bool AddFuncs(CGameObject* obj){return true;};
    //virtual bool RemoveFuncs(CGameObject* obj){return true;};

    /**
     * @brief Getter del estado del componente.
     *
     * Un componente puede estar marcado como **activado** como **desactivado**. En el primer caso, el componente se tratará con normalidad, mientras
     * que en el segundo caso, en la mayoría de componentes, se hará como que el componente no existe.
     *
     * @warning En algunos componentes es necesario que se actualicen algunos valores, por lo que el componente seguirá ejecutando ciertas funciones aunque esté desactivado.
     *
     * @return Retorna true si está activado, false en caso contrario.
     */
    inline bool GetState()
    {
      return enabled;
    }

    /**
     * @brief Cambiar el estado del componente.
     *
     * Cambia el estado del componente actual por el estado especificado por "state" (true -> **activado**, false -> **desactivado**.
     *
     * @param state Nuevo estado del componente.
     */
    inline void SetState(bool state = true)
    {
      enabled = state;
    }

    /**
     * @brief Activa el componente.
     *
     * Cambia el estado del componente actual por **activado**.
     */
    inline void Enable()
    {
      enabled = true;
    }

    /**
     * @brief Desactiva el componente.
     *
     * Cambia el estado del componente actual por **desactivado**.
     */
    inline void Disable()
    {
      enabled = false;
    }

    /**
     * @brief Obtener el gameObject que encapsula este componente.
     *
     * Existe una relación bidireccional entre los gameObjects y los componentes que tienen. Por tanto, puede ser de utilidad obtener
     * el objeto que tiene un componente actualmente, ya sea para acceder a algunos atributos del objeto en cuestión, o para acceder a otro componente.
     * @return Devuelve un puntero a un gameObject que tenga el componente. No debería ser NULL en ningún momento.
     */
    inline CGameObject* GetGameObject()
    {
      return gameObject;
    }
};

/*@}*/

#endif /* __COMPONENT_H_ */
