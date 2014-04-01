#include "particles/other_particles.h"

void particle_emitter1_behaviour(CGameObject* object)
{
  GLfloat& n_particles_per_second = object->ParticleEmitter()->particles_per_second;
  if(n_particles_per_second < 10000)
  {
    n_particles_per_second += 1;
    //if((int)n_particles_per_second % 100 == 0)
      //gSystem_Debug.console_msg("Particles per second: %f", n_particles_per_second);
  }
}

bool SetGameObjects_Instance1_Other_Particles()
{
  CGameObject* fire = gGameObjects.Add("fire1");

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

  // Huge test
  CGameObject* particle_emitter1 = gGameObjects.Add("particle_emitter1");
  particle_emitter1->ParticleEmitter()->direction(0.f, 1.f, 0.f);

  // velocidades
  particle_emitter1->ParticleEmitter()->start_min_vel = 10.f;
  particle_emitter1->ParticleEmitter()->start_max_vel = 12.5f;

  particle_emitter1->ParticleEmitter()->start_max_life_time = 5.5f;
  particle_emitter1->ParticleEmitter()->start_min_life_time = 2.5f;

  particle_emitter1->ParticleEmitter()->max_particles = 10000;
  particle_emitter1->ParticleEmitter()->particles_per_second = 10;

  particle_emitter1->ParticleEmitter()->material_name = "sprite1";

  particle_emitter1->ParticleEmitter()->angle_spread = 35.f;

  particle_emitter1->ParticleEmitter()->start_max_color(1.f, 0.f, 0.f, 0.f);
  particle_emitter1->ParticleEmitter()->start_min_color(0.3f, 0.0f, 0.0f, 1.f);

  particle_emitter1->ParticleEmitter()->Start();

  particle_emitter1->SetBehaviourFunction(&particle_emitter1_behaviour);

  return true;
}
