/*
 * gestores.cpp
 *
 *  Created on: 25/04/2014
 *      Author: Dani
 */

#include "presentacion/general.h"
#include "presentacion/gestores.h"
#include "presentacion/gestor_entrada.h"

void Camara_main_rotacion(CGameObject* gameObject)
{
  vector3f rotation = gameObject->Transform()->LRotation();
  gameObject->Transform()->SetAngle( gMath.lerpAngles(rotation, rotate_to, 0.02) );

  GLfloat epsilon = 10;
  if(diapositiva_actual == 4 and epsilon > gMath.NormalizeAngle( gameObject->Transform()->LRotation().y - rotate_to.y ) )
  {
    gGameObjects["slide5_camera"]->Enable();
  }
  else if(diapositiva_actual != 4)
  {
    if(gGameObjects["slide5_camera"]) gGameObjects["slide5_camera"]->Disable();
  }
}

void Camara_main_input(CGameObject* gameObject)
{
  vector<GO_InputClasses::CJoystick> joys = gUserInput.GetJoysticks();

  if(joys.size() > 0)
  {
    // Rotation
    if(joys[0].axes.size() >= 2)
    {
      gameObject->Transform()->LRotate(joys[0].axes[1].value * 60.f * gTime.deltaTime_s(), 0, 0);
      gameObject->Transform()->Rotate(0, joys[0].axes[0].value * -60.f * gTime.deltaTime_s(), 0);
    }

  }
}

void Planeta_Movimiento(CGameObject* gameObject)
{
  GLfloat speed = 10 / gameObject->Transform()->scale.x;

  gameObject->Transform()->LRotate(0, speed * gTime.deltaTime_s(), 0);
}

void Crate_Movimiento(CGameObject* gameObject)
{
  // Rotar mediante seno
  GLfloat angle = 45 * gMath.sin( gTime.GetTicks_s() * 5 );
  gameObject->Transform()->SetAngle(angle, gTime.GetTicks_s() * 15, 0);

  // Cambiar escala
  GLfloat scale = gMath.abs( gMath.cos( gTime.GetTicks_s() * 10  ) );
  gameObject->Transform()->SetScale(scale, scale, scale);

  // Sumar un color aleatorio
  GLfloat gb = gMath.abs( gMath.cos( gTime.GetTicks_s() * 25  ) );
  gameObject->MeshRender()->color.g = gameObject->MeshRender()->color.b = gb;
}

void Wrench_Movimiento(CGameObject* gameObject)
{
  gameObject->Transform()->Rotate(0, -60.f * gTime.deltaTime_s(), 0);
}

void Camera_BeforeRender(CGameObject* gameObject)
{
  CGameObject* mesh = gGameObjects["slide5_mesh"];
  mesh->MeshRender()->material_name = "texture_crate";

  CGameObject* titulo = gGameObjects["slide5_titulo"];
  titulo->MeshRender()->color(0, 0, 0, 1);
}

void Camera_AfterRender(CGameObject* gameObject)
{
  CGameObject* mesh = gGameObjects["slide5_mesh"];
  mesh->MeshRender()->material_name = "texture_mdl_hada1";

  CGameObject* titulo = gGameObjects["slide5_titulo"];
  titulo->MeshRender()->color = COLOR_TITULO;
}


int pres_number_of_fireworks = 0;

