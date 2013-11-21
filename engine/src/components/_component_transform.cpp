#include "components/_component_transform.h"
#include "_systems.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Transform);

CComponent_Transform::CComponent_Transform(CGameObject* gameObject): CComponent(gameObject)
{
  //id = ccomponents::transform;

  position.x = position.y = position.z = 0;
  //angle.x = angle.y = angle.z = 0;
  scale.x = scale.y = scale.z = 1.f;

  //glm::vec3 EulerAngles(0, 0, 0);
  //angle = glm::quat(EulerAngles);
  //angle = glm::quat;
  //SetAngle(0, 0, 0);
}

CComponent_Transform::~CComponent_Transform()
{

}

void CComponent_Transform::Set(input_t d)
{
  (*this) = *(CComponent_Transform*)d;
}

output_t CComponent_Transform::Get()
{
  return (void*)this; // ?
}

void CComponent_Transform::OnRender()
{
  if(!enabled) return;

  glBindTexture(GL_TEXTURE_2D, 0);

  glBegin(GL_LINES);
    // X - Rojo
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(0.5f, 0.f, 0.f);

    // Y - Verde
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(0.f, 0.5f, 0.f);

    // Z - Azul
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(0.f, 0.f, 0.5f);
  glEnd();
}

vector3f CComponent_Transform::EulerAngles()
{
  /*glm::vec3 ea = glm::eulerAngles(angle);
  // Pasar a grados
  return vector3f(_RAD_TO_DEG(ea.x), _RAD_TO_DEG(ea.y), _RAD_TO_DEG(ea.z));*/
  glm::vec3 ea = glm::eulerAngles(angle);

  GLfloat nx = _RAD_TO_DEG(ea.x);
  GLfloat ny = _RAD_TO_DEG(ea.y);
  GLfloat nz = _RAD_TO_DEG(ea.z);

  NormalizeAngles(nx, ny, nz);
  return vector3f(nx, ny, nz);
  // Necesario?
}

void CComponent_Transform::LTranslate(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled) return;

  // Mover basandose en la rotación (cosa extraña)
  // Translate X
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // Orientación
  glMultMatrixf((const float*)glm::value_ptr(glm::toMat4(angle)));
  glTranslatef(x, y, z);

  GLdouble matrix[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, matrix);

  position.x += matrix[12];
  position.y += matrix[13];
  position.z += matrix[14];

  glPopMatrix();
}

void CComponent_Transform::LRotate(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled) return;

  // Como Rotate(), sólo que primero se aplica la rotación, y luego la orientación (mientras que en Rotate se aplica primero la orientación y luego la rotación).
  NormalizeAngles(x, y, z);

  glm::vec3 EulerAngles(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  angle = angle * glm::quat(EulerAngles);

  /* Basura
  glMatrixMode(GL_MODELVIEW); // <- ?
  glPushMatrix();
  glLoadIdentity(); // <- ?
  glm::quat QuatAroundX = glm::normalize(glm::quat( glm::vec3(_DEG_TO_RAD(x), 0.f, 0.f) ));
  glm::quat QuatAroundY = glm::normalize(glm::quat( glm::vec3(0.f, _DEG_TO_RAD(y), 0.f) ));
  glm::quat QuatAroundZ = glm::normalize(glm::quat( glm::vec3(0.f, 0.f, _DEG_TO_RAD(z)) ));

  angle = glm::normalize(QuatAroundZ * QuatAroundY * QuatAroundX * angle);

  Basura
  glRotatef(angle.x, 1.f, 0.f, 0.f);
  glRotatef(angle.y, 0.f, 1.f, 0.f);
  glRotatef(angle.z, 0.f, 0.f, 1.f);

  glRotatef(x, 1.f, 0.f, 0.f);
  glRotatef(y, 0.f, 1.f, 0.f);
  glRotatef(z, 0.f, 0.f, 1.f);

  GLdouble mat[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, mat);

#define RADIANS M_PI/180

  float C, D, _trx, _try;
  angle.y = D = -asin( mat[2]);
  C           =  cos( angle.y );
  angle.y    *= RADIANS;

  if ( fabs( C ) > 0.005 )
  {
    _trx      =  mat[10] / C;
    _try      = -mat[6]  / C;

    angle.x  = atan2( _try, _trx ) * RADIANS;

    _trx      =  mat[0] / C;
    _try      = -mat[1] / C;

    angle.z  = atan2( _try, _trx ) * RADIANS;
  }
  else
  {
    angle.x  = 0;

    _trx      = mat[5];
    _try      = mat[4];

    angle.z  = atan2( _try, _trx ) * RADIANS;
  }

  NormalizeAngles();*/
}

