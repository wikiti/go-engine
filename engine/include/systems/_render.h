#ifndef __RENDER_H_
#define __RENDER_H_

#include "_object.h"
#include "systems/_system.h"

#define __RENDER_OPENGL_MIN_CORE "3.3.0"

// Problema grave: alphablending
// http://blogs.msdn.com/b/shawnhar/archive/2009/02/18/depth-sorting-alpha-blended-objects.aspx
// http://stackoverflow.com/questions/5793354/how-to-write-prevent-writing-to-opengl-depth-buffer-in-glsl
/*
 * Parece que el "algoritmo" será:
 *
 * Para cada objeto a dibujar:
 *   Si el objeto es transparente, añadir en el vector "lista_transparente", ordenando por la distancia hacia la cámara
 *   Si no es transparente, dibujar el objeto
 *
 * Desactivar la escritura al depth buffer
 * Para cada objeto en lista_transparente
 *     dibujar objetos de atrás hacia delante
 */

namespace GO_Render
{
  enum window_display_t {windowed = 0, fullscreen = SDL_WINDOW_FULLSCREEN, fullwindowed = SDL_WINDOW_FULLSCREEN_DESKTOP};
  enum opengl_core_support_t { not_supported = 0, in_core, arb_extension };
}

class CSystem_Render: public CSystem
{
  private:
    friend class CSystem_Debug;
    friend class CEngine;
    friend class CInstance;

    // OpenGL info
    vector<string> GLInfo;
    void SetGLInfo();

    // Draw primitives
    GLUquadricObj *quadratic;

    SDL_Window* window;
    SDL_GLContext GLcontext;
    vector<CGameObject*> camera_list;
    CGameObject* GUI_Camera;

    int current_camera;

    // Skybox VBO
    GLuint m_SkyboxVBOVertices;                     // Vertex VBO Name
    GLuint m_SkyboxVBOTexCoords;                    // Texture Coordinate VBO Name
    GLuint m_SkyboxVAO;

    // Grid VBO
    GLuint m_GridVBOVertices;                     // Vertex VBO Name
    GLuint m_GridVBOColors;                       // Vertex VBO Name
    GLuint m_GridVBO_numcols;
    GLuint m_GridVBO_numrows;
    GLuint m_GridVAO;

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

    inline void GetMaxSamples(int& enabled, int& max_value)
    {
      SDL_Window* l_window = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
      if(!l_window)
      {
        gSystem_Debug.error("From CSystem_Render: Could not create window for multisampling: %s", SDL_GetError());

        enabled = 0;
        max_value = 0;
      }

      SDL_GLContext l_GLcontext = SDL_GL_CreateContext(l_window);

      glEnable(GL_MULTISAMPLE);

      if(SDL_GL_ExtensionSupported("GL_ARB_multisample"))
      {
        glGetIntegerv(GL_MAX_SAMPLES, &max_value);

        enabled = 1;
        max_value /= 2;
      }
      else
      {
        enabled = 0;
        max_value = 0;
      }

      SDL_GL_DeleteContext(l_GLcontext);
      SDL_DestroyWindow(l_window);
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
      bool InitSkyboxVBO();
      bool InitGridVBO();
        void UpdateGridVBO(int ncols, int nrows);
    virtual void Close();
    bool Reset();

    inline void ResizeWindow(int w, int h)
    {
      if(w > 0 and h > 0)
      {
        SDL_SetWindowSize(window, w, h);
        ApplyCameraChanges();
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
      }
    }

    inline void SetFullScreenWindow(GO_Render::window_display_t mode)
    {
      if(mode == GO_Render::windowed or mode == GO_Render::fullscreen or mode == GO_Render::fullwindowed)
      {
        SDL_SetWindowFullscreen(window, mode);
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
      }
    }

    vector<string> GetGLInfo()
    {
      return GLInfo;
    }

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
  protected:
    void OnLoop();
    void OnRender();
      void RenderGrid(CComponent_Camera* cam);
      bool RenderSkybox(CComponent_Camera* cam);
      inline void Clear();
      inline void RenderToScreen()
      {
        SDL_GL_SwapWindow(window);
        // http://wiki.libsdl.org/MigrationGuide#OpenGL
      }

  public:

    inline void GetWindowSize(int* w, int* h)
    {
      SDL_GetWindowSize(window, w, h);
    }

    void RenderSphere(GLdouble radius, GLint slices = 10, GLint stacks = 10)
    {
      gluSphere(quadratic, radius, slices, stacks);
    }

#define __GL_CHECK_ERRORS() CSystem_Render::_check_gl_error(__FILE__, __LINE__)

    void _check_gl_error(const char *file, int line)
    {
      GLenum error(glGetError());

      while(error != GL_NO_ERROR)
      {
        gSystem_Debug.console_error_msg("From CSystem_Render: OpenGL error %d: %s - %s:%d", error, gluErrorString(error), __FILE__, __LINE__);
        gSystem_Debug.error("From CSystem_Render: OpenGL error %d: %s - %s:%d", error, gluErrorString(error), __FILE__, __LINE__);

        error = glGetError();
      }
    }

    //inline void ResizeWindow(int w,, int h);

};

extern CSystem_Render gSystem_Render;
extern CSystem_Render& gRender;


#endif /* __RENDER_H_ */