void Firework_Manager_Crear(CGameObject* gameObject)
{
  ostringstream oss;
  oss << pres_number_of_fireworks;
  string value = oss.str();

  float timeout = gMath.random(1.f, 2.f);
  gSystem_Data_Storage.SetFloat("firework_timeout_id" + value, timeout);
  gSystem_Data_Storage.SetFloat("firework_timer_id" + value, gTime.GetTicks_s());
  gSystem_Data_Storage.SetInt("firework_exploded_id" + value, 0);

  CGameObject* firework = gGameObjects.Add("firework_id" + value);

  CGameObject* explosion = gGameObjects.Add("firework_explosion_id" + value);
  CGameObject* trail = gGameObjects.Add("firework_trail_id" + value);

  firework->AddChild(explosion);
  firework->AddChild(trail);
  firework->Transform()->position = gMath.random_point(vector3f(0, 0, 20), vector3f(0, 0, -10));
  firework->Transform()->position.x = -20;

  // Explosion
  explosion->ParticleEmitter()->material_name = "sprite1";
  explosion->ParticleEmitter()->max_particles = gData.GetInt("firework_explosion_particles");
  explosion->ParticleEmitter()->particles_per_second = 0;
  explosion->ParticleEmitter()->angle_spread = 360;
  explosion->ParticleEmitter()->gravity(0, -5.f, 0);
  explosion->ParticleEmitter()->start_max_life_time = gMath.random(1.5f, 3.f);
  explosion->ParticleEmitter()->start_min_life_time = 1.f;

  GLfloat r = gMath.random(0.5, 1.f);
  GLfloat g = gMath.random(0.5, 1.f);
  GLfloat b = gMath.random(0.5, 1.f);

  explosion->ParticleEmitter()->start_min_color(r, g, b, 1.f);
  explosion->ParticleEmitter()->start_max_color(r, g, b, 1.f);

  r = gMath.random(0.f, 1.f);
  g = gMath.random(0.f, 1.f);
  b = gMath.random(0.f, 1.f);

  explosion->ParticleEmitter()->color_adder(r, g, b, 0.f);
  //explosion->particleEmitter()->color_adder(0, 0, 0, -0.1f);

  explosion->AudioSource()->SetSound("firework_explosion");
  explosion->AudioSource()->volume = 1.f;
  explosion->AudioSource()->max_distance = 200.f;

  // Trail
  trail->ParticleEmitter()->material_name = "smoke1";
  trail->ParticleEmitter()->max_particles = 500;
  trail->ParticleEmitter()->particles_per_second = 500;
  trail->ParticleEmitter()->angle_spread = 10;
  trail->ParticleEmitter()->direction(0, -1.f, 0);
  trail->ParticleEmitter()->gravity(0, 0.f, 0);

  trail->ParticleEmitter()->start_max_vel = 0.f;
  trail->ParticleEmitter()->start_min_vel = 0.f;

  trail->ParticleEmitter()->start_min_scale = 0.75f;
  trail->ParticleEmitter()->start_max_scale = 1.f;

  trail->ParticleEmitter()->start_min_color(1.f, 0.5f, 0.12f, 0.5f);
  trail->ParticleEmitter()->start_max_color(1.f, 0.5f, 0.12f, 0.8f);
  trail->ParticleEmitter()->min_color(0.12f, 0.12f, 0.12f, 0.f);
  trail->ParticleEmitter()->color_adder(-4.f, -2.f, 0.f, -1.f);

  trail->ParticleEmitter()->start_max_vel = 0.5f;
  trail->ParticleEmitter()->start_min_vel = 0.f;
  trail->ParticleEmitter()->start_max_life_time = 0.05f;
  trail->ParticleEmitter()->start_min_life_time = 0.85f;

  trail->ParticleEmitter()->Start();

  trail->AudioSource()->SetSound("firework_trail");
  trail->AudioSource()->volume = 0.6f;
  trail->AudioSource()->max_distance = 100.f;
  trail->AudioSource()->Bind();
  trail->AudioSource()->Play();

  pres_number_of_fireworks++;
}


void Firework_Manager_Comportamiento(CGameObject* gameObject)
{
  static GLfloat start_time = gTime.GetTicks_s();
  // Probabilida de crear un fuego artificial: 20% cada medio segundo
  if(gTime.GetTicks_s() - start_time >= 0.5f)
  {
    if(gMath.random(0.0f, 1.0f) <= 0.20)
      Firework_Manager_Crear(gameObject);

    start_time = gTime.GetTicks_s();
  }

  vector<CGameObject*> fireworks = gGameObjects.Search("firework_id");
  for(vector<CGameObject*>::iterator it = fireworks.begin(); it != fireworks.end(); ++it)
  {
    string value =  (*it)->GetName().substr(11);
    CGameObject* current_firework = (*it);

    int exploded = gSystem_Data_Storage.GetInt("firework_exploded_id" + value);
    float startTime = gSystem_Data_Storage.GetFloat("firework_timer_id" + value);
    float timeout = gSystem_Data_Storage.GetFloat("firework_timeout_id" + value);

    if((gTime.GetTicks_s() - startTime) < timeout / gTime.timeScale()) // 1.5 segundos.
    {
      current_firework->Transform()->Translate(0.f, 20.f * gTime.deltaTime_s(), 0.f);
    }
    else if(!exploded)
    {
      current_firework->GetChild(1)->ParticleEmitter()->Stop();
      current_firework->GetChild(0)->ParticleEmitter()->Start();
      current_firework->GetChild(0)->ParticleEmitter()->Stop();
      current_firework->GetChild(0)->AudioSource()->PlayOneShot();

      gSystem_Data_Storage.SetInt("firework_exploded_id" + value, 1);
    }
    else if((gTime.GetTicks_s() - startTime) > 5 / gTime.timeScale())
    {
      // Clear float vars
      gSystem_Data_Storage.RemoveFloat("firework_timer_id"+value);
      gSystem_Data_Storage.RemoveFloat("firework_timeout_id"+value);
      gSystem_Data_Storage.RemoveFloat("firework_exploded_id"+value);

      // Clear vectors
      gGameObjects.Delete(current_firework->GetName(), true);
    }
  }
}
