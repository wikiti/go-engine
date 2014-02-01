// Componentes de prueba

#ifndef __COMPONENT_CAMERA_DUMMYS_H_
#define __COMPONENT_CAMERA_DUMMYS_H_

#include "_globals.h"
#include "components/_component.h"

#include "systems/_other.h"

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

    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);
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

    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);
};

class CComponent_Dummy3: public CComponent
{
  private:
    friend class CSystem_Render;
    friend class CGameObject;

  private:
    static int GetID() { return components::dummy3; }

    vector3f direction;
    vector3f current_random;
    vector3f another_random;

    vector<vector3f> random_array;

    float angle;

  protected:

#define __RANDOM_ARRAY_SIZE 1000

  public:
    CComponent_Dummy3()
    {
      angle = 45;
      direction = current_random = vector3f(0.f, 1.f, 0.f);
      another_random(1.f, 0.f, 0.f);
      random_array.resize(__RANDOM_ARRAY_SIZE);
      for(uint i = 0; i < __RANDOM_ARRAY_SIZE; i++)
        random_array[i] = gMath.random_vector(direction, angle);
    };

    CComponent_Dummy3(CGameObject* gameObject): CComponent(gameObject)
    {
      angle = 45;
      direction = current_random = vector3f(0.f, 1.f, 0.f);
      another_random(1.f, 0.f, 0.f);

      random_array.resize(__RANDOM_ARRAY_SIZE);
      for(uint i = 0; i < __RANDOM_ARRAY_SIZE; i++)
        random_array[i] = gMath.random_vector(direction, angle);
    };

    ~CComponent_Dummy3(){};

    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);
    void OnEvent();
};

BOOST_CLASS_EXPORT_KEY( CComponent_Dummy1 );

#endif /* __COMPONENT_CAMERA_DUMMYS_H_ */