void CComponent_Transform::Translate(vector3f v)
{
  Translate(v.x, v.y, v.z);
}

void CComponent_Transform::Translate(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled) return;

  position.x += x;
  position.y += y;
  position.z += z;

  /*int num = gameObject->GetNumChildren();
  for(int i = 0; i < num && num != 0; i++)
    gameObject->GetChild(i)->GetComponent<CComponent_Transform>()->Translate(x, y, z);*/

}

void CComponent_Transform::SetPosition(vector3f v)
{
  SetPosition(v.x, v.y, v.z);
}

void CComponent_Transform::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled) return;

   /*vector3f input(x, y, z);

   input.x -= position.x;
   input.y -= position.y;
   input.z -= position.z;

   int num = gameObject->GetNumChildren();
   for(int i = 0; i < num && num != 0; i++)
     gameObject->GetChild(i)->GetComponent<CComponent_Transform>()->Translate(input);*/

   position.x = x;
   position.y = y;
   position.z = z;
}

void CComponent_Transform::Rotate(vector3f v)
{
  Rotate(v.x, v.y, v.z);
}

void CComponent_Transform::Rotate(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled) return;

  NormalizeAngles(x, y, z);

  glm::vec3 EulerAngles(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  angle = glm::quat(EulerAngles) * angle;

  // Equivalente a

  /*glm::quat QuatAroundX = glm::quat( glm::vec3(_DEG_TO_RAD(x), 0.f, 0.f) );
  glm::quat QuatAroundY = glm::quat( glm::vec3(0.f, _DEG_TO_RAD(y), 0.f) );
  glm::quat QuatAroundZ = glm::quat( glm::vec3(0.f, 0.f, _DEG_TO_RAD(z)) );

  angle = glm::normalize(QuatAroundZ * QuatAroundY * QuatAroundX * angle);*/

  /*  BASURA!glm::quat QuatAroundX = glm::quat( glm::vec3(_DEG_TO_RAD(x), 0.f, 0.f) );
  glm::quat QuatAroundY = glm::quat( glm::vec3(0.f, _DEG_TO_RAD(y), 0.f) );
  glm::quat QuatAroundZ = glm::quat( glm::vec3(0.f, 0.f, _DEG_TO_RAD(z)) );

  angle = glm::normalize(QuatAroundZ * QuatAroundY * QuatAroundX * angle);

  glm::quat QuatAroundX = glm::quat( 1.f, 0.f, 0.f, _DEG_TO_RAD(x) );
  glm::quat QuatAroundY = glm::quat( 0.f, 1.f, 0.f, _DEG_TO_RAD(y) );
  glm::quat QuatAroundZ = glm::quat( 0.f, 0.f, 1.f, _DEG_TO_RAD(z) );
  angle = angle * QuatAroundZ * QuatAroundY * QuatAroundX;

  glm::vec3 EulerAngles(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  glm::quat rotacion = glm::quat(EulerAngles);

  glm::vec3 ea(EulerAngles().x + x, EulerAngles().y + y, EulerAngles().z + z);
  cout << "EULER: " << EulerAngles().x << " " << EulerAngles().y << " " << EulerAngles().z << " " << endl;
  GLfloat rx = EulerAngles().x + x;
  GLfloat ry = EulerAngles().y + y;
  GLfloat rz = EulerAngles().z + z;

  NormalizeAngles(rx, ry, rz);
  SetAngle(rx, ry, rz);
  SetAngle(EulerAngles().x + x, EulerAngles().y + y, EulerAngles().z + z);

  angle = glm::quat(ea);

  NormalizeAngles(x, y, z);

  glm::quat QuatAroundX = glm::quat( 1.0, 0.f, 0.f, _DEG_TO_RAD(x) );
  glm::quat QuatAroundZ = glm::quat( 0.f, 0.f, 1.f, _DEG_TO_RAD(z) );

  angle = QuatAroundX * QuatAroundY * QuatAroundZ * angle;


  glm::vec3 ea(_DEG_TO_RAD(EulerAngles().x + x), _DEG_TO_RAD(EulerAngles().y + y), _DEG_TO_RAD( EulerAngles().z + z));
  angle = glm::quat(ea);

  glm::vec3 ea(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  angle = glm::quat(ea) * angle;

  glm::vec3 ea(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  glm::quat rotacion = glm::quat(ea);
  angle = rotacion * angle;

  Quaternion finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;
  angle = rotacion * angle;
   Comprobar que no se pasen de 360
  angle.x += x;
  angle.y += y;
  angle.z += z;

  int num = gameObject->GetNumChildren();
  for(int i = 0; i < num && num != 0; i++)
    gameObject->GetChild(i)->GetComponent<CComponent_Transform>()->Rotate(x, y, z);

  NormalizeAngles();
  cout << "Current angles: " << angle << endl;*/
}

