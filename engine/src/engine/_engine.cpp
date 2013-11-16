#include "engine/_engine.h"
#include "engine/_instance.h"
#include "_systems.h"

// User
#include "instances.h"

CEngine gEngine;

CEngine::CEngine(): title(""), running(false)
{

}

bool CEngine::Init()
{
  if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load SDL module");
    return false;
  }

  /** Mas módulos y librerías de SDL aquí **/

  if(!Systems_Init())
  {
    return false;
  }

  if(!User_Engine_Loader())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load engine settings (User_Engine_Loader)");
    return false;
  }

  if(!User_Instance_Loader())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load instance settings (User_Instance_Loader)");
    return false;
  }

  current_instance = 0;
  running = true;

  return true;
}


void CEngine::Close()
{
  RemoveAllInstances();
  Systems_Close();

  SDL_Quit();
}

int CEngine::OnExecute()
{
  Init();

  while(current_instance >= 0 && running)
    current_instance = instances[current_instance]->OnExecute();

  Close();

  return current_instance;
}

void CEngine::AddInstance(fboolpointer load_gameObject_function, string resource_file)
{
  CInstance* new_instance = new CInstance(load_gameObject_function, resource_file);
  instances.push_back(new_instance);
}

void CEngine::RemoveAllInstances()
{
  for(vector<CInstance*>::iterator it = instances.begin(); it != instances.end(); it++)
  {
    delete *it;
  }
}

void CEngine::SetIcon(string icon_dir)
{
  SDL_Surface* icon = sdl_cargar_img(icon_dir);
  SDL_SetWindowIcon(gRender.window, icon);
  SDL_FreeSurface(icon);
}
