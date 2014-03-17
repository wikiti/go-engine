#ifndef __COMPONENT_TRANSFORM_H_
#define __COMPONENT_TRANSFORM_H_

#include "_globals.h"
#include "systems/_other.h"
#include "components/_component.h"

class CComponent_Transform: public CComponent
{
  friend class CGameObject;
  friend class CSystem_Render;
  friend class CSystem_Debug;

  friend const char* components::component_to_string(components c);
  friend int components::string_to_component(const string& c);

  public:
    vector3f position, scale;
    glm::quat angle;

    //glm::mat4 transformMatrix;

  private:
    //vector3f angle_vector;
    static GLuint m_TransformVBOVertices;
    static GLuint m_TransformVBOColors;
    static GLuint m_TransformVAO;

    static int GetID() { return components::transform; }

    void parseDebug(string command);

    /*friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      //ar & boost::serialization::base_object<CComponent>(*this);
      //ar & position &  angle & scale;
    }*/

  protected:
    //component_transform_t data;
    void ApplyParentTransform(CGameObject* parent, glm::mat4& transformMatrix);



    //inline virtual bool AddFuncs(CGameObject* obj);
    //inline virtual bool RemoveFuncs(CGameObject* obj);

  public:
    CComponent_Transform(){};
    CComponent_Transform(CGameObject* gameObject);
    ~CComponent_Transform();

    inline virtual void Set(input_t data);
    inline virtual output_t Get();

    void OnRender(glm::mat4 modelViewMatrix, glm::mat4 projMatrix);
    static bool InitRenderVBO();
    static void CloseRenderVBO();

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

    // Transformaciones locales ¿?
    void LTranslate(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    void LRotate(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0); // <- ANGULOS DE EULER Y CUATERNIONES!!!
    //void LScale(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);

    vector3f EulerAngles();

    // Problemática: Rotaciones locales y globales: las rotaciones "globales" funcionan como pseudolocales
    //  -> Usar cuaterniones, ya que la rotación de un eje altera el resto de ejes.
    // Problemática: Gimbal Lock
    //  -> Usar cuaterniones en vez de ángulos de eulers, y mostrar al usuario los ángulos como ángulos de euler
    //  -> Set: Euler a Cuaterniones
    //  -> Get: Cuaterniones a Euler
    //  -> Se trabaja internamente con cuaterniones. No se podrá modificar los ángulos de euler a mano (angle.x ...), ya que internamente no existen. Se usará el set y el get exclusivamente.
    //  -> http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
    //     -> Nota: se puede usar glRotatef con un cuaternion?
    void Translate(vector3f v);
    void Translate(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    void SetPosition(vector3f v);
    void SetPosition(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    void Rotate(vector3f v);
    void Rotate(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    void SetAngle(vector3f v);
    void SetAngle(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    void Scale(vector3f v);
    void Scale(GLfloat x = 1.f, GLfloat y = 1.f, GLfloat z = 1.f);
    void SetScale(vector3f v);
    void SetScale(GLfloat x = 1.f, GLfloat y = 1.f, GLfloat z = 1.f);

    /*inline void LookAt(vector3f target)
    {
      LookAt(target.x, target.y, target.z);
    }*/
    void LookAt(vector3f target, vector3f up_vector = gMath.Y_AXIS, vector3f forward_vector = gMath.Z_AXIS);
    void LookAt(GLfloat x, GLfloat y, GLfloat z, vector3f up_vector = gMath.Y_AXIS, vector3f forward_vector = gMath.Z_AXIS);

    glm::mat4 ApplyTransform(const glm::mat4& modelviewMatrix);

    inline void NormalizeAngles()
    {
      if(angle.x < 0 ) angle.x = 360 + angle.x;
      if(angle.y < 0 ) angle.y = 360 + angle.y;
      if(angle.z < 0 ) angle.z = 360 + angle.z;

      if(angle.x >= 360 ) angle.x = angle.x - 360;
      if(angle.y >= 360 ) angle.y = angle.y - 360;
      if(angle.z >= 360 ) angle.z = angle.z - 360;
    }

    // Esto debería ir en CSystem_Math

    vector3f up();
    vector3f left();
    vector3f forward();

    GLfloat pitch();
    GLfloat yaw();
    GLfloat roll();

    // Pitch, yaw, roll
    vector3f Rotation();
    vector3f LRotation();

//    Por hacer: Posiciones globales y locales
    vector3f_t Position();
//    vector3f_t Angle();
//    vector3f_t Scale();
//
    vector3f_t LPosition()
    {
      return position;
    }
//    vector3f_t LAngle();
//    vector3f_t LScale();
};

//BOOST_CLASS_EXPORT_KEY( CComponent_Transform );


#endif /* __COMPONENT_TRANSFORM_H_ */
