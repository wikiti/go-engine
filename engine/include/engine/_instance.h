#ifndef __INSTANCE_H_
#define __INSTANCE_H_

#include "_globals.h"

class CInstance
{
  protected:
    friend class CEngine;

    bool i_running;
    int next_instance;

    fboolpointer gameObjects_loader;
    string resource_file;

    /** FPS **/
    GLfloat FPS;
    uint frames;
    Uint32 current_time, previous_time;

  public:
    CInstance(fboolpointer load_gameObject_function, string resource_file);
    //~CInstance();

    bool Init();
      bool LoadResources();

    void Close();
      void UnLoadResources(); // se descargarán los archivos cuando, al cargar los archivos de la proxima estancia, se demuestre que solo se descartaran los que no necesitan seguir en el motor

    int OnExecute();
    void OnLoop();
    void OnEvent();
    void OnRender();
    void CalculateFPS();

    GLfloat fps()
    {
      return FPS;
    }

    //virtual void OnDebug(){};
};

#endif /* __INSTANCE_H_ */
