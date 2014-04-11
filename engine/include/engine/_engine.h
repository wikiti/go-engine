/**
 * @file
 * @brief Fichero que incluye la clase CEngine, y su instancia global gEngine.
 */

#ifndef __ENGINE_H_
#define __ENGINE_H_

#include "_globals.h"
#include "engine/_instance.h"

#include "systems/_render.h"

/** @addtogroup Otros */
/*@{*/

/**
 * @brief Motor gráfico.
 *
 * Representa el conjunto de estancias del juego (los sistemas van aparte).
 * En este caso, el motor gráfico se encargará de gestionar la ejecución y flujo principal del
 * juego. En caso de un cambio de nivel o de estancia (CInstance), será esta clase la encargada de
 * gestionar dicho cambio.
 *
 * El objeto de esta clase (gEngine), será el encargado de gestionar las estancias (CInstance) o niveles.
 * Para ello, dispone de un mapa de estancias, cada una caracterizada por un nombre (string). Desde cualquier
 * estancia, sabiendo el nombre de alguna otra, se podrá pedir un cambio de estancia, simulando un
 * cambio de nivel, por lo que se cargarán nuevos recursos, nuevos objetos, se reiniciarán sistemas, etc.
 *
 * @note **Nota**: en caso de querer añadir soporte multihilo, solo bastaría con modificar parte e esta clase, y parte de CInstance,
 * @note por lo que no debería de haber grandes cambios.
 *
 * @warning Úsese el objeto gEngine para utilizar los métodos de este clase, para que repercutan en tiempo de ejecución.
 *
 * @see gEngine
 * @see CInstance
 */
class CEngine
{
  protected:
    //vector<CInstance*> instances;
    std::map<std::string, CInstance*> instances;
    std::string title;
    bool running;

    std::string current_instance;

    // Arguments
    std::vector<std::string> arguments;

  protected:
    void RemoveAllInstances();

  public:
    /**
     * @brief Constructor.
     *
     * Construye el motor gráfico, y pone de título a la ventana "GO-Engine".
     */
    CEngine();
    /**
     * @brief Destructor.
     *
     * Destruye el motor gráfico.
     */
    virtual ~CEngine(){};

    /**
     * @brief Iniciar el motor.
     *
     * Inicia el motor gráfico, por lo que iniciará *SDL2*, los sistemas definidos e iniciará las funciones
     * User_Engine_Loader() (icono, título, etc.) y User_Instance_Loader() (cargar estancias). Además, esta clase
     * conservará los argumentos pasados al programa. Será llamado al iniciar OnExecute().
     *
     * @param argc Cantidad de argumentos del programa. Se corresponde por el argumento definido por la función main().
     * @param argv Array de argumentos del programa. Se corresponde por el argumento definido por la función main().
     * @return Retorna true si se inicia correctamente, false en caso contrario.
     *
     * @see CSystem
     * @see User_Engine_Loader()
     * @see User_Instance_Loader()
     */
    virtual bool Init(int argc, char* argv[]);
    /**
     * @brief Cerrar el motor.
     *
     * Cierra el motor gráfico, cerrando la librería *SDL2*, cerrando los sitemas, y eliminando todas las estancias.
     * Será llamado al acabar OnExecute().
     */
    virtual void Close();

    /**
     * @brief Comenzar ejecución
     *
     * Comienza la ejecución del motor gráfico. Primero el motor con Init(), y luego ejecuta la primer estancia añadida con la función
     * AddInstance(), usada desde User_Instance_Loader(). Finalmente, después de haber ejecutado las estancias necesarias, se cerrará el motor, llamando
     * a la función close. Será llamado desde la función main().
     *
     * @param argc Cantidad de argumentos. Procedente de la función main().
     * @param argv Argumentos. Procedente de la función main().
     * @return Estado de ejecución. Retorna siempre 0.
     */
    virtual int OnExecute(int argc, char* argv[]);

    // Should be 32x32 RGBA format
    /**
     * @brief Cambiar icono del motor.
     *
     * Cambia el icono del motor gráfico por uno personalizado. Debe ser una imagen de tamaño 32x32, con formato RGBA, preferiblemente en formato tga.
     * Se debe especificar la ruta completa al icono, por lo general, debe estar en data/engine
     * @param icon_name
     */
    void SetIcon(std::string icon_name);

    /**
     * @brief Obtener título.
     *
     * Devuelve el título del motor gráfico (título de la ventana).
     *
     * @return String que contiene el título de la ventana.
     */
    std::string GetTitle()
    {
      return title;
    }

    /**
     * @brief Cambiar título.
     *
     * Cambia el título de la ventana de la aplicación.
     * @param text Nuevo título.
     */
    void SetTitle(const char* text)
    {
      if(text)
        title = text;

      SDL_SetWindowTitle(gSystem_Render.window, title.c_str());
    }

