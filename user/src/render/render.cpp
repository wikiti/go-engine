/*
 * random_stuff.cpp
 *
 *  Created on: 24/11/2013
 *      Author: Dani
 */

#include "render/render.h"

bool SetGameObjects_Instance1_Render()
{
  gShader.LoadShader("phong", "data/shaders/phong_vert.glsl", "data/shaders/phong_frag.glsl");
  gShader.LinkShader("phong");

  CGameObject* hada1 = gGameObjects.Add("hada1");

  hada1->Transform()->SetScale(0.1f, 0.1f, 0.1f);
  hada1->Transform()->Translate(0.f, 0.f, 10.f);
  hada1->Transform()->Rotate(0.f, 90.f, 0.f);

  hada1->MeshRender()->mesh_name = "mdl_hada1";
  hada1->MeshRender()->material_name = "texture_mdl_hada1";
  hada1->MeshRender()->color(1.0, 0.f, 0.f, 1.f);

  CGameObject* texto1 = gGameObjects.Add("texto1");
  texto1->Transform()->SetScale(1.f, 1.f, 1.f);
  texto1->Transform()->SetAngle(90.f, 180.f, 0.5f);
  texto1->MeshRender()->mesh_name = "mdl_texto1";
  texto1->MeshRender()->material_name = "__WHITE_TETURE";
  texto1->MeshRender()->color(0.f, 1.f, 0.25f, 1.f);
  texto1->MeshRender()->shader_name = "phong";

  /*CGameObject* gui_example = gGameObjects.AddGameObject("gui_example");
  gui_example->GUITexture()->texture_name = "textura1";
  gui_example->GUITexture()->width = gui_example->GUITexture()->height = 0.5f;
  gui_example->Transform()->position(0.5f, 0.5f, 0.f);
  //gui_example->Transform()->Rotate(0.f, 0.f, 45.f);
  gui_example->GUITexture()->pixel_offset_x = 2;
  gui_example->GUITexture()->color.a = 0.5f;

  gui_example->GUITexture()->color.g = gui_example->GUITexture()->color.b = 0.f;*/

  return true;
}
