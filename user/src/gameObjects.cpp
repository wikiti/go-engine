#include "gameObjects.h"

bool SetGameObjects_Instance1()
{
  CGameObject* camara_main = gGameObjects.AddGameObject("camara_main");
  camara_main->Camera()->skybox_texture = "skybox1";
  camara_main->Transform()->position.y += 3.f;
  camara_main->Transform()->position.z -= 20.f;

  camara_main->SetKeyEventFunction(&Camara_main_movimiento);
  camara_main->SetEventFunction(&Camara_mouse_movimiento);

  gRender.AddCamera(camara_main);

  SetGameObjects_Instance1_Fireworks();
  SetGameObjects_Instance1_Rainbow();
  SetGameObjects_Instance1_Other_Particles();
  SetGameObjects_Instance1_Shaders();
  //SetGameObjects_Instance1_RandomStuff();

  return true;
}


void Camara_main_movimiento(CGameObject* gameObject)
{
  float boost = 1.f;
  if (gUserInput.Keyboard("left shift"))
    boost = 3.f;

  if (gUserInput.axis1.vertical > 0)
  {
    gameObject->Transform()->LTranslate(0.f, 0.f, boost * 3.f * gTime.deltaTime_s());
  }
  if (gUserInput.axis1.vertical < 0)
  {
    gameObject->Transform()->LTranslate(0.f, 0.f, boost * -3.f * gTime.deltaTime_s());
  }
  if (gUserInput.axis1.horizontal < 0)
  {
    gameObject->Transform()->LTranslate(boost * 3.f * gTime.deltaTime_s(), 0.f, 0.f);
  }
  if (gUserInput.axis1.horizontal > 0)
  {
    gameObject->Transform()->LTranslate(boost * -3.f * gTime.deltaTime_s(), 0.f, 0.f);
  }
  if (gUserInput.Keyboard("E"))
  {
    gameObject->Transform()->Translate(0.f, boost * -3.f * gTime.deltaTime_s(), 0.f);
  }
  if (gUserInput.Keyboard("Q"))
  {
    gameObject->Transform()->Translate(0.f, boost * 3.f * gTime.deltaTime_s(), 0.f);
  }

  // Viewport
  if(gUserInput.axis2.vertical > 0)
  {
    gameObject->Camera()->viewport.height += 0.1f * gTime.deltaTime_s();
    if(gameObject->Camera()->viewport.height > 1.f)
      gameObject->Camera()->viewport.height = 1.f;
  }
  if(gUserInput.axis2.vertical < 0)
  {
    gameObject->Camera()->viewport.height -= 0.1f * gTime.deltaTime_s();
    if(gameObject->Camera()->viewport.height < 0.f)
      gameObject->Camera()->viewport.height = 0.f;
  }
  if(gUserInput.axis2.horizontal < 0)
  {
    gameObject->Camera()->viewport.width -= 0.1f * gTime.deltaTime_s();
    if(gameObject->Camera()->viewport.width < 0.f)
      gameObject->Camera()->viewport.width = 0.f;
  }
  if(gUserInput.axis2.horizontal > 0)
  {
    gameObject->Camera()->viewport.width += 0.1f * gTime.deltaTime_s();
    if(gameObject->Camera()->viewport.width > 1.f)
      gameObject->Camera()->viewport.width = 1.f;
  }

  if (gUserInput.Keyboard("T"))
  {
    gameObject->Transform()->LookAt(vector3f(0.f, 0.f, 0.f));
  }

  if (gUserInput.jump() == GO_Keystates::keydown)
  {
    gDebug.console_msg("lololo");
  }
  if (gUserInput.jump() == GO_Keystates::keyup)
  {
    gDebug.console_msg("lololo");
  }
}

void Camara_mouse_movimiento(CGameObject* gameObject)
{
  static bool hide_cursor = false;

  if(hide_cursor && event.type == SDL_MOUSEMOTION)
  {
    //GLfloat pitch = gameObject->Transform()->pitch();
    //GLfloat yaw = gameObject->Transform()->yaw();
    //if(pitch + event.motion.yrel * 20.f * gTime.deltaTime_s() < 90 or pitch + event.motion.yrel * 20.f * gTime.deltaTime_s() > 270)
    gameObject->Transform()->LRotate(event.motion.yrel * 20.f * gTime.deltaTime_s(), 0, 0);
    gameObject->Transform()->Rotate(0, event.motion.xrel * -20.f * gTime.deltaTime_s(), 0);
    //cout << gameObject->Transform()->pitch() << " " << gameObject->Transform()->yaw() << " " << gameObject->Transform()->roll() << endl;
    //vector3f euler = gameObject->Transform()->EulerAngles();
    //cout << ((float*) &euler)[0] << " " << ((float*) &euler)[1] << " " << ((float*) &euler)[2] << endl;
  }
  else if(event.type == SDL_MOUSEWHEEL)
  {
    gameObject->Transform()->LTranslate(0.f, 0.f, event.wheel.y * 20.f * gTime.deltaTime_s());
  }
  else if(event.type == SDL_MOUSEBUTTONDOWN)
  {
    if( event.button.button == SDL_BUTTON_RIGHT)
    {
      hide_cursor = !hide_cursor;
      gRender.SetRelativeMouseMode(hide_cursor);
    }
  }
}
