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
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load GameObject Manager system");
    return false;
  }

  if(!gSystem_Render.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load Render system");
    return false;
  }

  // pasar esto a System_Resources.Init()!!
  if(!gSystem_Debug.InitConsoleFont())
  {
    gSystem_Debug.msg_box(ERROR_INIT, "Could not load Debug::Console system");
  }

  if(!gSystem_Mixer.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load Mixer system");
    return false;
  }

  if(!gSystem_Resources.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load de Recursos system");
    return false;
  }

  if(!gSystem_Time.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load Time system");
    return false;
  }

  if(!gSystem_Math.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load Math system");
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
  gSystem_Mixer.Close();
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

