/*
 * general.cpp
 *
 *  Created on: 25/04/2014
 *      Author: Dani
 */

#include "presentacion/general.h"
#include "presentacion/gestor_entrada.h"
#include "presentacion/gestores.h"

vector<slide_f> diapositivas_funcs_init;
vector<slide_f> diapositivas_funcs_close;

int diapositiva_actual = 0;

bool Presentacion_Init()
{
  // Iniciar cámara
  CGameObject* camara_main = gGameObjects["camara_main"];

  if(!camara_main)
  {
    camara_main = gGameObjects.Add("camara_main");

    camara_main->Camera()->far_clip = 100.f;
    camara_main->Preserve();
    camara_main->SetInputFunction(&Camara_main_input);
    camara_main->SetBehaviourFunction(&Camara_main_rotacion);
    gRender.AddCamera(camara_main);
  }
  camara_main->Camera()->skybox_texture = "skybox3";

  // Cámara de marca de agua
  CGameObject* marca_agua = gGameObjects["marca_agua"];

  if(!marca_agua)
  {
    marca_agua = gGameObjects.Add("marca_agua");
    marca_agua->GUITexture()->texture_name = "watermark";

    marca_agua->Transform()->position.x = gData.GetInt("__RENDER_RESOLUTION_WIDTH")/2;
    marca_agua->Transform()->position.y = (gData.GetInt("__RENDER_RESOLUTION_HEIGHT")*0.1)/2;

    marca_agua->GUITexture()->width = gData.GetInt("__RENDER_RESOLUTION_WIDTH");
    marca_agua->GUITexture()->height = gData.GetInt("__RENDER_RESOLUTION_HEIGHT") * 0.1; // 5% de pantalla

    marca_agua->Preserve();
  }

  diapositivas_funcs_init.resize(NUM_DIAPOSITIVAS, NULL);
  diapositivas_funcs_close.resize(NUM_DIAPOSITIVAS, NULL);

  // Gestor de eventos para mover la cámara (y tal)
  CGameObject* gestor_entrada = gGameObjects.Add("gestor_entrada");
  gestor_entrada->SetInputFunction(&Gestor_Entrada_Input);

  // Iniciar slides
  Slide1_Init();
  diapositivas_funcs_init[0] = &Slide1_Init;
  diapositivas_funcs_close[0] = &Slide1_Close;
  Slide2_Init();
  diapositivas_funcs_init[1] = &Slide2_Init;
  diapositivas_funcs_close[1] = &Slide2_Close;

  diapositivas_funcs_init[2] = &Slide3_Init;
  diapositivas_funcs_close[2] = &Slide3_Close;
  diapositivas_funcs_init[3] = &Slide4_Init;
  diapositivas_funcs_close[3] = &Slide4_Close;
  diapositivas_funcs_init[4] = &Slide5_Init;
  diapositivas_funcs_close[4] = &Slide5_Close;
  diapositivas_funcs_init[5] = &Slide6_Init;
  diapositivas_funcs_close[5] = &Slide6_Close;
  diapositivas_funcs_init[6] = &Slide7_Init;
  diapositivas_funcs_close[6] = &Slide7_Close;
  diapositivas_funcs_init[7] = &Slide8_Init;
  diapositivas_funcs_close[7] = &Slide8_Close;
  diapositivas_funcs_init[8] = &Slide9_Init;
  diapositivas_funcs_close[8] = &Slide9_Close;
  diapositivas_funcs_init[9] = &Slide10_Init;
  diapositivas_funcs_close[9] = &Slide10_Close;
  // ...

  return true;
}

bool Slide1_Init()
{
  // Título
  CGameObject* titulo = gGameObjects.Add("slide1_titulo");
  //slide1_titulo->Transform()->position.y += 10;
  titulo->Transform()->Translate(0, 0.6, 4);
  titulo->Transform()->Rotate(0, 180, 0);
  titulo->MeshRender()->material_name = "__WHITE_TEXTURE";
  titulo->MeshRender()->mesh_name = "mdl_slide1_title";
  titulo->MeshRender()->color = COLOR_TITULO;

  // Engranaje que gira
  CGameObject* engranaje = gGameObjects.Add("slide1_engranaje");
  titulo->Transform()->Translate(0, 0, 5);
  engranaje->MeshRender()->material_name = "__WHITE_TEXTURE";
  engranaje->MeshRender()->color = COLOR_TITULO;

  return true;
}

