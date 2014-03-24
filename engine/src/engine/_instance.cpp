#include "engine/_instance.h"
#include "_globals.h"
#include "_systems.h"

#include "gameObjects.h"

CInstance::CInstance(fboolpointer f, string r): i_running(true), next_instance(""), gameObjects_loader(f), resource_file(r)
{

}

bool CInstance::Init()
{
  if(!LoadResources())
  {
    return false;
  }

  next_instance = "";

  if(!gameObjects_loader)
  {
    return false;
  }

  gameObjects_loader();

  FPS = frames = 0;
  current_time = previous_time = 0;

  i_running = true;
  first_frame = true;

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

  i_running = false;

  // Aquí habría que borrar objetos y tal

}
void CInstance::UnLoadResources()
{

}

string CInstance::OnExecute()
{
  //gDebug.console_info_msg("Running instance: \"%s\"", gEngine.GetCurrentInstance().c_str());
  Init();

  while(i_running)
  {
    // Disable user input on first iteration
    if(!first_frame) OnEvent();
    OnLoop();
    OnRender();

    CalculateFPS();
    first_frame = false;
    //cout << "FPS: " << fps() << endl;
  }

  Close();

  return next_instance;
}

void CInstance::OnLoop()
{
  gSystem_UserInput.OnLoop();
  gSystem_Time.OnLoop();
  gSystem_Mixer.OnLoop();
  gSystem_GameObject_Manager.OnLoop();
  gSystem_Render.OnLoop();
}

void CInstance::OnEvent()
{
  while(SDL_PollEvent(&event))
  {
    if(event.type == SDL_QUIT)
    {
      i_running = false;
      next_instance = "";

      SDL_StopTextInput();
    }

    gSystem_Debug.OnEvent();
    gSystem_UserInput.OnEvent();
    if(!gSystem_Debug.IsConsole()) gSystem_GameObject_Manager.OnEvent();
  }

  gSystem_UserInput.OnInput();
  if(!gSystem_Debug.IsConsole()) gSystem_GameObject_Manager.OnInput();
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

  //FPS = 1/(timeInterval/1000.f);
  //previous_time = current_time;

  if(timeInterval > 10)
  {
    FPS = frames / (timeInterval / 1000.0f);
    previous_time = current_time;
    frames = 0;
  }
}

void CInstance::NextInstance(string next_instance_name)
{
  SetNextInstance(next_instance_name);
  EndInstance();
}

void CInstance::SetNextInstance(string next_instance_name)
{
  next_instance = next_instance_name;
}

void CInstance::EndInstance()
{
  i_running = false;
  gDebug.console_info_msg("Preparing to run next instance: \"%s\"", next_instance.c_str());
}

