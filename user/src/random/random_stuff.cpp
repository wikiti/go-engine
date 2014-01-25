/*
 * random_stuff.cpp
 *
 *  Created on: 24/11/2013
 *      Author: Dani
 */

#include "random/random_stuff.h"

bool SetGameObjects_Instance1_RandomStuff()
{
  CGameObject* INVALID = NULL;
  INVALID = gGameObjects.AddGameObject("$invalid1");
  INVALID = gGameObjects.AddGameObject("_inva lid1");
  INVALID = gGameObjects.AddGameObject("inva%liAd1");
  INVALID = gGameObjects.AddGameObject("_VALID_");

  CGameObject* gui_example = gGameObjects.AddGameObject("gui_example");
  gui_example->GUITexture()->texture_name = "textura1";
  gui_example->GUITexture()->width = gui_example->GUITexture()->height = 0.5f;
  //gui_example->Transform()->scale(2.f, 2.f, 2.f);
  gui_example->Transform()->position(0.5f, 0.5f, 0.f);
  gui_example->Transform()->Rotate(0.f, 0.f, 45.f);
  gui_example->GUITexture()->pixel_offset_x = 2;
  gui_example->GUITexture()->color.a = 0.5f;
  gui_example->GUITexture()->color.g = gui_example->GUITexture()->color.b = 0.f;

  CGameObject* camara_second = gGameObjects.AddGameObject("camara_second");
  CGameObject* cubo_main = gGameObjects.AddGameObject("cubo_main");
  CGameObject* cubo_second = gGameObjects.AddGameObject("cubo_second");
  CGameObject* cubo_third = gGameObjects.AddGameObject("cubo_third");
  CGameObject* hada = gGameObjects.AddGameObject("hada");

  CGameObject* particle_emitter = gGameObjects.AddGameObject("particle_emitter");
  particle_emitter->ParticleEmitter()->material_name = "smoke1";
  particle_emitter->ParticleEmitter()->max_particles = 250;
  particle_emitter->ParticleEmitter()->angle_spread = 90;
  particle_emitter->ParticleEmitter()->gravity(0, -5.f, 0);
  //particle_emitter->particleEmitter()->color_adder(-1.f, -1.f, 0.f);
  particle_emitter->ParticleEmitter()->start_min_color(0.f, 0.5f, 0.5f, 1.f);
  particle_emitter->ParticleEmitter()->start_max_color(0.f, 1.f, 1.f, 1.f);
  particle_emitter->ParticleEmitter()->start_max_scale = 2.f;
  particle_emitter->ParticleEmitter()->start_min_scale_factor = -0.5f;
  particle_emitter->Transform()->Translate(0.f, 0.f, 5.f);
  particle_emitter->SetEventFunction(&Particle_emitter_explosions);

  CGameObject* random_vector = gGameObjects.AddGameObject("random_vector");
  random_vector->AddComponent<CComponent_Dummy3>();

  CGameObject* p1 = gGameObjects.AddGameObject("padre1");
  CGameObject* h1 = gGameObjects.AddGameObject("hijo1");
  CGameObject* h11 = gGameObjects.AddGameObject("hijo11");
  CGameObject* h12 = gGameObjects.AddGameObject("hijo12");
  CGameObject* h2 = gGameObjects.AddGameObject("hijo2");
  CGameObject* h21 = gGameObjects.AddGameObject("hijo21");
  CGameObject* h212 = gGameObjects.AddGameObject("hijo212");

  p1->AddChild(h1);
  p1->AddChild(h2);

  h1->AddChild(h11);
  h1->AddChild(h12);

  h2->AddChild(h21);
  h21->AddChild(h212);

  //h1->RemoveChildren();

  //gGameObjects.DeleteGameObject("hijo1", true);
  //gGameObjects.DeleteGameObject("hijo2", false);

  cubo_main->AddChild(cubo_second);
  cubo_main->AddChild(particle_emitter);
  cubo_second->AddChild(cubo_third);

  camara_second->Transform()->position.y += 3.f;
  cubo_second->Transform()->Translate(1.f, 0.f, 0.f);
  cubo_third->Transform()->Translate(0.f, 1.f, 0.f);
  cubo_main->Transform()->Translate(0.f, 0.f, 5.f);

  hada->Transform()->SetScale(0.1f, 0.1f, 0.1f);
  hada->Transform()->Translate(0.f, 0.f, 10.f);
  hada->Transform()->Rotate(0.f, 90.f, 0.f);

  camara_second->Camera()->SetTarget(cubo_third->GetName());
  camara_second->Camera()->viewport.height = camara_second->Camera()->viewport.width = 0.3f;
  camara_second->Camera()->background_color(0, 0.5f, 0.75f, 1.f);
  camara_second->Camera()->skybox_texture = "skybox1";

  cubo_main->SetKeyEventFunction(&Cubo_main_movimiento);
  cubo_second->SetKeyEventFunction(&Cubo_second_movimiento);
  cubo_third->SetBehaviourFunction(&Cubo_third_behaviour);

  hada->SetBehaviourFunction(&Hada_movimiento);
  DefineTramsformByVar("GO_HADA");

  gSystem_Render.AddCamera(camara_second);

  cubo_main->AddComponent<CComponent_Dummy1>();
  cubo_second->AddComponent<CComponent_Dummy1>();
  cubo_third->AddComponent<CComponent_Dummy1>();
  //hada->AddComponent<CComponent_Dummy2>();

  hada->MeshRender()->mesh_name = "mdl_hada1";
  hada->MeshRender()->material_name = "texture_mdl_hada1";

  camara_second->Camera()->before_render = &Camara_second_before_render;
  camara_second->Camera()->after_render = &Camara_second_after_render;

  gGameObjects.RenameGameObject("cubo_main", "cubo_second");
  gGameObjects.RenameGameObject("cubo_main", "cubo");

  return true;
}

