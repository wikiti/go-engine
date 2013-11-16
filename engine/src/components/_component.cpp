#include "components/_component.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CComponent);

const char* components::component_to_string(components c)
{
  switch(c)
  {
    case base: return "Base"; break;
    case camera: return "Camera"; break;
    case dummy1: return "Dummy1"; break;
    case transform: return "Transform"; break;
    default: return "Not defined"; break;
  }
  return NULL;
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