void CComponent_Transform::SetAngle(vector3f v)
{
  SetAngle(v.x, v.y, v.z);
}

void CComponent_Transform::SetAngle(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled) return;

  NormalizeAngles(x, y, z);

  glm::vec3 EulerAngles(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  angle = glm::quat(EulerAngles);

  /* Basura!!
  glm::quat QuatAroundX = glm::quat( 1.0, 0.f, 0.f, _DEG_TO_RAD(x) );
  glm::quat QuatAroundY = glm::quat( 0.f, 1.f, 0.f, _DEG_TO_RAD(y) );
  glm::quat QuatAroundZ = glm::quat( 0.f, 0.f, 1.f, _DEG_TO_RAD(z) );
  angle = QuatAroundX * QuatAroundY * QuatAroundZ;
  glm::vec3 EulerAngles(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  angle = glm::quat(EulerAngles);
   Pseudocode
  Quaternion QuatAroundX = Quaternion( Vector3(1.0,0.0,0.0), EulerAngle.x );
  Quaternion QuatAroundY = Quaternion( Vector3(0.0,1.0,0.0), EulerAngle.y );
  Quaternion QuatAroundZ = Quaternion( Vector3(0.0,0.0,1.0), EulerAngle.z );
  Quaternion finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;

  angle.x = x;
  angle.y = y;
  angle.z = z;

  NormalizeAngles();
  vector3f input(x, y, z);

  input.x -= angle.x;
  input.y -= angle.y;
  input.z -= angle.z;

  int num = gameObject->GetNumChildren();
  for(int i = 0; i < num && num != 0; i++)
    gameObject->GetChild(i)->GetComponent<CComponent_Transform>()->Rotate(input);

  angle.x = x;
  angle.y = y;
  angle.z = z;

  NormalizeAngles();*/
}

void CComponent_Transform::Scale(vector3f v)
{
  Scale(v.x, v.y, v.z);
}

void CComponent_Transform::Scale(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled) return;

  // Multiplicar en vez de sumar
  scale.x *= x;
  scale.y *= y;
  scale.z *= z;

  /*
  if(scale.x < 0) scale.x = -scale.x;
  if(scale.y < 0) scale.y = -scale.y;
  if(scale.z < 0) scale.z = -scale.z;*/

  /*int num = gameObject->GetNumChildren();
  for(int i = 0; i < num && num != 0; i++)
    gameObject->GetChild(i)->GetComponent<CComponent_Transform>()->Scale(x, y, z);*/
}

void CComponent_Transform::SetScale(vector3f v)
{
  SetScale(v.x, v.y, v.z);
}

void CComponent_Transform::SetScale(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled) return;

  /*vector3f input(x, y, z);

  input.x -= scale.x;
  input.y -= scale.y;
  input.z -= scale.z;

  int num = gameObject->GetNumChildren();
  for(int i = 0; i < num && num != 0; i++)
    gameObject->GetChild(i)->GetComponent<CComponent_Transform>()->Scale(input);*/

  scale.x = x;
  scale.y = y;
  scale.z = z;
}

