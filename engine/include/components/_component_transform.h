#ifndef __COMPONENT_TRANSFORM_H_
#define __COMPONENT_TRANSFORM_H_

#include "_globals.h"
#include "components/_component.h"

class CComponent_Transform: public CComponent
{
  public:
    vector3f position, angle, scale;

  public:
    friend class CGameObject;

  private:
    //vector3f angle_vector;

    static int GetID() { return components::transform; }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & boost::serialization::base_object<CComponent>(*this);
      ar & position &  angle & scale;
    }

  protected:
    //component_transform_t data;
    void ApplyParentTransform(CGameObject* parent);

    //inline virtual bool AddFuncs(CGameObject* obj);
    //inline virtual bool RemoveFuncs(CGameObject* obj);

  public:
    CComponent_Transform(){};
    CComponent_Transform(CGameObject* gameObject);
    ~CComponent_Transform();

    inline virtual void Set(input_t data);
    inline virtual output_t Get();

    void OnRender();

  public:
    // Movimientos locales o globales al eje x,y,z
    // Cambia mucha la cosa:
    //    Por ejemplo, mover rotar un objeto con hijos... ¿dónde acaban los hijos?
    //      -> Posición de los hijos LOCAL al padre (calculada o guardada) <- DECISIÓN
    //          -> Se deberá calcular de manera recursiva hacia arriba hasta llegar al mayor padre sin hijos.
    //          -> No hace falta calcular la posición y luego hacer la transformación
    //              -> Se realizarán las transformaciones recursivamente desde arriba hasta abajo, sin calcular posiciones
    // ¿qué tal un booleano para saber cuando se ha renderizado un hijo? Así, las transformaciones se realizan en cascada de arriba a abajo, pasando por todos los hijos
    //    -> Problematico? Y si no usamos DEPTH_TEST? Mejor dejarlo así

    void Translate(vector3f v);
    void Translate(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    void SetPosition(vector3f v);
    void SetPosition(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    void Rotate(vector3f v);
    void Rotate(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    void SetAngle(vector3f v);
    void SetAngle(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    void Scale(vector3f v);
    void Scale(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    void SetScale(vector3f v);
    void SetScale(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);

    void ApplyTransform();

    inline void NormalizeAngles()
    {
      if(angle.x < 0 ) angle.x = 360 + angle.x;
      if(angle.y < 0 ) angle.y = 360 + angle.y;
      if(angle.z < 0 ) angle.z = 360 + angle.z;

      if(angle.x >= 360 ) angle.x = angle.x - 360;
      if(angle.y >= 360 ) angle.y = angle.y - 360;
      if(angle.z >= 360 ) angle.z = angle.z - 360;
    }
};

BOOST_CLASS_EXPORT_KEY( CComponent_Transform );


#endif /* __COMPONENT_TRANSFORM_H_ */