void Camara_second_before_render(CGameObject* gameObject)
{
  gGameObjects["hada"]->MeshRender()->material_name = "textura1";
}

void Camara_second_after_render(CGameObject* gameObject)
{
  gGameObjects["hada"]->MeshRender()->material_name = "texture_mdl_hada1";
}

void Cubo_main_movimiento(CGameObject* gameObject)
{
  if(gUserInput.Keyboard(SDL_SCANCODE_LCTRL))
  {
    if(gUserInput.Keyboard(SDL_SCANCODE_W))
    {
      gameObject->Transform()->Rotate(20.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_S))
    {
      gameObject->Transform()->Rotate(-20.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_D))
    {
      gameObject->Transform()->Rotate(0.f, 20.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_A))
    {
      gameObject->Transform()->Rotate(0.f, -20.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_Q))
    {
      gameObject->Transform()->Rotate(0.f, 0.f, -20.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_E))
    {
      gameObject->Transform()->Rotate(0.f, 0.f, 20.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_R))
    {
      gameObject->Transform()->SetAngle(0, 0, 0);
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_1))
    {
      gameObject->Transform()->SetAngle(45, 0, 0);
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_2))
    {
      gameObject->Transform()->SetAngle(0, 45, 0);
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_3))
    {
      gameObject->Transform()->SetAngle(0, 0, 45);
    }
