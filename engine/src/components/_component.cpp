#include "components/_component.h"
#include "systems/_debug.h"

//BOOST_CLASS_EXPORT_IMPLEMENT(CComponent);

const char* components::components_s[] = {"base", "camera", "mesh_render", "particle_emitter", "gui_texture", "audio_source", "transform", "dummy1", "dummy2", "dummy3", "not_defined"};

const char* components::component_to_string(components c)
{
  if(c < base or c >= __not_defined)
    c = __not_defined;

  return components_s[c];
}

int components::string_to_component(const string& c)
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

void CComponent::parseDebug(string command)
{
  gSystem_Debug.console_error_msg("From component %s - %s: Unhandheld \"parseDebug\"", gameObject->GetName().c_str(), components::component_to_string( (components::components)this->GetID()) );
}

void CComponent::printDebug()
{
  gSystem_Debug.console_error_msg("From component %s - %s: Unhandheld \"printDebug\"", gameObject->GetName().c_str(), components::component_to_string( (components::components)this->GetID()) );
}
