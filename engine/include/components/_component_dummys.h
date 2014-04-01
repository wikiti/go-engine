/**
 * @file
 * @brief Fichero para los componentes de pruebas.
 */

#ifndef __COMPONENT_CAMERA_DUMMYS_H_
#define __COMPONENT_CAMERA_DUMMYS_H_

#include "_globals.h"
#include "components/_component.h"

#include "systems/_other.h"

/** @addtogroup Componentes */
/*@{*/

/**
 * @brief Componente de pruebas.
 *
 * Componente de pruebas. Usado para testear cosas rápidas sin necesidad de definir nuevos componentes.
 */
class CComponent_Dummy: public CComponent
{
  private:
    friend class CSystem_Render;
    friend class CGameObject;

  private:
    static int GetID() { return Components::dummy; }

    vector3f direction;
    vector3f current_random;
    vector3f another_random;

    std::vector<vector3f> random_array;

    float angle;

  protected:

  public:
    CComponent_Dummy()
    {
      angle = 45;
      direction = current_random = vector3f(0.f, 1.f, 0.f);
      another_random(1.f, 0.f, 0.f);
      random_array.resize(1000);
      for(uint i = 0; i < 1000; i++)
        random_array[i] = gMath.random_vector(direction, angle);
    };

    CComponent_Dummy(CGameObject* gameObject): CComponent(gameObject)
    {
      angle = 45;
      direction = current_random = vector3f(0.f, 1.f, 0.f);
      another_random(1.f, 0.f, 0.f);

      random_array.resize(1000);
      for(uint i = 0; i < 1000; i++)
        random_array[i] = gMath.random_vector(direction, angle);
    };

    ~CComponent_Dummy(){};

    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);
    void OnEvent();
};

/*@}*/


#endif /* __COMPONENT_CAMERA_DUMMYS_H_ */
