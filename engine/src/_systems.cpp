#include "_systems.h"

bool Systems_Init()
{
  if(!gSystem_Debug.Init())
  {
    gSystem_Debug.msg_box(ERROR_INIT, "Error cargando sistema Debug");
  }

  if(!gSystem_Data_Storage.Init())
  {
    gSystem_Debug.msg_box(ERROR_INIT, "Error cargando sistema Storage");
  }

  if(!gSystem_GameObject_Manager.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Error cargando sistema GameObject Manager");
    return false;
  }

  if(!gSystem_Render.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Error cargando sistema Render");
    return false;
  }

  if(!gSystem_Debug.InitConsoleFont())
  {
    gSystem_Debug.msg_box(ERROR_INIT, "Error cargando sistema Debug::Console");
  }

  if(!gSystem_Resources.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Error cargando sistema de Recursos");
    return false;
  }

  if(!gSystem_Time.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Error cargando sistema Time");
    return false;
  }

  if(!gSystem_Math.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Error cargando sistema Time");
    return false;
  }

  return true;
}

void Systems_Close()
{
  gSystem_Data_Storage.Close();
  gSystem_GameObject_Manager.Close();
  gSystem_Render.Close();
  gSystem_Debug.Close();
  gSystem_Resources.Close();
  gSystem_Time.Close();
  gSystem_Math.Close();
}

void Systems_OnEvent()
{

}

void Systems_OnKeyEvent()
{

}

void Systems_OnLoop()
{

}

void Systems_OnRender()
{

}