void CComponent_Transform::ApplyTransform()
{
  //   if(!enabled) return;
  //glLoadIdentity();
  if(gameObject->GetParent())
    ApplyParentTransform(gameObject->GetParent());

  /* BASURA!
  glRotatef(angle.y, 0.f, 1.f, 0.f);
  glRotatef(angle.z, 0.f, 0.f, 1.f);
  glRotatef(angle.x, 1.f, 0.f, 0.f);

  glRotatef(angle.z, 0.f, 0.f, 1.f);
  glRotatef(angle.y, 0.f, 1.f, 0.f);
  glRotatef(angle.x, 1.f, 0.f, 0.f);

  glRotatef(angle.y, 0.f, 1.f, 0.f);
  glRotatef(angle.x, cos(angle.y * M_PI/180), 0, sin(angle.y*M_PI/180)); // <- CASI!
  glRotatef(angle.z, cos(angle.y * M_PI/180), 0.f, sin(angle.y*M_PI/180));
  glRotatef(angle.x, cos(angle.y * M_PI/180), 0, sin(angle.y*M_PI/180));
  glRotatef(angle.z, cos(angle.y * M_PI/180), cos(angle.x * M_PI/180), sin(angle.y*M_PI/180));

  glRotatef(angle.z, 0.f, 0.f, 1.f);
  glRotatef(angle.y, 0.f, 1.f, 0.f);
  glRotatef(angle.x, 1.f, 0.f, 0.f);

  glRotatef(angle.y, 0.f, 1.f, 0.f);
  glRotatef(angle.z, 0.f, 0.f, 1.f);
  glRotatef(angle.x, 1.f, 0.f, 0.f);

  glRotatef(_RAD_TO_DEG(angle.w), angle.x, angle.y, angle.z);*/

  // Posición
  glTranslatef(position.x, position.y, position.z);
  // Orientación
  glMultMatrixf((const float*)glm::value_ptr(glm::toMat4(angle)));                  // <- GLOBAL
  //glMultMatrixf((const float*)glm::value_ptr(glm::inverse(glm::toMat4(angle))));  // <- LOCAL
  // Escala
  glScalef(scale.x, scale.y, scale.z);
}

void CComponent_Transform::ApplyParentTransform(CGameObject* parent)
{
  //   if(!parent->transform()->enabled) return;
  // Se puede usar en una pila sin problemas y sin llamadas recursivas
  // se evita el parámetro "parent"
  if(parent == NULL)
    return;

  ApplyParentTransform(parent->GetParent());

  // Posición
  glTranslatef(parent->transform()->position.x, parent->transform()->position.y, parent->transform()->position.z);
  // Orientación
  glMultMatrixf((const float*)glm::value_ptr(glm::toMat4(parent->transform()->angle)));                  // <- GLOBAL
  //glMultMatrixf((const float*)glm::value_ptr(glm::inverse(glm::toMat4(parent->transform()->angle))));  // <- LOCAL
  // Escala
  glScalef(parent->transform()->scale.x, parent->transform()->scale.y, parent->transform()->scale.z);

  /*std::stack<CGameObject*> parent_stack;
  CGameObject* p = gameObject->GetParent();
  while(p)
  {
    parent_stack.push(p);
    p = p->GetParent();
  }

  while(!parent_stack.empty())
  {
    p = parent_stack.top();
    parent_stack.pop();

    glTranslatef(p->transform()->position.x, p->transform()->position.y, p->transform()->position.z);

    glRotatef(p->transform()->angle.x, 1.f, 0.f, 0.f);
    glRotatef(p->transform()->angle.y, 0.f, 1.f, 0.f);
    glRotatef(p->transform()->angle.z, 0.f, 0.f, 1.f);

    glScalef(p->transform()->scale.x, p->transform()->scale.y, p->transform()->scale.z);
  }*/
}

vector3f_t CComponent_Transform::Position()
{
  if(!gameObject->GetParent())
    return position;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  ApplyParentTransform(gameObject->GetParent());

  // Posición
  glTranslatef(position.x, position.y, position.z);
  // Orientación
  glMultMatrixf((const float*)glm::value_ptr(glm::toMat4(angle)));
  // Escala
  glScalef(scale.x, scale.y, scale.z); // ¿?

  GLdouble matrix[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, matrix);

  glPopMatrix();

  return vector3f(matrix[12], matrix[13], matrix[14]);
}



