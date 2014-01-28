/*
 * random_stuff.cpp
 *
 *  Created on: 24/11/2013
 *      Author: Dani
 */

#include "render/mesh.h"

bool SetGameObjects_Instance1_Mesh()
{
  CGameObject* hada = gGameObjects.AddGameObject("hada");

  hada->Transform()->SetScale(0.1f, 0.1f, 0.1f);
  hada->Transform()->Translate(0.f, 0.f, 10.f);
  hada->Transform()->Rotate(0.f, 90.f, 0.f);

  hada->MeshRender()->mesh_name = "mdl_hada1";
  hada->MeshRender()->material_name = "texture_mdl_hada1";

  return true;
}
