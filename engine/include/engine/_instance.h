/**
 * @file
 * @brief Fichero que incluye la clase CInstance.
 */

#ifndef __INSTANCE_H_
#define __INSTANCE_H_

#include "_globals.h"

/** @addtogroup Otros */
/*@{*/

/**
 * @brief Clase que representa a una estancia o nivel.
 *
 * Una estancia o nivel representa un escenario concreto de nuestra aplicación o nivel.
 * Por ejemplo, supóngase el caso en el que nuestro juego esta compuesto de varios subjuegos (3 en raya,
 * ajedrez, póker...). Cada juego podría corresponderse con una estancia, y el menú de selección de juego
 * también podría considerarse una estancia o nivel.
 *
 * Así pues, una estancia de componene de:
 * - Una serie de objetos de juego, cargados desde el callback de carga de objetos (véase CInstance() ).
 * - Una serie de recursos, sean texturas, modelos, sonidos... cargados directamente desde un fichero de recursos *.rc*.
 * - Una próxima estancia a ejecutar (si no hay próxima, finaliza el juego).
 *
 * Sin más, el motor gráfica u objeto gEngine se encargará de gestionar las estancias, de manera que el usuario sólo
 * tiene que definirlas, definir la próxima estancia de cada una (se puede hacer de manera dinámica) y estblecer un
 * fichero de recursos a cargar.
 *
 * @see CEngine
 * @see gEngine
 *
 */
class CInstance
{
  protected:
    friend class CEngine;

    bool i_running;
    std::string next_instance;

    fboolpointer gameObjects_loader;
    std::string resource_file;

    /** FPS **/
    GLdouble FPS;
    uint frames;
    Uint32 current_time, previous_time;

    bool first_frame;

    bool Init();
      bool LoadResources();

    void Close();
      void UnLoadResources(); // se descargarán los archivos cuando, al cargar los archivos de la proxima estancia, se demuestre que solo se descartaran los que no necesitan seguir en el motor

      std::string OnExecute();
    void OnLoop();
    void OnEvent();
    void OnRender();
    void CalculateFPS();

  public:
    /**
     * @brief Constructor principal.
     *
     * Crea una estancia. No debe llamarse a este constructor de manera dinámica, sino que
     * debe ser creada por el método CEngine::AddInstance(). Se caracteriza porque se le proporciona
     * un callback de los objetos a crear, y un fichero de recursos.
     *
     * @param load_gameObject_function
     * @param resource_file
     */
    CInstance(fboolpointer load_gameObject_function, std::string resource_file);
    //~CInstance();

    GLdouble fps()
    {
      return FPS;
    }

    void NextInstance(std::string next_instance_name);
      void SetNextInstance(std::string next_instance_name);
      void EndInstance();

    //virtual void OnDebug(){};
};

/*@}*/

#endif /* __INSTANCE_H_ */
