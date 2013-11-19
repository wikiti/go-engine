#include "components/_component.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CComponent);

const char* components::components_s[] = {"base", "camera", "mesh_render", "dummy1", "dummy2", "transform", "not_defined"};

const char* components::component_to_string(components c)
{
  if(c < base or c >= __not_defined)
    c = __not_defined;

  return components_s[c];
}

int components::string_to_component(string c)
{
       if(c == "base")             return base;
  else if(c == "camera")           return camera;
  else if(c == "dummy1")           return dummy1;
  else if(c == "dummy2")           return dummy2;
  else if(c == "transform")        return transform;
  else if(c == "mesh_render")      return mesh_render;


  return __not_defined;
}

CComponent::CComponent(CGameObject* go)
{
  gameObject = go;
  //id = ccomponents::base;
  enabled = true;
}

CComponent::~CComponent()
{

}
