#include "components/_component.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CComponent);

const char* components::components_s[] = {"base", "camera", "mesh_render", "particle_emitter", "dummy1", "dummy2", "transform", "not_defined"};

const char* components::component_to_string(components c)
{
  if(c < base or c >= __not_defined)
    c = __not_defined;

  return components_s[c];
}

int components::string_to_component(string c)
{
  for(uint i = 0; i < components::__not_defined; i++)
    if(c == components_s[i])
      return i;

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