void CComponent_Transform::LookAt(vector3f target, vector3f up)
{
  // http://gamedev.stackexchange.com/questions/53129/quaternion-look-at-with-up-vector
  vector3f forward_l = (target - Position()).normalize();
  vector3f forward_w(1, 0, 0);
  vector3f axis  = forward_l % forward_w;
  float angle1 = acos(forward_l * forward_w);

  vector3f third = axis % forward_w;
  if (third * forward_l < 0)
  {
    angle = - angle;
  }

  //glm::quat q1 = glm::gtx::axis_angle_to_quaternion(angle, axis);
  glm::quat q1 = glm::angleAxis(angle1, axis.to_glm());

  //vector3f up_l  = mx::transform(q1, up.normalize());
  vector3f up_l  = gMath.transform(q1, up.normalize());
  vector3f right = (forward_l % up).normalize();
  vector3f up_w  = (right % forward_l).normalize();

  vector3f     axis2  = up_l % up_w;
  float        angle2 = acos(up_l * up_w);

  /*mx::Vector3f third2 = axis2 % up_w;
  if (third2 * up_l < 0)
  {
    angle2 = - angle2;
  }*/
  //glm::quat q2 = mx::axis_angle_to_quaternion(angle2, axis2);
  glm::quat q2 = glm::angleAxis(angle2, axis2.to_glm());

  this->angle = (q2 * q1);

  /*vector3f current_pos = Position();
  vector3f direction(x - current_pos.x, y - current_pos.y, z - current_pos.z);
  GLfloat  direction_length = direction.length();

  vector3f diagonal_XZ = vector3f(x - current_pos.x, 0, z - current_pos.z);
  GLfloat diagonal_XZ_length = diagonal_XZ.length();

  cout << "Lenght of direction(" << direction << ") is " << direction_length << endl;
  cout << "Lenght of diagonal_XZ(" << diagonal_XZ << ") is " << diagonal_XZ_length << endl;
  cout << "Alpha = asin(direction.z/diagonal_XZ) = asin(" << direction.z << "/" << diagonal_XZ_length << ") = " << 90 - gMath.asin(direction.z/diagonal_XZ_length) << endl;
  cout << "Beta  = asin(direction.y/direction_length) = asin(" << direction.y << "/" << direction_length << ") = " << gMath.asin(direction.y/direction_length) << endl;
  //float delta_x = x - current_pos.x;
  //float delta_y = y - current_pos.y;

  //float angle = atan2(delta_x, delta_y);

  SetAngle(0.f, 90 - gMath.asin(direction.z/diagonal_XZ_length), 0.f);
  LRotate(0.f, 0.f, gMath.asin(direction.y/direction_length));

  // No funciona ni a tiros
  vector3f pos = Position();
  vector3f direction(x - pos.x, y - pos.y, z - pos.z);

  // Find the rotation between the front of the object (that we assume towards +Z,
  // but this depends on your model) and the desired direction
  CSystem_Math::quat rot1 = gMath.RotationBetweenVectors(gMath.Z_AXIS, direction);

  vector3f desiredUp(gMath.Y_AXIS);
  vector3f right = gMath.cross_product(direction, desiredUp);
  desiredUp = gMath.cross_product(right, direction);

  // Because of the 1rst rotation, the up is probably completely screwed up.
  // Find the rotation between the "up" of the rotated object, and the desired up
  glm::vec3 newUp = rot1 * glm::vec3(0.f, 1.f, 0.f);
  CSystem_Math::quat rot2 = gMath.RotationBetweenVectors(vector3f(newUp), desiredUp);

  //CSystem_Math::quat targetOrientation = rot2 * rot1; // remember, in reverse order.
  angle = glm::normalize(rot2 * rot1);*/
}

/*void CComponent_Transform::ApplyParentTransform()
{
  CGameObject* parent = gameObject->GetParent();

  // ¿de abajo a arriba o de arriba a abajo?
  while(parent != NULL)
  {
    glTranslatef(parent->transform()->position.x, parent->transform()->position.y, parent->transform()->
        position.z);

    glRotatef(parent->transform()->angle.x, 1.f, 0.f, 0.f);
    glRotatef(parent->transform()->angle.y, 0.f, 1.f, 0.f);
    glRotatef(parent->transform()->angle.z, 0.f, 0.f, 1.f);

    glScalef(parent->transform()->scale.x, parent->transform()->scale.y, parent->transform()->scale.z);

    parent = parent->GetParent();
  }
}*/
