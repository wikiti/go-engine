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

    virtual int OnExecute(int argc, char* argv[]);

    // Should be 32x32 RGBA format
    void SetIcon(std::string icon_name);

    std::string GetTitle()
    {
      return title;
    }

    void SetTitle(const char* text)
    {
      if(text)
        title = text;

      SDL_SetWindowTitle(gSystem_Render.window, title.c_str());
    }

    void AddInstance(fboolpointer load_gameObject_function, std::string resource_file, std::string instance_name);

    GLdouble fps()
    {
      return instances[current_instance]->fps();
    }

    CInstance* CurrentInstance()
    {
      return instances[current_instance];
    }

    std::vector<std::string>& Arguments()
    {
      return arguments;
    }

    // Exits without saving.
    // Should never be used
    void Exit()
    {
      exit(0);
    }

    void Quit()
    {
      running = false;
      if(instances.find(current_instance) != instances.end())
        instances[current_instance]->i_running = false;
    }

    std::string GetCurrentInstance()
    {
      return current_instance;
    }

    std::string GetNextInstance()
    {
      if(instances.find(current_instance) != instances.end())
        return instances[current_instance]->next_instance;

      return "";
    }

    void NextInstance(std::string instance)
    {
      if(instances.find(current_instance) != instances.end())
      {
        instances[current_instance]->NextInstance(instance);
      }
    }

    void SetNextInstance(std::string instance)
    {
      if(instances.find(current_instance) != instances.end())
        instances[current_instance]->SetNextInstance(instance);
    }

    void EndInstance()
    {
      instances[current_instance]->EndInstance();
    }
};

extern CEngine gEngine;

/*@}*/

#endif /* __ENGINE_H_ */
