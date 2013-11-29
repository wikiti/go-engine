#include "engine/_instance.h"
#include "_globals.h"
#include "_systems.h"

#include "gameObjects.h"

CInstance::CInstance(fboolpointer f, string r): i_running(true), next_instance(0), gameObjects_loader(f), resource_file(r)
{

}

bool CInstance::Init()
{
  if(!LoadResources())
  {
    return false;
  }

  if(!gameObjects_loader)
  {
    return false;
  }

  gameObjects_loader();

  FPS = frames = 0;
  current_time = previous_time = 0;

  return true;
}

bool CInstance::LoadResources()
{
  if(!gSystem_Resources.LoadResourceFile(resource_file))
    return false;

  return true;
}

void CInstance::Close()
{
  UnLoadResources();

  // Aquí habría que borrar objetos y tal

}
void CInstance::UnLoadResources()
{

}

int CInstance::OnExecute()
{
  Init();

  while(i_running)
  {
    OnEvent();
    OnLoop();
    OnRender();

    CalculateFPS();
    //cout << "FPS: " << fps() << endl;
  }

  Close();

  return next_instance;
}
void CInstance::OnLoop()
{
  gSystem_Time.OnLoop();
  gSystem_GameObject_Manager.OnLoop();
  gSystem_Render.OnLoop();
  gSystem_Mixer.OnLoop();
}

void CInstance::OnEvent()
{
  while(SDL_PollEvent(&event))
  {
    if(event.type == SDL_QUIT)
    {
      i_running = false;
      next_instance = -1;

      SDL_StopTextInput();
    }

    gSystem_Debug.OnEvent();
    if(!gSystem_Debug.IsConsole()) gSystem_GameObject_Manager.OnEvent();
  }

  if(!gSystem_Debug.IsConsole()) gSystem_GameObject_Manager.OnKeyEvent();
}

void CInstance::OnRender()
{
  gSystem_Render.OnRender();

  // Esto debería ir en gSystem_Render
  if(gSystem_Debug.IsConsole()) gSystem_Debug.OnRender();
  gSystem_Render.RenderToScreen();
}

void CInstance::CalculateFPS()
{
  frames++;

  current_time = SDL_GetTicks();
  int timeInterval = current_time - previous_time;

  if(timeInterval > 1000)
  {
     FPS = frames / (timeInterval / 1000.0f);
     previous_time = current_time;
     frames = 0;
  }
}
