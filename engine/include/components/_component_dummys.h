// Componentes de prueba

#ifndef __COMPONENT_CAMERA_DUMMYS_H_
#define __COMPONENT_CAMERA_DUMMYS_H_

#include "_globals.h"
#include "components/_component.h"

class CComponent_Dummy1: public CComponent
{
  private:
    friend class CSystem_Render;
    friend class CGameObject;

  public:
    GLfloat cube_side;

  private:
    static int GetID() { return components::dummy1; }

  protected:

  public:
    CComponent_Dummy1(){};
    CComponent_Dummy1(CGameObject* gameObject);

    ~CComponent_Dummy1(){};

    void OnRender();
};

class CComponent_Dummy2: public CComponent
{
  private:
    friend class CSystem_Render;
    friend class CGameObject;

  private:
    static int GetID() { return components::dummy2; }

  protected:

  public:
    CComponent_Dummy2(){};
    CComponent_Dummy2(CGameObject* gameObject): CComponent(gameObject){ };

    ~CComponent_Dummy2(){};

    void OnRender();
};

BOOST_CLASS_EXPORT_KEY( CComponent_Dummy1 );

#endif /* __COMPONENT_CAMERA_DUMMYS_H_ */
