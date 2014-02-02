/*
 * random_stuff.cpp
 *
 *  Created on: 24/11/2013
 *      Author: Dani
 */

#include "render/mesh.h"

bool SetGameObjects_Instance1_Mesh()
{
  CGameObject* hada1 = gGameObjects.AddGameObject("hada1");
  CGameObject* hada2 = gGameObjects.AddGameObject("hada2");

  hada1->Transform()->SetScale(0.1f, 0.1f, 0.1f);
  hada1->Transform()->Translate(0.f, 0.f, 10.f);
  hada1->Transform()->Rotate(0.f, 90.f, 0.f);

  hada1->MeshRender()->mesh_name = "mdl_hada1";
  hada1->MeshRender()->material_name = "texture_mdl_hada1";
  hada1->MeshRender()->color(1.0, 0.9f, 0.9f, 0.25f);

  hada2->Transform()->SetScale(0.1f, 0.1f, 0.1f);
  hada2->Transform()->Translate(0.f, 0.f, 8.f);
  hada2->Transform()->Rotate(0.f, 90.f, 0.f);

  hada2->MeshRender()->mesh_name = "mdl_hada1";
  hada2->MeshRender()->material_name = "texture_mdl_hada1";
  hada2->MeshRender()->color(1.0, 0.9f, 0.9f, 0.25f);


  return true;
}
