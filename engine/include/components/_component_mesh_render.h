#ifndef __COMPONENT_MESH_RENDER_H_
#define __COMPONENT_MESH_RENDER_H_

#include "_globals.h"
#include "components/_component.h"

class CComponent_Mesh_Render: public CComponent
{
  public:
    friend class CGameObject;

    string mesh_name, material_name;
    //vector<string> materials;
    // Guardar colores en un vector, o algo por el estilo...
    colorf_t color;

    // Modo difuso, etc etc

  private:
    static int GetID() { return components::mesh_render; }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & boost::serialization::base_object<CComponent>(*this);
    }


  public:
    CComponent_Mesh_Render(){};
    CComponent_Mesh_Render(CGameObject* gameObject);
    ~CComponent_Mesh_Render();

    void OnRender();

  public:

};

BOOST_CLASS_EXPORT_KEY( CComponent_Mesh_Render );


#endif /* __COMPONENT_MESH_RENDER_H_ */
