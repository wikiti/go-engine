#include "gameObjects.h"

bool SetGameObjects_Instance1()
{
  SetGameObjects_Instance1_Fireworks();
  //SetGameObjects_Instance1_RandomStuff();

  CGameObject* camara_main = gGameObjects.AddGameObject("camara_main");
  camara_main->transform()->position.y += 3.f;
  //camara_main->transform()->position.z -= 50.f;
  camara_main->transform()->position.z -= 20.f;
  //camara_main->transform()->SetAngle(-30.f, 0.f, 0.f);
  camara_main->SetKeyEventFunction(&Camara_main_movimiento);
  gRender.AddCamera(camara_main);

  CGameObject* rainbow_violet = gGameObjects.AddGameObject("rainbow_violet");
  CGameObject* rainbow_dblue = gGameObjects.AddGameObject("rainbow_dblue");
  CGameObject* rainbow_blue = gGameObjects.AddGameObject("rainbow_blue");
  CGameObject* rainbow_green = gGameObjects.AddGameObject("rainbow_green");
  CGameObject* rainbow_yellow = gGameObjects.AddGameObject("rainbow_yellow");
  CGameObject* rainbow_orange = gGameObjects.AddGameObject("rainbow_orange");
  CGameObject* rainbow_red = gGameObjects.AddGameObject("rainbow_red");

  rainbow_violet->transform()->position.x += 3.f;
  rainbow_dblue->transform()->position.x += 2.5f;
  rainbow_blue->transform()->position.x += 2.f;
  rainbow_green->transform()->position.x += 1.5f;
  rainbow_yellow->transform()->position.x += 1.f;
  rainbow_orange->transform()->position.x += 0.5f;
  //rainbow_red->transform()->position.x -= 10.f;

  // velocidades
  rainbow_violet->particleEmitter()->start_min_vel = rainbow_violet->particleEmitter()->start_max_vel = 8.8f;
  rainbow_dblue->particleEmitter()->start_min_vel = rainbow_dblue->particleEmitter()->start_max_vel = 8.1f;
  rainbow_blue->particleEmitter()->start_min_vel = rainbow_blue->particleEmitter()->start_max_vel = 7.5f;
  rainbow_green->particleEmitter()->start_min_vel = rainbow_green->particleEmitter()->start_max_vel = 6.8f;
  rainbow_yellow->particleEmitter()->start_min_vel = rainbow_yellow->particleEmitter()->start_max_vel = 6.f;
  rainbow_orange->particleEmitter()->start_min_vel = rainbow_orange->particleEmitter()->start_max_vel = 5.f;
  rainbow_red->particleEmitter()->start_min_vel = rainbow_red->particleEmitter()->start_max_vel = 4.f;

  /*rainbow_violet->particleEmitter()->max_particles = 500;
  rainbow_dblue->particleEmitter()->max_particles = 500;
  rainbow_blue->particleEmitter()->max_particles = 500;
  rainbow_green->particleEmitter()->max_particles = 500;
  rainbow_yellow->particleEmitter()->max_particles = 500;
  rainbow_orange->particleEmitter()->max_particles = 500;
  rainbow_red->particleEmitter()->max_particles = 500;*/

  rainbow_violet->particleEmitter()->material_name = "penis1";
  rainbow_dblue->particleEmitter()->material_name = "penis1";
  rainbow_blue->particleEmitter()->material_name = "penis1";
  rainbow_green->particleEmitter()->material_name = "penis1";
  rainbow_yellow->particleEmitter()->material_name = "penis1";
  rainbow_orange->particleEmitter()->material_name = "penis1";
  rainbow_red->particleEmitter()->material_name = "penis1";

  rainbow_violet->particleEmitter()->direction(1.f, 1.6f, 0.f);
  rainbow_dblue->particleEmitter()->direction(1.f, 1.5f, 0.f);
  rainbow_blue->particleEmitter()->direction(1.f, 1.4f, 0.f);
  rainbow_green->particleEmitter()->direction(1.f, 1.3f, 0.f);
  rainbow_yellow->particleEmitter()->direction(1.f, 1.2f, 0.f);
  rainbow_orange->particleEmitter()->direction(1.f, 1.1f, 0.f);
  rainbow_red->particleEmitter()->direction(1.f, 1.f, 0.f);

  rainbow_violet->particleEmitter()->angle_spread = 5.f;
  rainbow_dblue->particleEmitter()->angle_spread = 5.f;
  rainbow_blue->particleEmitter()->angle_spread = 5.f;
  rainbow_green->particleEmitter()->angle_spread = 5.f;
  rainbow_yellow->particleEmitter()->angle_spread = 5.f;
  rainbow_orange->particleEmitter()->angle_spread = 5.f;
  rainbow_red->particleEmitter()->angle_spread = 5.f;

  rainbow_violet->particleEmitter()->start_min_color(0.6f, 0.f, 0.6f);
  rainbow_violet->particleEmitter()->start_max_color(0.6f, 0.f, 0.6f);

  rainbow_dblue->particleEmitter()->start_min_color(0, 0, 0.6f);
  rainbow_dblue->particleEmitter()->start_max_color(0, 0, 0.6f);

  rainbow_blue->particleEmitter()->start_min_color(0, 0, 1.f);
  rainbow_blue->particleEmitter()->start_max_color(0, 0, 1.f);

  rainbow_green->particleEmitter()->start_min_color(0.f, 1.f, 0.f);
  rainbow_green->particleEmitter()->start_max_color(0.f, 1.f, 0.f);

  rainbow_yellow->particleEmitter()->start_min_color(1.f, 1.f, 0.f);
  rainbow_yellow->particleEmitter()->start_max_color(1.f, 1.f, 0.f);

  rainbow_orange->particleEmitter()->start_min_color(1.f, 0.5f, 0.f);
  rainbow_orange->particleEmitter()->start_max_color(1.f, 0.5f, 0.f);

  rainbow_red->particleEmitter()->start_min_color(1.f, 0.f, 0.f);
  rainbow_red->particleEmitter()->start_max_color(1.f, 0.f, 0.f);

  rainbow_violet->particleEmitter()->Start();
  rainbow_dblue->particleEmitter()->Start();
  rainbow_blue->particleEmitter()->Start();
  rainbow_green->particleEmitter()->Start();
  rainbow_yellow->particleEmitter()->Start();
  rainbow_orange->particleEmitter()->Start();
  rainbow_red->particleEmitter()->Start();


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
