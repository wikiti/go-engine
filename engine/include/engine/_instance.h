#ifndef __INSTANCE_H_
#define __INSTANCE_H_

#include "_globals.h"

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

  public:
    CInstance(fboolpointer load_gameObject_function, std::string resource_file);
    //~CInstance();

    bool Init();
      bool LoadResources();

    void Close();
      void UnLoadResources(); // se descargarán los archivos cuando, al cargar los archivos de la proxima estancia, se demuestre que solo se descartaran los que no necesitan seguir en el motor

      std::string OnExecute();
    void OnLoop();
    void OnEvent();
    void OnRender();
    void CalculateFPS();

    GLdouble fps()
    {
      return FPS;
    }

    void NextInstance(std::string next_instance_name);
      void SetNextInstance(std::string next_instance_name);
      void EndInstance();

    //virtual void OnDebug(){};
};

#endif /* __INSTANCE_H_ */