bool Slide1_Close()
{
  gGameObjects.Delete("slide1_titulo");
  gGameObjects.Delete("slide1_engranaje");

  return true;
}

bool Slide2_Init()
{
  // Título
  CGameObject* titulo = gGameObjects.Add("slide2_titulo");
  titulo->Transform()->Translate(-6, 1.4, 0);
  titulo->Transform()->Rotate(0, 90, 0);
  titulo->MeshRender()->material_name = "__WHITE_TEXTURE";
  titulo->MeshRender()->mesh_name = "mdl_slide2_title";
  titulo->MeshRender()->color = COLOR_TITULO;

  // Texto
  CGameObject* texto = gGameObjects.Add("slide2_texto");
  texto->Transform()->Translate(-12, 1.5, 6);
  texto->Transform()->Rotate(0, 90, 0);
  texto->MeshRender()->material_name = "__WHITE_TEXTURE";
  texto->MeshRender()->mesh_name = "mdl_slide2_text";
  texto->MeshRender()->color = COLOR_TEXTO;

  // Crate
  CGameObject* crate = gGameObjects.Add("slide2_crate");
  crate->Transform()->Translate(-12, -2, 4.2);
  crate->Transform()->Rotate(0, 35, 0);
  crate->MeshRender()->material_name = "texture_crate";
  crate->MeshRender()->mesh_name = "mdl_crate";

  // Fire
  CGameObject* fire = gGameObjects.Add("slide2_fire");

  //fire->transform()->position.x += 3.f;
  fire->Transform()->Translate(-5, -1.3, -2);
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

bool Slide2_Close()
{
  gGameObjects.Delete("slide2_titulo");
  gGameObjects.Delete("slide2_texto");
  gGameObjects.Delete("slide2_crate");
  gGameObjects.Delete("slide2_fire");

  return true;
}

bool Slide3_Init()
{
  // Título
  CGameObject* titulo = gGameObjects.Add("slide3_titulo");
  titulo->Transform()->Translate(0, 2.6, -9);
  titulo->Transform()->Rotate(0, 0, 0);
  titulo->MeshRender()->material_name = "__WHITE_TEXTURE";
  titulo->MeshRender()->mesh_name = "mdl_slide3_title";
  titulo->MeshRender()->color = COLOR_TITULO;

  // Texto
  CGameObject* texto = gGameObjects.Add("slide3_texto");
  texto->Transform()->Translate(1.5, 1, -12);
  texto->Transform()->Rotate(0, 0, 0);
  texto->MeshRender()->material_name = "__WHITE_TEXTURE";
  texto->MeshRender()->mesh_name = "mdl_slide3_text";
  texto->MeshRender()->color = COLOR_TEXTO;

  // Planeta1
  CGameObject* Planeta1 = gGameObjects.Add("Planeta1");
  Planeta1->Transform()->Translate(-4, -0.3, -10);
  Planeta1->Transform()->Rotate(0, 0, 35);
  Planeta1->MeshRender()->material_name = "texture_dirt";
  Planeta1->MeshRender()->mesh_name = "mdl_planet";
  Planeta1->MeshRender()->color(0.4, 0, 0.16, 1);
  Planeta1->SetBehaviourFunction(&Planeta_Movimiento);

  // Planeta2
  CGameObject* Planeta2 = gGameObjects.Add("Planeta2");
  Planeta2->Transform()->Translate(3, 0, 0);
  Planeta2->Transform()->Rotate(0, 0, 15);
  Planeta2->Transform()->SetScale(0.5, 0.5, 0.5);
  Planeta2->MeshRender()->material_name = "texture_dirt";
  Planeta2->MeshRender()->mesh_name = "mdl_planet";
  Planeta2->MeshRender()->color(0.82, 0.52, 0.24, 1);
  Planeta2->SetBehaviourFunction(&Planeta_Movimiento);
  Planeta1->AddChild(Planeta2);

  // Planeta3
  CGameObject* Planeta3 = gGameObjects.Add("Planeta3");
  Planeta3->Transform()->Translate(2, 0, 0);
  Planeta3->Transform()->Rotate(0, 0, 0);
  Planeta3->Transform()->SetScale(0.3, 0.3, 0.3);
  Planeta3->MeshRender()->material_name = "texture_dirt";
  Planeta3->MeshRender()->mesh_name = "mdl_planet";
  Planeta3->MeshRender()->color(0.6, 0.6, 0.6, 1);
  Planeta3->SetBehaviourFunction(&Planeta_Movimiento);
  Planeta2->AddChild(Planeta3);

  return true;
}

bool Slide3_Close()
{
  gGameObjects.Delete("slide3_titulo");
  gGameObjects.Delete("slide3_texto");
  gGameObjects.Delete("Planeta1", true);

  return true;
}

bool Slide4_Init()
{
  // Título
  CGameObject* titulo = gGameObjects.Add("slide4_titulo");
  titulo->Transform()->Translate(10, 2.5, 0);
  titulo->Transform()->Rotate(0, 270, 0);
  titulo->MeshRender()->material_name = "__WHITE_TEXTURE";
  titulo->MeshRender()->mesh_name = "mdl_slide4_title";
  titulo->MeshRender()->color = COLOR_TITULO;

  // Texto
  CGameObject* texto = gGameObjects.Add("slide4_texto");
  texto->Transform()->Translate(12, 1, -8);
  texto->Transform()->Rotate(0, 270, 0);
  texto->MeshRender()->material_name = "__WHITE_TEXTURE";
  texto->MeshRender()->mesh_name = "mdl_slide4_text";
  texto->MeshRender()->color = COLOR_TEXTO;

  // Crate
  CGameObject* crate = gGameObjects.Add("slide4_crate");
  crate->Transform()->Translate(12, 0, 5);
  crate->MeshRender()->material_name = "texture_crate";
  crate->MeshRender()->mesh_name = "mdl_crate";
  crate->SetBehaviourFunction(&Crate_Movimiento);

  return true;
}

bool Slide4_Close()
{
  gGameObjects.Delete("slide4_titulo");
  gGameObjects.Delete("slide4_texto");
  gGameObjects.Delete("slide4_crate");

  return true;
}

bool Slide5_Init()
{
  // Título
  CGameObject* titulo = gGameObjects.Add("slide5_titulo");
  titulo->Transform()->Translate(0, 2, 8);
  titulo->Transform()->Rotate(0, 180, 0);
  titulo->MeshRender()->material_name = "__WHITE_TEXTURE";
  titulo->MeshRender()->mesh_name = "mdl_slide5_title";
  titulo->MeshRender()->color = COLOR_TITULO;

  // Texto
  CGameObject* texto = gGameObjects.Add("slide5_texto");
  texto->Transform()->Translate(0, -3.5, 15);
  texto->Transform()->Rotate(0, 180, 0);
  texto->MeshRender()->material_name = "__WHITE_TEXTURE";
  texto->MeshRender()->mesh_name = "mdl_slide5_text";
  texto->MeshRender()->color = COLOR_TEXTO;

  // Mesh
  CGameObject* mesh = gGameObjects.Add("slide5_mesh");

  mesh->Transform()->SetScale(0.1f, 0.1f, 0.1f);
  mesh->Transform()->Translate(7.5, 0, 15);
  mesh->Transform()->Rotate(0.f, 90.f, 0.f);

  mesh->MeshRender()->mesh_name = "mdl_hada1";
  mesh->MeshRender()->material_name = "texture_mdl_hada1";

  // Particles
  CGameObject* particles = gGameObjects.Add("slide5_particles");
  particles->Transform()->Translate(0.5, -1, 20);

  CGameObject* rainbow_violet = gGameObjects.Add("rainbow_violet");
  CGameObject* rainbow_dblue = gGameObjects.Add("rainbow_dblue");
  CGameObject* rainbow_blue = gGameObjects.Add("rainbow_blue");
  CGameObject* rainbow_green = gGameObjects.Add("rainbow_green");
  CGameObject* rainbow_yellow = gGameObjects.Add("rainbow_yellow");
  CGameObject* rainbow_orange = gGameObjects.Add("rainbow_orange");
  CGameObject* rainbow_red = gGameObjects.Add("rainbow_red");

  particles->AddChildren( {rainbow_violet, rainbow_dblue, rainbow_blue, rainbow_green,
      rainbow_yellow, rainbow_orange, rainbow_red});

  rainbow_violet->Transform()->position.x = 3.f;
  rainbow_dblue->Transform()->position.x = 2.5f;
  rainbow_blue->Transform()->position.x = 2.f;
  rainbow_green->Transform()->position.x = 1.5f;
  rainbow_yellow->Transform()->position.x = 1.f;
  rainbow_orange->Transform()->position.x = 0.5f;

  // velocidades
  rainbow_violet->ParticleEmitter()->start_min_vel = rainbow_violet->ParticleEmitter()->start_max_vel = 8.8f;
  rainbow_dblue->ParticleEmitter()->start_min_vel = rainbow_dblue->ParticleEmitter()->start_max_vel = 8.1f;
  rainbow_blue->ParticleEmitter()->start_min_vel = rainbow_blue->ParticleEmitter()->start_max_vel = 7.5f;
  rainbow_green->ParticleEmitter()->start_min_vel = rainbow_green->ParticleEmitter()->start_max_vel = 6.8f;
  rainbow_yellow->ParticleEmitter()->start_min_vel = rainbow_yellow->ParticleEmitter()->start_max_vel = 6.f;
  rainbow_orange->ParticleEmitter()->start_min_vel = rainbow_orange->ParticleEmitter()->start_max_vel = 5.f;
  rainbow_red->ParticleEmitter()->start_min_vel = rainbow_red->ParticleEmitter()->start_max_vel = 4.f;


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

  rainbow_violet->ParticleEmitter()->Start();
  rainbow_dblue->ParticleEmitter()->Start();
  rainbow_blue->ParticleEmitter()->Start();
  rainbow_green->ParticleEmitter()->Start();
  rainbow_yellow->ParticleEmitter()->Start();
  rainbow_orange->ParticleEmitter()->Start();
  rainbow_red->ParticleEmitter()->Start();

  // Camera
  CGameObject* camera = gGameObjects.Add("slide5_camera");
  camera->Camera()->far_clip = 100.f;
  camera->Camera()->viewport.x = 0.7;
  camera->Camera()->viewport.y = 0.4;
  camera->Camera()->viewport.width = 0.25;
  camera->Camera()->viewport.height = 0.25;
  camera->Camera()->skybox_texture = "skybox1";
  camera->Camera()->before_render = &Camera_BeforeRender;
  camera->Camera()->after_render = &Camera_AfterRender;

  camera->Transform()->SetAngle();
  camera->Preserve();
  gRender.AddCamera(camera);

  // Desactivar por el momento
  camera->Disable();

  return true;
}

bool Slide5_Close()
{
  gGameObjects.Delete("slide5_titulo");
  gGameObjects.Delete("slide5_texto");

  gGameObjects.Delete("slide5_mesh");
  gGameObjects.Delete("slide5_particles", true);
  gRender.RemoveCamera("slide5_camera");
  gGameObjects["slide5_camera"]->RemoveComponent<CComponent_Camera>();
  gGameObjects.Delete("slide5_camera");

  return true;
}

bool Slide6_Init()
{
  // Título
  CGameObject* titulo = gGameObjects.Add("slide6_titulo");
  titulo->Transform()->Translate(-6, 1.5, 0);
  titulo->Transform()->Rotate(0, 90, 0);
  titulo->MeshRender()->material_name = "__WHITE_TEXTURE";
  titulo->MeshRender()->mesh_name = "mdl_slide6_title";
  titulo->MeshRender()->color = COLOR_TITULO;

  // Texto
  CGameObject* texto = gGameObjects.Add("slide6_texto");
  texto->Transform()->Translate(-12, 1.5, 6);
  texto->Transform()->Rotate(0, 90, 0);
  texto->MeshRender()->material_name = "__WHITE_TEXTURE";
  texto->MeshRender()->mesh_name = "mdl_slide6_text";
  texto->MeshRender()->color = COLOR_TEXTO;

  return true;
}

bool Slide6_Close()
{
  gGameObjects.Delete("slide6_titulo");
  gGameObjects.Delete("slide6_texto");

  return true;
}

bool Slide7_Init()
{
  // Título
  CGameObject* titulo = gGameObjects.Add("slide7_titulo");
  titulo->Transform()->Translate(0, 2.6, -9);
  titulo->Transform()->Rotate(0, 0, 0);
  titulo->MeshRender()->material_name = "__WHITE_TEXTURE";
  titulo->MeshRender()->mesh_name = "mdl_slide7_title";
  titulo->MeshRender()->color = COLOR_TITULO;

  // Texto
  CGameObject* texto = gGameObjects.Add("slide7_texto");
  texto->Transform()->Translate(-5, 2, -15);
  texto->Transform()->Rotate(0, 0, 0);
  texto->MeshRender()->material_name = "__WHITE_TEXTURE";
  texto->MeshRender()->mesh_name = "mdl_slide7_text";
  texto->MeshRender()->color = COLOR_TEXTO;

  return true;
}

bool Slide7_Close()
{
  gGameObjects.Delete("slide7_titulo");
  gGameObjects.Delete("slide7_texto");

  return true;
}

bool Slide8_Init()
{
  // Título
  CGameObject* titulo = gGameObjects.Add("slide8_titulo");
  titulo->Transform()->Translate(10, 2.5, 0);
  titulo->Transform()->Rotate(0, 270, 0);
  titulo->MeshRender()->material_name = "__WHITE_TEXTURE";
  titulo->MeshRender()->mesh_name = "mdl_slide8_title";
  titulo->MeshRender()->color = COLOR_TITULO;

  // Texto
  CGameObject* texto = gGameObjects.Add("slide8_texto");
  texto->Transform()->Translate(12, 1, -8);
  texto->Transform()->Rotate(0, 270, 0);
  texto->MeshRender()->material_name = "__WHITE_TEXTURE";
  texto->MeshRender()->mesh_name = "mdl_slide8_text";
  texto->MeshRender()->color = COLOR_TEXTO;

  // Wrench
  CGameObject* wrench = gGameObjects.Add("slide8_wrench");
  wrench->Transform()->Translate(12, 0, 5);
  wrench->Transform()->Rotate(0, 35, 90);
  wrench->Transform()->Scale(2, 2, 2);
  wrench->MeshRender()->material_name = "texture_wrench";
  wrench->MeshRender()->mesh_name = "mdl_wrench";
  wrench->SetBehaviourFunction(&Wrench_Movimiento);

  return true;
}

bool Slide8_Close()
{
  gGameObjects.Delete("slide8_titulo");
  gGameObjects.Delete("slide8_texto");
  gGameObjects.Delete("slide8_wrench");

  return true;
}

bool Slide9_Init()
{
  // Título
  CGameObject* titulo = gGameObjects.Add("slide9_titulo");
  titulo->Transform()->Translate(0, 2, 8);
  titulo->Transform()->Rotate(0, 180, 0);
  titulo->MeshRender()->material_name = "__WHITE_TEXTURE";
  titulo->MeshRender()->mesh_name = "mdl_slide9_title";
  titulo->MeshRender()->color = COLOR_TITULO;

  // Texto
  CGameObject* texto = gGameObjects.Add("slide9_texto");
  texto->Transform()->Translate(5, 1, 11);
  texto->Transform()->Rotate(0, 180, 0);
  texto->MeshRender()->material_name = "__WHITE_TEXTURE";
  texto->MeshRender()->mesh_name = "mdl_slide9_text";
  texto->MeshRender()->color = COLOR_TEXTO;

  return true;
}

bool Slide9_Close()
{
  gGameObjects.Delete("slide9_titulo");
  gGameObjects.Delete("slide9_texto");

  return true;
}


bool Slide10_Init()
{
  // Título
  CGameObject* titulo = gGameObjects.Add("slide10_titulo");
  titulo->Transform()->Translate(-8, 5, 0); // Elevado!
  titulo->Transform()->Rotate(45, 90, 0);
  titulo->MeshRender()->material_name = "__WHITE_TEXTURE";
  titulo->MeshRender()->mesh_name = "mdl_slide10_title";
  titulo->MeshRender()->color = COLOR_TITULO;

  // Gestor de fuegos artificiales ->BUG Crash sin razón aparente en el firework_manager de la presentación.
  CGameObject* firework_manager = gGameObjects.Add("firework_manager");
  firework_manager->Preserve();
  firework_manager->SetBehaviourFunction(&Firework_Manager_Comportamiento);
  firework_manager->Disable();

  if(gData.GetInt("firework_explosion_particles") == __CSYSTEM_DATA_STORAGE_NOINT)
    gData.SetInt("firework_explosion_particles", 500);

  return true;
}

bool Slide10_Close()
{
  gGameObjects.Delete("slide10_titulo");

  return true;
}
