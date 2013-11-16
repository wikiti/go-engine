#ifndef __RENDER_H_
#define __RENDER_H_

#include "_object.h"
#include "_globals.h"
#include "systems/_system.h"

class CSystem_Render: public CSystem
{
  private:
    friend class CSystem_Debug;
    friend class CEngine;

    SDL_Window* window;
    SDL_GLContext GLcontext;
    vector<CGameObject*> camera_list;

    inline void ResizeWindow(int w, int h)
    {
      SDL_SetWindowSize(window, w, h);
      ApplyCameraChanges();
    }

    inline void ApplyCameraChanges()
    {
      for(vector<CGameObject*>::iterator it = camera_list.begin(); it != camera_list.end(); it++)
      {
        (*it)->camera()->ApplyChanges();
      }
    }

  public:
    CSystem_Render(): window(NULL){ };

    virtual bool Init();
    virtual void Close();

    // Usar booleanos (o algo)

    void SetMainCamera(CGameObject* camera);
    void SetMainCamera(const string& camera);

    void UnSetMainCamera(const string& camera); // si la lista de camaras esta vacia, no se renderiza nada
    // camera_list se usa como una cola

    //void SetFullscreenMode(bool state = true);

    void AddCamera(CGameObject* camera);
    void AddCamera(const string& camera);
    void AddCameraPrior(CGameObject* camera);
    void AddCameraPrior(const string& camera); // ?

    void RemoveCamera(const string& camera);

    //inline void RenderGameObject(CGameObject* go);
    void OnLoop();
    void OnRender();
      void RenderGrid(int rows = 20, int cols = 20);
      inline void Clear();
      inline void RenderToScreen()
      {
        SDL_GL_SwapWindow(window);
        // http://wiki.libsdl.org/MigrationGuide#OpenGL
      }


    inline void GetWindowSize(int* w, int* h)
    {
      SDL_GetWindowSize(window, w, h);
    }

    //inline void ResizeWindow(int w,, int h);

};

extern CSystem_Render gSystem_Render;
extern CSystem_Render& gRender;


#endif /* __RENDER_H_ */
