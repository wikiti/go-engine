#include "_systems.h"

using namespace std;

bool Systems_Init()
{
  if(!gSystem_Debug.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load Debug system");
  }

  if(!gSystem_Data_Storage.Init())
  {
    gSystem_Debug.msg_box(ERROR_INIT, "Could not load Storage system");
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
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load Debug::Console system");
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

  if(!gSystem_UserInput.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load UserInput system");
    return false;
  }

  if(!gSystem_Shader_Manager.Init())
  {
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not load Shader system");
    return false;
  }

  gSystem_Debug.ParseAppArguments();

  return true;
}

void Systems_Close()
{
  gSystem_Data_Storage.Close();
  gSystem_GameObject_Manager.Close();
  gSystem_Render.Close();
  gSystem_Debug.Close();
  gSystem_Mixer.ResetSources();
  gSystem_Resources.Close();
  gSystem_Time.Close();
  gSystem_Math.Close();
  gSystem_Mixer.Close();
  gSystem_UserInput.Close();
  gSystem_Shader_Manager.Close();
}

bool Systems_Reset()
{
  if(!gSystem_Debug.Reset())
  {
    gSystem_Debug.msg_box(ERROR_FATAL, "Could not reset Debug system");
  }

  if(!gSystem_Data_Storage.Reset())
  {
    gSystem_Debug.msg_box(ERROR_FATAL, "Could not reset Storage system");
  }

  if(!gSystem_GameObject_Manager.Reset())
  {
    gSystem_Debug.msg_box(ERROR_FATAL, "Could not reset GameObject Manager system");
    return false;
  }

  if(!gSystem_Render.Reset())
  {
    gSystem_Debug.msg_box(ERROR_FATAL, "Could not reset Render system");
    return false;
  }

  if(!gSystem_Mixer.Reset())
  {
    gSystem_Debug.msg_box(ERROR_FATAL, "Could not reset Mixer system");
    return false;
  }

  if(!gSystem_Resources.Reset())
  {
    gSystem_Debug.msg_box(ERROR_FATAL, "Could not reset de Recursos system");
    return false;
  }

  if(!gSystem_Time.Reset())
  {
    gSystem_Debug.msg_box(ERROR_FATAL, "Could not reset Time system");
    return false;
  }

  if(!gSystem_Math.Reset())
  {
    gSystem_Debug.msg_box(ERROR_FATAL, "Could not reset Math system");
    return false;
  }

  if(!gSystem_UserInput.Reset())
  {
    gSystem_Debug.msg_box(ERROR_FATAL, "Could not reset UserInput system");
    return false;
  }

  if(!gSystem_Shader_Manager.Reset())
  {
    gSystem_Debug.msg_box(ERROR_FATAL, "Could not reset Shader system");
    return false;
  }

  return true;
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

