#include "particles/fireworks/fireworks.h"
// Firework manager

bool SetGameObjects_Instance1_Fireworks()
{
  CGameObject* firework_manager = gGameObjects.AddGameObject("firework_manager");
  firework_manager->SetEventFunction(&Firework_Manager_Event);
  firework_manager->SetBehaviourFunction(&Firework_Manager_Behaviour);

  return true;
}

int number_of_fireworks = 0;
vector<bool> exploded;

void Firework_Manager_Event(CGameObject* gameObject)
{
  if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
  {
    //std::string random_string = generate_random_alphanumeric_string(5);
    ostringstream oss;
    oss << number_of_fireworks;
    string value = oss.str();

    float timeout = gMath.random(1.f, 3.f);
    gSystem_Data_Storage.SetFloat("firework_timeout_"+value, timeout);
    gSystem_Data_Storage.SetFloat("firework_timer_"+value, gTime.GetTicks_s());

    CGameObject* firework = gGameObjects.AddGameObject("firework_"+value);
    CGameObject* explosion = gGameObjects.AddGameObject("firework_explosion_"+value);
    CGameObject* trail = gGameObjects.AddGameObject("firework_trail_"+value);

    firework->AddChild(explosion);
    firework->AddChild(trail);

    firework->Transform()->position = gMath.random_point(vector3f(50, 0, 0), vector3f(-25, 0, 0));

      // Explosion
    explosion->ParticleEmitter()->material_name = "sprite1";
    explosion->ParticleEmitter()->max_particles = 500;
    explosion->ParticleEmitter()->angle_spread = 360;
    explosion->ParticleEmitter()->gravity(0, -5.f, 0);
    explosion->ParticleEmitter()->start_max_life_time = gMath.random(1.5f, 3.f);
    explosion->ParticleEmitter()->start_min_life_time = 1.f;

    GLfloat r = gMath.random(0.5, 1.f);
    GLfloat g = gMath.random(0.5, 1.f);
    GLfloat b = gMath.random(0.5, 1.f);

    explosion->ParticleEmitter()->start_min_color(r, g, b, 1.f);
    explosion->ParticleEmitter()->start_max_color(r, g, b, 1.f);
    //explosion->particleEmitter()->color_adder(0, 0, 0, -0.1f);

      // Trail
    trail->ParticleEmitter()->material_name = "smoke1";
    trail->ParticleEmitter()->max_particles = 100;
    trail->ParticleEmitter()->angle_spread = 10;
    trail->ParticleEmitter()->direction(0, -1.f, 0);
    trail->ParticleEmitter()->gravity(0, 0.f, 0);

    trail->ParticleEmitter()->start_max_vel = 0.f;
    trail->ParticleEmitter()->start_min_vel = 0.f;

    trail->ParticleEmitter()->start_min_color(1.f, 0.5f, 0.f, 1.f);
    trail->ParticleEmitter()->start_max_color(1.f, 0.5f, 0.f, 1.f);
    trail->ParticleEmitter()->color_adder(-0.2f, -0.1f, 0.f, 0.f);

    trail->ParticleEmitter()->start_max_vel = 0.5f;
    trail->ParticleEmitter()->start_min_vel = 0.f;
    trail->ParticleEmitter()->start_max_life_time = 0.05f;
    trail->ParticleEmitter()->start_min_life_time = 0.85f;

    trail->ParticleEmitter()->Start();

    number_of_fireworks++;
    exploded.resize(number_of_fireworks);
    exploded[exploded.size()-1] = false;
  }
}

void Firework_Manager_Behaviour(CGameObject* gameObject)
{
  if(!number_of_fireworks)
    return;

  for(int i = 0; i < number_of_fireworks; i++)
  {
    ostringstream oss;
    oss << i;
    string value = oss.str();

    CGameObject* current_firework = gGameObjects["firework_"+value];

    float startTime = gSystem_Data_Storage.GetFloat("firework_timer_"+value);
    float timeout = gSystem_Data_Storage.GetFloat("firework_timeout_"+value);

    if((gTime.GetTicks_s() - startTime) < timeout/gTime.timeScale()) // 1.5 segundos.
    {
      current_firework->Transform()->Translate(0.f, 20.f*gTime.deltaTime_s(), 0.f);
    }
    else if(!exploded[i])
    {
      current_firework->GetChild("firework_trail_"+value)->ParticleEmitter()->Stop();
      current_firework->GetChild("firework_explosion_"+value)->ParticleEmitter()->Start();
      current_firework->GetChild("firework_explosion_"+value)->ParticleEmitter()->Stop();

      gMixer.PlaySound("leroy", current_firework);
      //gMixer.PlaySound("explosion", current_firework);

      exploded[i] = true;
    }
    //Firework_Behaviour(gGameObjects["firework_"+value]);
  }
}
