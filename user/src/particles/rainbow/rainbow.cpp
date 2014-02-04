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

  CGameObject* rainbow = gGameObjects.AddGameObject("rainbow");
  rainbow->AddChildren({rainbow_violet, rainbow_dblue, rainbow_blue, rainbow_green, rainbow_yellow, rainbow_orange, rainbow_red});
  rainbow->Transform()->position.y += 3.f;

  rainbow_violet->Transform()->position.x = 3.f;
  rainbow_dblue->Transform()->position.x = 2.5f;
  rainbow_blue->Transform()->position.x = 2.f;
  rainbow_green->Transform()->position.x = 1.5f;
  rainbow_yellow->Transform()->position.x = 1.f;
  rainbow_orange->Transform()->position.x = 0.5f;
  //rainbow_red->transform()->position.x -= 10.f;

  // velocidades
  rainbow_violet->ParticleEmitter()->start_min_vel = rainbow_violet->ParticleEmitter()->start_max_vel = 8.8f;
  rainbow_dblue->ParticleEmitter()->start_min_vel = rainbow_dblue->ParticleEmitter()->start_max_vel = 8.1f;
  rainbow_blue->ParticleEmitter()->start_min_vel = rainbow_blue->ParticleEmitter()->start_max_vel = 7.5f;
  rainbow_green->ParticleEmitter()->start_min_vel = rainbow_green->ParticleEmitter()->start_max_vel = 6.8f;
  rainbow_yellow->ParticleEmitter()->start_min_vel = rainbow_yellow->ParticleEmitter()->start_max_vel = 6.f;
  rainbow_orange->ParticleEmitter()->start_min_vel = rainbow_orange->ParticleEmitter()->start_max_vel = 5.f;
  rainbow_red->ParticleEmitter()->start_min_vel = rainbow_red->ParticleEmitter()->start_max_vel = 4.f;

//  rainbow_violet->particleEmitter()->start_max_base_radius = 2.f;
//  rainbow_violet->particleEmitter()->start_min_base_radius = 2.f;
//  rainbow_dblue->particleEmitter()->start_max_base_radius = rainbow_dblue->particleEmitter()->start_min_base_radius= 2.f;
//  rainbow_blue->particleEmitter()->start_max_base_radius = rainbow_blue->particleEmitter()->start_min_base_radius = 2.f;
//  rainbow_green->particleEmitter()->start_max_base_radius = rainbow_green->particleEmitter()->start_min_base_radius = 2.f;
//  rainbow_yellow->particleEmitter()->start_max_base_radius = rainbow_yellow->particleEmitter()->start_min_base_radius = 2.f;
//  rainbow_orange->particleEmitter()->start_max_base_radius = rainbow_orange->particleEmitter()->start_min_base_radius = 2.f;
//  rainbow_red->particleEmitter()->start_max_base_radius = rainbow_red->particleEmitter()->start_min_base_radius = 2.f;

    rainbow_violet->ParticleEmitter()->max_particles = 200;
   rainbow_dblue->ParticleEmitter()->max_particles = 200;
   rainbow_blue->ParticleEmitter()->max_particles = 200;
   rainbow_green->ParticleEmitter()->max_particles = 200;
   rainbow_yellow->ParticleEmitter()->max_particles = 200;
   rainbow_orange->ParticleEmitter()->max_particles = 200;
   rainbow_red->ParticleEmitter()->max_particles = 200;

  rainbow_violet->ParticleEmitter()->particles_per_second = 200;
  rainbow_dblue->ParticleEmitter()->particles_per_second = 200;
  rainbow_blue->ParticleEmitter()->particles_per_second = 200;
  rainbow_green->ParticleEmitter()->particles_per_second = 200;
  rainbow_yellow->ParticleEmitter()->particles_per_second = 200;
  rainbow_orange->ParticleEmitter()->particles_per_second = 200;
  rainbow_red->ParticleEmitter()->particles_per_second = 200;

  rainbow_violet->ParticleEmitter()->material_name = "sprite1";
  rainbow_dblue->ParticleEmitter()->material_name = "sprite1";
  rainbow_blue->ParticleEmitter()->material_name = "sprite1";
  rainbow_green->ParticleEmitter()->material_name = "sprite1";
  rainbow_yellow->ParticleEmitter()->material_name = "sprite1";
  rainbow_orange->ParticleEmitter()->material_name = "sprite1";
  rainbow_red->ParticleEmitter()->material_name = "sprite1";

  rainbow_violet->ParticleEmitter()->direction(-1.f, 1.6f, 0.f);
  rainbow_dblue->ParticleEmitter()->direction(-1.f, 1.5f, 0.f);
  rainbow_blue->ParticleEmitter()->direction(-1.f, 1.4f, 0.f);
  rainbow_green->ParticleEmitter()->direction(-1.f, 1.3f, 0.f);
  rainbow_yellow->ParticleEmitter()->direction(-1.f, 1.2f, 0.f);
  rainbow_orange->ParticleEmitter()->direction(-1.f, 1.1f, 0.f);
  rainbow_red->ParticleEmitter()->direction(-1.f, 1.f, 0.f);

  rainbow_violet->ParticleEmitter()->angle_spread = 5.f;
  rainbow_dblue->ParticleEmitter()->angle_spread = 5.f;
  rainbow_blue->ParticleEmitter()->angle_spread = 5.f;
  rainbow_green->ParticleEmitter()->angle_spread = 5.f;
  rainbow_yellow->ParticleEmitter()->angle_spread = 5.f;
  rainbow_orange->ParticleEmitter()->angle_spread = 5.f;
  rainbow_red->ParticleEmitter()->angle_spread = 5.f;

  rainbow_violet->ParticleEmitter()->start_min_color(0.6f, 0.f, 0.6f);
  rainbow_violet->ParticleEmitter()->start_max_color(0.6f, 0.f, 0.6f);

  rainbow_dblue->ParticleEmitter()->start_min_color(0, 0, 0.6f);
  rainbow_dblue->ParticleEmitter()->start_max_color(0, 0, 0.6f);

  rainbow_blue->ParticleEmitter()->start_min_color(0, 0, 1.f);
  rainbow_blue->ParticleEmitter()->start_max_color(0, 0, 1.f);

  rainbow_green->ParticleEmitter()->start_min_color(0.f, 1.f, 0.f);
  rainbow_green->ParticleEmitter()->start_max_color(0.f, 1.f, 0.f);

  rainbow_yellow->ParticleEmitter()->start_min_color(1.f, 1.f, 0.f);
  rainbow_yellow->ParticleEmitter()->start_max_color(1.f, 1.f, 0.f);

  rainbow_orange->ParticleEmitter()->start_min_color(1.f, 0.5f, 0.f);
  rainbow_orange->ParticleEmitter()->start_max_color(1.f, 0.5f, 0.f);

  rainbow_red->ParticleEmitter()->start_min_color(1.f, 0.f, 0.f);
  rainbow_red->ParticleEmitter()->start_max_color(1.f, 0.f, 0.f);

//  rainbow_violet->particleEmitter()->gravity(0, 0, 0);

  rainbow_violet->ParticleEmitter()->Start();
  rainbow_dblue->ParticleEmitter()->Start();
  rainbow_blue->ParticleEmitter()->Start();
  rainbow_green->ParticleEmitter()->Start();
  rainbow_yellow->ParticleEmitter()->Start();
  rainbow_orange->ParticleEmitter()->Start();
  rainbow_red->ParticleEmitter()->Start();

  return true;
}
