#include "components/_component_mesh_render.h"
#include "_systems.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Mesh_Render);

CComponent_Mesh_Render::CComponent_Mesh_Render(CGameObject* gameObject): CComponent(gameObject)
{
  mesh_name = material_name = "";
  //materials.resize(0);
  color(1.f, 1.f, 1.f, 1.f);
}

CComponent_Mesh_Render::~CComponent_Mesh_Render()
{
  //materials.resize(0);
}

void CComponent_Mesh_Render::OnRender()
{
  glColor3f(color.r, color.g, color.b);
  if(material_name != "") glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture(material_name)->GetID());

  CResource_Mesh* mesh = gSystem_Resources.GetMesh(mesh_name);
  if(mesh) mesh->Render();
}
