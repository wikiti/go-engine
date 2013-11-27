#include "particles/other_particles.h"

bool SetGameObjects_Instance1_Other_Particles()
{
  CGameObject* fire = gGameObjects.AddGameObject("fire1");

  //fire->transform()->position.x += 3.f;
  fire->ParticleEmitter()->direction(0.f, -1.f, 0.f);

  // velocidades
  fire->ParticleEmitter()->start_min_vel = -1.f;
  fire->ParticleEmitter()->start_max_vel = -1.5f;

  fire->ParticleEmitter()->start_max_base_radius = 0.5f;
  fire->ParticleEmitter()->start_min_base_radius = 0.f;

  fire->ParticleEmitter()->start_max_life_time = 1.5f;
  fire->ParticleEmitter()->start_min_life_time = 0.5f;

  fire->ParticleEmitter()->max_particles = 15;

  fire->ParticleEmitter()->material_name = "fire1";

  fire->ParticleEmitter()->angle_spread = 5.f;

  fire->ParticleEmitter()->start_max_color(1.f, 1.f, 0.4f, 0.f);
  fire->ParticleEmitter()->start_min_color(1.f, 1.f, 0.23f, 0.f);

  fire->ParticleEmitter()->start_min_scale = 1.5f;
  fire->ParticleEmitter()->start_max_scale = 1.8f;

  fire->ParticleEmitter()->color_adder(0.f, -1.f, -0.5f, 2.f);

  fire->ParticleEmitter()->start_max_scale_factor = -0.5;
  fire->ParticleEmitter()->start_min_scale_factor = -0.5;

  fire->ParticleEmitter()->gravity(0, 0, 0);

  fire->ParticleEmitter()->Start();

  return true;
}
