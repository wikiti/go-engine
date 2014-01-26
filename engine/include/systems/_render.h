#ifndef __RENDER_H_
#define __RENDER_H_

#include "_object.h"
#include "systems/_system.h"

class CSystem_Render: public CSystem
{
  private:
    friend class CSystem_Debug;
    friend class CEngine;

    // Draw primitives
    GLUquadricObj *quadratic;

    SDL_Window* window;
    SDL_GLContext GLcontext;
    vector<CGameObject*> camera_list;
    CGameObject* GUI_Camera;

    int current_camera;

    // Skybox VBO
    unsigned int    m_nVBOVertices;                     // Vertex VBO Name
    unsigned int    m_nVBOTexCoords;                    // Texture Coordinate VBO Name

    //bool multitexture_supported;
    //bool vbos_supported;

    /*
    struct gameObject_Render_count_t
    {
      CGameObject* go;
      uint count;
    }
    map<string, gameObject_Render_count_t> r_gameObjects; // <- ¡¡ESTO!!

    bool AddToRender(CGameObject* go)
    {
      if(!go) return false;

      map<string, gameObject_Render_count_t>::iterator it = r_gameObjects.find(go->GetName());
      if(it != r_gameObjects.end()) // Ya existe, no se añade
        return false;
      else
      {
        string go_name = go->GetName();
        r_gameObjects.insert(pair<string, gameObject_Render_count_t>(go_name, new gameObject_Render_count_t);
        r_gameObjects[go_name]->go = go;
      }

    }

    */

    //CComponent_Camera* GUI_Camera;

    inline void ResizeWindow(int w, int h)
    {
      SDL_SetWindowSize(window, w, h);
      ApplyCameraChanges();
    }

    inline void ApplyCameraChanges()
    {
      for(vector<CGameObject*>::iterator it = camera_list.begin(); it != camera_list.end(); it++)
      {
        (*it)->Camera()->ApplyChanges();
      }

      GUI_Camera->Camera()->ApplyChanges();
    }

  public:
    CSystem_Render(): CSystem(), window(NULL){ };

    virtual bool Init();
      void InitSkyboxVBO();
    virtual void Close();

    // Usar booleanos (o algo)
    SDL_Window* GetWindow()
    {
      return window;
    }

    CGameObject* GetCurrentCamera()
    {
      if(current_camera < 0 || !camera_list.size() )
        return NULL;

      return camera_list[current_camera];
    }

    CGameObject* GetMainCamera()
    {
      if(!camera_list.size() )
        return NULL;

      return camera_list[0];
    }

    int GetCurrentCameraIndex()
    {
      return current_camera;
    }

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
      bool DrawSkybox(CComponent_Camera* cam);
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

    void RenderSphere(GLdouble radius, GLint slices = 10, GLint stacks = 10)
    {
      gluSphere(quadratic, radius, slices, stacks);
    }

    //inline void ResizeWindow(int w,, int h);

};

extern CSystem_Render gSystem_Render;
extern CSystem_Render& gRender;


#endif /* __RENDER_H_ */
