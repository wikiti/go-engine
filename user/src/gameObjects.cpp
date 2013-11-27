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
  //SetGameObjects_Instance1_Rainbow();
  SetGameObjects_Instance1_Other_Particles();
  //SetGameObjects_Instance1_RandomStuff();

  return true;
}


void Camara_main_movimiento(CGameObject* gameObject)
{
  if (gKeyboardState[SDL_SCANCODE_W])
  {
    gameObject->Transform()->LTranslate(0.f, 0.f, 3.f * gTime.deltaTime_s());
  }
  if (gKeyboardState[SDL_SCANCODE_S])
  {
    gameObject->Transform()->LTranslate(0.f, 0.f, -3.f * gTime.deltaTime_s());
  }
  if (gKeyboardState[SDL_SCANCODE_A])
  {
    gameObject->Transform()->LTranslate(3.f * gTime.deltaTime_s(), 0.f, 0.f);
  }
  if (gKeyboardState[SDL_SCANCODE_D])
  {
    gameObject->Transform()->LTranslate(-3.f * gTime.deltaTime_s(), 0.f, 0.f);
  }
  if (gKeyboardState[SDL_SCANCODE_E])
  {
    gameObject->Transform()->Translate(0.f, -3.f * gTime.deltaTime_s(), 0.f);
  }
  if (gKeyboardState[SDL_SCANCODE_Q])
  {
    gameObject->Transform()->Translate(0.f, 3.f * gTime.deltaTime_s(), 0.f);
  }

  // Viewport
  if(gKeyboardState[SDL_SCANCODE_I])
  {
    gameObject->Camera()->viewport.height += 0.1f * gTime.deltaTime_s();
    if(gameObject->Camera()->viewport.height > 1.f)
      gameObject->Camera()->viewport.height = 1.f;
  }
  if(gKeyboardState[SDL_SCANCODE_K])
  {
    gameObject->Camera()->viewport.height -= 0.1f * gTime.deltaTime_s();
    if(gameObject->Camera()->viewport.height < 0.f)
      gameObject->Camera()->viewport.height = 0.f;
  }
  if(gKeyboardState[SDL_SCANCODE_J])
  {
    gameObject->Camera()->viewport.width -= 0.1f * gTime.deltaTime_s();
    if(gameObject->Camera()->viewport.width < 0.f)
      gameObject->Camera()->viewport.width = 0.f;
  }
  if(gKeyboardState[SDL_SCANCODE_L])
  {
    gameObject->Camera()->viewport.width += 0.1f * gTime.deltaTime_s();
    if(gameObject->Camera()->viewport.width > 1.f)
      gameObject->Camera()->viewport.width = 1.f;
  }

  if (gKeyboardState[SDL_SCANCODE_T])
  {
    gameObject->Transform()->LookAt(vector3f(0.f, 0.f, 0.f));
  }
}

void Camara_mouse_movimiento(CGameObject* gameObject)
{
  static bool hide_cursor = false;

  if(hide_cursor && event.type == SDL_MOUSEMOTION)
  {
    gameObject->Transform()->LRotate(event.motion.yrel * 20.f * gTime.deltaTime_s(), 0, 0);
    gameObject->Transform()->Rotate(0, event.motion.xrel * -20.f * gTime.deltaTime_s(), 0);
    cout << gameObject->Transform()->EulerAngles() << endl;
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