//    cout << "Cubo vectors:" << endl;
//    cout << "X: " << gameObject->transform()->left() << endl;
//    cout << "Y: " << gameObject->transform()->up() << endl;
//    cout << "Z: " << gameObject->transform()->forward() << endl;
//    cout << "--------------------------------------" << endl;
  }
  else if(gUserInput.Keyboard(SDL_SCANCODE_LALT))
  {
    if(gUserInput.Keyboard(SDL_SCANCODE_W))
    {
      gameObject->Transform()->LRotate(20.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_S))
    {
      gameObject->Transform()->LRotate(-20.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_D))
    {
      gameObject->Transform()->LRotate(0.f, 20.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_A))
    {
      gameObject->Transform()->LRotate(0.f, -20.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_Q))
    {
      gameObject->Transform()->LRotate(0.f, 0.f, -20.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_E))
    {
      gameObject->Transform()->LRotate(0.f, 0.f, 20.f * gTime.deltaTime_s());
    }
  }
  else if(gUserInput.Keyboard(SDL_SCANCODE_LSHIFT))
  {
    if(gUserInput.Keyboard(SDL_SCANCODE_W))
    {
      gameObject->Transform()->LTranslate(0.f, 0.f, 5.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_S))
    {
      gameObject->Transform()->LTranslate(0.f, 0.f, -5.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_D))
    {
      gameObject->Transform()->LTranslate(5.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_A))
    {
      gameObject->Transform()->LTranslate(-5.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_Q))
    {
      gameObject->Transform()->LTranslate(0.f, 5.f * gTime.deltaTime_s(), 0.f);
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_E))
    {
      gameObject->Transform()->LTranslate(0.f, -5.f * gTime.deltaTime_s(), 0.f);
    }
  }
  else
  {
    if(gUserInput.Keyboard(SDL_SCANCODE_W))
    {
      gameObject->Transform()->Translate(0.f, 0.f, 5.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_S))
    {
      gameObject->Transform()->Translate(0.f, 0.f, -5.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_D))
    {
      gameObject->Transform()->Translate(5.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_A))
    {
      gameObject->Transform()->Translate(-5.f * gTime.deltaTime_s());
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_Q))
    {
      gameObject->Transform()->Translate(0.f, 5.f * gTime.deltaTime_s(), 0.f);
    }
    if(gUserInput.Keyboard(SDL_SCANCODE_E))
    {
      gameObject->Transform()->Translate(0.f, -5.f * gTime.deltaTime_s(), 0.f);
    }
  }
}


void Cubo_second_movimiento(CGameObject* gameObject)
{
  static float scale = 1.f;
  // Viewport
  if(gUserInput.Keyboard(SDL_SCANCODE_Y))
  {
    scale += 1.f * gTime.deltaTime_s();
    gameObject->Transform()->SetScale(1.f, scale, 1.f);
  }
  if(gUserInput.Keyboard(SDL_SCANCODE_H))
  {
    scale -= 1.f * gTime.deltaTime_s();
    gameObject->Transform()->SetScale(1.f, scale, 1.f);
  }
  cout << " ROTATION: " << gameObject->Transform()->Rotation() << endl;
  cout << "LROTATION: " << gameObject->Transform()->LRotation() << endl;
}

void Cubo_second_cout(CGameObject* gameObject)
{
  cout << "Cubo_second GPOS: " << gameObject->Transform()->Position() << endl;
  cout << "Cubo_second LPOS: " << gameObject->Transform()->LPosition() << endl;
  cout << "----------------------------------------------------" << endl;
}

void Cubo_third_behaviour(CGameObject* gameObject)
{
  gameObject->Transform()->LookAt(gGameObjects["camara_main"]->Transform()->Position());
}

void Camara_second_movimiento(CGameObject* gameObject)
{
  static bool target = false;
  if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
  {
    if(!target)
      gameObject->Camera()->SetTarget(gGameObjects["cubo_second"]->GetName());
    else
      gameObject->Camera()->UnSetTarget();

    target = !target;
  }
}

void Hada_movimiento(CGameObject* gameObject)
{
  SetTransformByVar(gameObject, "GO_HADA");
}

void Particle_emitter_explosions(CGameObject* gameObject)
{
  if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
  {
    gameObject->Transform()->position = gMath.random_point(vector3f(10, 10, 0), vector3f(-5, -5, 0));
    gameObject->ParticleEmitter()->Start();
    //gameObject->particleEmitter()->Stop();
  }
}


