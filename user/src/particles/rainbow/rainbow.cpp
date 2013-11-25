#include "particles/rainbow/rainbow.h"

// Firework manager

bool SetGameObjects_Instance1_Rainbow()
{
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

  rainbow_violet->particleEmitter()->material_name = "sprite1";
  rainbow_dblue->particleEmitter()->material_name = "sprite1";
  rainbow_blue->particleEmitter()->material_name = "sprite1";
  rainbow_green->particleEmitter()->material_name = "sprite1";
  rainbow_yellow->particleEmitter()->material_name = "sprite1";
  rainbow_orange->particleEmitter()->material_name = "sprite1";
  rainbow_red->particleEmitter()->material_name = "sprite1";

  rainbow_violet->particleEmitter()->direction(-1.f, 1.6f, 0.f);
  rainbow_dblue->particleEmitter()->direction(-1.f, 1.5f, 0.f);
  rainbow_blue->particleEmitter()->direction(-1.f, 1.4f, 0.f);
  rainbow_green->particleEmitter()->direction(-1.f, 1.3f, 0.f);
  rainbow_yellow->particleEmitter()->direction(-1.f, 1.2f, 0.f);
  rainbow_orange->particleEmitter()->direction(-1.f, 1.1f, 0.f);
  rainbow_red->particleEmitter()->direction(-1.f, 1.f, 0.f);

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
