#include "gameObjects.h"

bool SetGameObjects_Instance1()
{
  SetGameObjects_Instance1_Fireworks();
  //SetGameObjects_Instance1_RandomStuff();

  CGameObject* camara_main = gGameObjects.AddGameObject("camara_main");
  camara_main->transform()->position.y += 3.f;
  camara_main->transform()->position.z -= 50.f;
  camara_main->transform()->SetAngle(-30.f, 0.f, 0.f);
  camara_main->SetKeyEventFunction(&Camara_main_movimiento);
  gRender.AddCamera(camara_main);

  return true;
}


void Camara_main_movimiento(CGameObject* gameObject)
{
  if(gKeyboardState[SDL_SCANCODE_LCTRL])
  {
    if(gKeyboardState[SDL_SCANCODE_UP])
    {
      gameObject->transform()->LRotate(-20.f * gTime.deltaTime_s(), 0, 0);
    }
    if(gKeyboardState[SDL_SCANCODE_DOWN])
    {
      gameObject->transform()->LRotate(20.f * gTime.deltaTime_s(), 0, 0);
    }
    if(gKeyboardState[SDL_SCANCODE_LEFT])
    {
      gameObject->transform()->Rotate(0, +20.f * gTime.deltaTime_s(), 0);
    }
    if(gKeyboardState[SDL_SCANCODE_RIGHT])
    {
      gameObject->transform()->Rotate(0, -20.f * gTime.deltaTime_s(), 0);
    }
    if(gKeyboardState[SDL_SCANCODE_PAGEDOWN])
    {
      gameObject->transform()->LRotate(0, 0, 20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_PAGEUP])
    {
      gameObject->transform()->LRotate(0, 0, -20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_R])
    {
      gameObject->transform()->SetAngle(0.f, 0.f, 0.f);
    }
    if(gKeyboardState[SDL_SCANCODE_SPACE])
    {
      //gameObject->transform()->LookAt(gGameObjects["cubo"]->transform()->Position());
    }
  }
  else
  {
    if(gKeyboardState[SDL_SCANCODE_UP])
    {
      gameObject->transform()->LTranslate(0.f, 0.f, 3.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_DOWN])
    {
      gameObject->transform()->LTranslate(0.f, 0.f, -3.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_LEFT])
    {
      gameObject->transform()->LTranslate(3.f * gTime.deltaTime_s(), 0.f, 0.f);
    }
    if(gKeyboardState[SDL_SCANCODE_RIGHT])
    {
      gameObject->transform()->LTranslate(-3.f * gTime.deltaTime_s(), 0.f, 0.f);
    }
    if(gKeyboardState[SDL_SCANCODE_PAGEDOWN])
    {
      gameObject->transform()->Translate(0.f, -3.f * gTime.deltaTime_s(), 0.f);
    }
    if(gKeyboardState[SDL_SCANCODE_PAGEUP])
    {
      gameObject->transform()->Translate(0.f, 3.f * gTime.deltaTime_s(), 0.f);
    }
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