    /**
     * @brief Añadir una nueva estancia.
     *
     * Esta función añade una nueva estancia al motor gráfico, dada una función **callback** del formato
     @code
     bool callback(void);
     @endcode
     *
     * dado un nombre de estancia (identificador único) y la ruta a un fichero de recursos, con extensión .rc, que
     * contendrá los recursos necesarios para la estancia (sonidos, modelos, texturas...), generalmente en
     * la ruta <i>/data/resources</i>. El identificador de estancia será usado para hacer referencia a otras
     * estancias (cargar el "nivel2" desde el *"nivel1"*), y el callback se usará para **cargar los objetos de dicha estancia**.
     * Véanse los ejemplos de prueba de la carpeta "user".
     *
     * @warning La primera estancia añadida será la primera estancia cargada del programa.
     *
     * @param load_gameObject_function Callback de la función para cargar los objetos de juego. Debe ser del tipo *bool callback(void)*
     * @param resource_file Ruta al fichero de recursos a cargar para dicha estancia.
     * @param instance_name Nombre de la estancia. Debe ser un identificador único.
     *
     * @see user/include/gameObjects.h
     * @see user/src/gameObjects.cpp
     * @see user/include/instances.h
     * @see user/src/instances.cpp
     * @see CSystem_Resources
     */
    void AddInstance(fboolpointer load_gameObject_function, std::string resource_file, std::string instance_name);

    /**
     * @brief Fotogramas por segundo.
     *
     * Retorna la cantidad de fotogramas por segundo. Será calculado cada 10 milisegundos.
     * @return Fotogramas por segundo de los últimos 10 milisegundos.
     */
    GLdouble fps()
    {
      return instances[current_instance]->fps();
    }

    /**
     * @brief Instancia ejecutada actualmente.
     *
     * Devuelve un puntero del tipo CInstance que apunta a la estancia que está siendo ejecutada actualmente.
     * Es mejor usar GetCurrentInstance() si sólo se desea saber el nombre.
     *
     * @see CInstance
     * @return Puntero a la estancia ejecutada actualmente.
     */
    CInstance* CurrentInstance()
    {
      return instances[current_instance];
    }

    /**
     * @brief Argumentos del programa.
     *
     * Obtener una referencia a un vector de argumentos (string) pasados al programa como argumentos.
     * Puede ser de utilidad para ciertos casos (depuración, etc). Si un argumento empieza por un "-",
     * será ejecutado como un comando de consola después de haber cargado los sistemas, y antes de
     * haber cargado la primera estancia.
     *
     * @return Referencia a un vector de argumentos.
     */
    std::vector<std::string>& Arguments()
    {
      return arguments;
    }

    // Exits without saving.
    // Should never be used
    /**
     * @brief Abortar la ejecución.
     *
     * Aborta la ejecución del programa, terminando bruscamente en cuanto es llamada. Esto hace que los sistemas,
     * objetos, recursos... no sean liberados correctamente. Si se quiere terminar la ejecución correctamente,
     * use Exit().
     *
     * @warning No debe de usar esta función salvo que sea estrictamente necesario (fallos drásticos).
     */
    void Abort()
    {
      exit(0);
    }

    /**
     * @brief Termina la ejecución.
     *
     * Termina la ejecución del programa, terminando primero la ejecución de la estancia actual, para posteriormente
     * liberar recursos, cerrar sistemas, etc.
     */
    void Exit()
    {
      running = false;
      if(instances.find(current_instance) != instances.end())
        instances[current_instance]->i_running = false;
    }

    /**
     * @brief Nombre de estancia ejecutada actualmente.
     *
     * Devuelve el identificador de la estancia que se está ejecutando actualmente.
     * @return Nombre de la estancia ejecutada actualmente. En caso de que no haya ninguna cargada, devuelve "".
     */
    std::string GetCurrentInstance()
    {
      return current_instance;
    }

    /**
     * @brief Nombre de la estancia que será ejecutada después de la actual.
     *
     * Devuelve el identificador de la estancia que será ejecutada después de la actual.
     *
     * @return Nombre de la estancia que será ejecutada después que la actual. Si no se ha planeado ninguna con SetNextInstance, devolverá "".
     */
    std::string GetNextInstance()
    {
      if(instances.find(current_instance) != instances.end())
        return instances[current_instance]->next_instance;

      return "";
    }

    /**
     * @brief Cambiar de estancia.
     *
     * Cambia la próxima estancia a ejecutar, y finaliza la estancia actual. Básicamente, es un "cambio de nivel".
     *
     * @param instance Nombre de la nueva estancia a ejecutar. Debe existir dicha estancia.
     */
    void NextInstance(std::string instance)
    {
      if(instances.find(current_instance) != instances.end())
      {
        instances[current_instance]->NextInstance(instance);
      }
    }

    /**
     * @brief Cambiar de estancia.
     *
     * Cambia la próxima estancia a ejecutar, pero **no** finaliza la estancia actual.
     *
     * @param instance Nombre de la próxima estancia a ejecutar. Debe existir dicha estancia.
     */
    void SetNextInstance(std::string instance)
    {
      if(instances.find(current_instance) != instances.end())
        instances[current_instance]->SetNextInstance(instance);
    }

    /**
     * @brief Terminar la estancia actual.
     *
     * Finaliza la ejecución de la estancia actual. Si se ha definido una próxima estancia con
     * SetNextInstance(), se ejecutará dicha estancia. En caso contrario, finalizará la ejecución del motor gráfico.
     */
    void EndInstance()
    {
      instances[current_instance]->EndInstance();
    }
};

/**
 * @brief Objeto global del motor gráfico.
 *
 * Para referirse al motor, basta con usar este objeto. Debe ser el único objeto de su tipo.
 *
 * @see CEngine
 */
extern CEngine gEngine;

/*@}*/

#endif /* __ENGINE_H_ */
