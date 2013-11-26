#include "gameObjects.h"

bool SetGameObjects_Instance1()
{
  SetGameObjects_Instance1_Fireworks();
  //SetGameObjects_Instance1_Rainbow();
  SetGameObjects_Instance1_Other_Particles();
  //SetGameObjects_Instance1_RandomStuff();

  CGameObject* camara_main = gGameObjects.AddGameObject("camara_main");
  camara_main->transform()->position.y += 3.f;
  camara_main->transform()->position.z -= 20.f;
  //camara_main->transform()->position.z -= 50.f;
  //camara_main->transform()->SetAngle(-30.f, 0.f, 0.f);
  camara_main->SetKeyEventFunction(&Camara_main_movimiento);
  camara_main->SetEventFunction(&Camara_mouse_movimiento);
  gRender.AddCamera(camara_main);

  return true;
}


void Camara_main_movimiento(CGameObject* gameObject)
{
  if (gKeyboardState[SDL_SCANCODE_W])
  {
    gameObject->transform()->LTranslate(0.f, 0.f, 3.f * gTime.deltaTime_s());
  }
  if (gKeyboardState[SDL_SCANCODE_S])
  {
    gameObject->transform()->LTranslate(0.f, 0.f, -3.f * gTime.deltaTime_s());
  }
  if (gKeyboardState[SDL_SCANCODE_A])
  {
    gameObject->transform()->LTranslate(3.f * gTime.deltaTime_s(), 0.f, 0.f);
  }
  if (gKeyboardState[SDL_SCANCODE_D])
  {
    gameObject->transform()->LTranslate(-3.f * gTime.deltaTime_s(), 0.f, 0.f);
  }
  if (gKeyboardState[SDL_SCANCODE_E])
  {
    gameObject->transform()->Translate(0.f, -3.f * gTime.deltaTime_s(), 0.f);
  }
  if (gKeyboardState[SDL_SCANCODE_Q])
  {
    gameObject->transform()->Translate(0.f, 3.f * gTime.deltaTime_s(), 0.f);
  }

  // Viewport
  if(gKeyboardState[SDL_SCANCODE_I])
  {
    gameObject->camera()->viewport.height += 0.1f * gTime.deltaTime_s();
    if(gameObject->camera()->viewport.height > 1.f)
      gameObject->camera()->viewport.height = 1.f;
  }
  if(gKeyboardState[SDL_SCANCODE_K])
  {
    gameObject->camera()->viewport.height -= 0.1f * gTime.deltaTime_s();
    if(gameObject->camera()->viewport.height < 0.f)
      gameObject->camera()->viewport.height = 0.f;
  }
  if(gKeyboardState[SDL_SCANCODE_J])
  {
    gameObject->camera()->viewport.width -= 0.1f * gTime.deltaTime_s();
    if(gameObject->camera()->viewport.width < 0.f)
      gameObject->camera()->viewport.width = 0.f;
  }
  if(gKeyboardState[SDL_SCANCODE_L])
  {
    gameObject->camera()->viewport.width += 0.1f * gTime.deltaTime_s();
    if(gameObject->camera()->viewport.width > 1.f)
      gameObject->camera()->viewport.width = 1.f;
  }
}

void Camara_mouse_movimiento(CGameObject* gameObject)
{
  static bool hide_cursor = false;

  if(hide_cursor && event.type == SDL_MOUSEMOTION)
  {
    gameObject->transform()->LRotate(event.motion.yrel * 20.f * gTime.deltaTime_s(), 0, 0);
    gameObject->transform()->Rotate(0, event.motion.xrel * -20.f * gTime.deltaTime_s(), 0);
  }
  else if(event.type == SDL_MOUSEWHEEL)
  {
    gameObject->transform()->LTranslate(0.f, 0.f, event.wheel.y * 20.f * gTime.deltaTime_s());
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
