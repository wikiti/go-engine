#include "components/_component.h"
#include "systems/_debug.h"

//BOOST_CLASS_EXPORT_IMPLEMENT(CComponent);

using namespace std;

const char* Components::components_s[] = {"base", "camera", "mesh_render", "particle_emitter", "gui_texture", "audio_source", "transform", "dummy", "not_defined"};

const char* Components::component_to_string(components_t c)
{
  if(c < base or c >= __component_not_defined)
    c = __component_not_defined;

  return components_s[c];
}

Components::components_t Components::string_to_component(const std::string& c)
{
  for(uint i = 0; i < Components::__component_not_defined; i++)
    if(c == components_s[i])
      return (Components::components_t)i;

  return __component_not_defined;
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
  gSystem_Debug.console_error_msg("From component %s - %s: Unhandheld \"parseDebug\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)this->GetID()) );
}

void CComponent::printDebug()
{
  gSystem_Debug.console_error_msg("From component %s - %s: Unhandheld \"printDebug\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)this->GetID()) );
}
