#include "engine/_engine.h"
#include "engine/_instance.h"
#include "_systems.h"

// User
#include "instances.h"

CEngine gEngine;

using namespace std;

CEngine::CEngine(): title(""), running(false)
{

}

bool CEngine::Init(int argc, char* argv[])
{
  for(int i = 0; i < argc; i++)
    arguments.push_back(string(argv[i]));

  current_instance = "";
  running = true;

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

  return true;
}


void CEngine::Close()
{
  RemoveAllInstances();
  Systems_Close();

  SDL_Quit();
}

int CEngine::OnExecute(int argc, char* argv[])
{
  Init(argc, argv);

  while(current_instance != "" && running)
  {
    if(instances.find(current_instance) != instances.end())
      current_instance = instances[current_instance]->OnExecute();
    else  { // Error y salir
      gSystem_Debug.error("From CEngine::OnExecute: Error, instance \"%s\" not defined.", current_instance.c_str());
      Quit();
    }
    // Resetear contenido de los sistemas.
    Systems_Reset();
  }

  Close();

  return 0;
}

void CEngine::AddInstance(fboolpointer load_gameObject_function, string resource_file, string instance_name)
{
  if(instances.find(instance_name) == instances.end()) {
    CInstance* new_instance = new CInstance(load_gameObject_function, resource_file);
    instances[instance_name] = new_instance;

    if(current_instance == "")
      current_instance = instance_name;
  }
  else {
    gSystem_Debug.error("From CEngine::AddInstance: Cannot add instance with name \"%s\" (existing name).", instance_name.c_str());
  }

  //instances.push_back(new_instance);
}

void CEngine::RemoveAllInstances()
{
  for(map<string, CInstance*>::iterator it = instances.begin(); it != instances.end(); ++it)
  {
    delete it->second;
  }

  instances.clear();
}

void CEngine::SetIcon(string icon_dir)
{
  SDL_Surface* icon = GO_Utils::sdl_cargar_img(icon_dir);
  SDL_SetWindowIcon(gRender.window, icon);
  SDL_FreeSurface(icon);
}
