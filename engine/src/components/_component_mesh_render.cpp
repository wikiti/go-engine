#include "components/_component_mesh_render.h"
#include "systems/_resource.h"
#include "systems/_shader.h"

//BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Mesh_Render);

CComponent_Mesh_Render::CComponent_Mesh_Render(CGameObject* gameObject): CComponent(gameObject)
{
  mesh_name = material_name = "";
  shader_name = "__textureShader";
  before_render = after_render = NULL;

  //materials.resize(0);
  color(1.f, 1.f, 1.f, 1.f);
  color_apply_force = 1.f;
}

CComponent_Mesh_Render::~CComponent_Mesh_Render()
{
  //materials.resize(0);
}

void CComponent_Mesh_Render::OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix)
{
  if(!enabled) return;

  //glColor3f(color.r, color.g, color.b); // <-- deprecated!
  glActiveTexture(GL_TEXTURE0);
  if(material_name != "") glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture(material_name)->GetID());
  else                    glBindTexture(GL_TEXTURE_2D, 0);

  // Guardar el shader dentro del mesh render!
  //CShader* simpleShader = gSystem_Shader_Manager.GetShader(shader_name);
  //glUseProgram(simpleShader->GetProgram());
  CShader* simpleShader = gSystem_Shader_Manager.UseShader(shader_name);

  glUniformMatrix4fv(simpleShader->GetUniformIndex("ProjMatrix") , 1, GL_FALSE, glm::value_ptr(projMatrix));
  glUniformMatrix4fv(simpleShader->GetUniformIndex("ModelViewMatrix") , 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
  glUniform1i(simpleShader->GetUniformIndex( "texture"), 0);
  if(color_apply_force > 1) color_apply_force = 1.f;
  if(color_apply_force < 0) color_apply_force = 0.f;
  glUniform1f(simpleShader->GetUniformIndex("textureFlag"), color_apply_force);
  glUniform4f(simpleShader->GetUniformIndex("in_Color"), color.r, color.g, color.b, color.a);

  if(before_render) before_render(gameObject);

  if(color.a != 1.0)
  {
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    //glEnable(GL_BLEND);
  }


  CResource_Mesh* mesh = gSystem_Resources.GetMesh(mesh_name);
  if(mesh) mesh->Render();

  glDepthMask(GL_TRUE);
  //glDisable(GL_BLEND);

  if(after_render) after_render(gameObject);



  //glUseProgram(0);
}
