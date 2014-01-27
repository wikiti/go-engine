#include "components/_component_transform.h"
#include "systems/_other.h"

#include "_object.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Transform);

GLuint CComponent_Transform::vertex_transformVBO = 0;
GLuint CComponent_Transform::colors_transformVBO = 0;

void CComponent_Transform::InitRenderVBO()
{
  GLfloat transform_vertex[][3]
  {
    {0.f, 0.f, 0.f}, {0.5f, 0.0f, 0.0f},
    {0.f, 0.f, 0.f}, {0.0f, 0.5f, 0.0f},
    {0.f, 0.f, 0.f}, {0.0f, 0.0f, 0.5f}
  };

  GLfloat colors_vertex[][3]
  {
    {1.f, 0.f, 0.f}, {1.0f, 0.0f, 0.0f},
    {0.f, 1.f, 0.f}, {0.0f, 1.0f, 0.0f},
    {0.f, 0.f, 1.f}, {0.0f, 0.0f, 1.0f}
  };

  glGenBuffers( 1, &vertex_transformVBO );
  glBindBuffer( GL_ARRAY_BUFFER_ARB, vertex_transformVBO );
  glBufferData( GL_ARRAY_BUFFER_ARB, 6*3*sizeof(GLfloat), transform_vertex, GL_STATIC_DRAW_ARB );

  glGenBuffers( 1, &colors_transformVBO );
  glBindBuffer( GL_ARRAY_BUFFER_ARB, colors_transformVBO );
  glBufferData( GL_ARRAY_BUFFER_ARB, 6*3*sizeof(GLfloat), colors_vertex, GL_STATIC_DRAW_ARB );
}

void CComponent_Transform::CloseRenderVBO()
{
  glDeleteBuffers(1, &vertex_transformVBO);
  glDeleteBuffers(1, &colors_transformVBO);
}


CComponent_Transform::CComponent_Transform(CGameObject* gameObject): CComponent(gameObject)
{
  position.x = position.y = position.z = 0;
  scale.x = scale.y = scale.z = 1.f;
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

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glBindBuffer( GL_ARRAY_BUFFER_ARB, vertex_transformVBO );
  glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );
  glBindBuffer( GL_ARRAY_BUFFER_ARB, colors_transformVBO );
  glColorPointer( 3, GL_FLOAT, 0, (char *) NULL );

  glDrawArrays( GL_LINES, 0, 6 );

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  /*glEnableClientState(GL_VERTEX_ARRAY);

  GLfloat axis_x_vertex[][3]{ {0.f, 0.f, 0.f}, {0.5f, 0.0f, 0.0f} };
  GLfloat axis_y_vertex[][3]{ {0.f, 0.f, 0.f}, {0.0f, 0.5f, 0.0f} };
  GLfloat axis_z_vertex[][3]{ {0.f, 0.f, 0.f}, {0.0f, 0.0f, 0.5f} };

  // X - Rojo
  glVertexPointer(2, GL_FLOAT, 0, &axis_x_vertex[0]);
  glColor3f(1.f, 0.f, 0.f); glDrawArrays(GL_LINES, 0, 1);

  // Y - Verde
  glVertexPointer(2, GL_FLOAT, 0, &axis_y_vertex[0]);
  glColor3f(0.f, 1.f, 0.f); glDrawArrays(GL_LINES, 0, 1);

  // Z - Azul
  glVertexPointer(3, GL_FLOAT, 0, &axis_z_vertex[0]);
  glColor3f(0.f, 0.f, 1.f); glDrawArrays(GL_LINES, 0, 1);

  glDisableClientState(GL_VERTEX_ARRAY);*/


  /*glBegin(GL_LINES);
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
  glEnd();*/
}

vector3f CComponent_Transform::EulerAngles()
{
  /*glm::vec3 ea = glm::eulerAngles(angle);
  // Pasar a grados
  return vector3f(_RAD_TO_DEG(ea.x), _RAD_TO_DEG(ea.y), _RAD_TO_DEG(ea.z));*/
  /*glm::vec3 ea = glm::eulerAngles(angle);

  GLfloat nx = _RAD_TO_DEG(ea.x);
  GLfloat ny = _RAD_TO_DEG(ea.y);
  GLfloat nz = _RAD_TO_DEG(ea.z);

  gMath.NormalizeAngles(nx, ny, nz);
  return vector3f(nx, ny, nz);*/
  float x = _RAD_TO_DEG(atan2(2*(angle.y*angle.z + angle.w*angle.x), angle.w*angle.w - angle.x*angle.x - angle.y*angle.y + angle.z*angle.z));
  float y = _RAD_TO_DEG(asin(-2*(angle.x*angle.z - angle.w*angle.y)));
  float z = _RAD_TO_DEG(atan2(2*(angle.x*angle.y + angle.w*angle.z), angle.w*angle.w + angle.x*angle.x - angle.y*angle.y - angle.z*angle.z));

  gMath.NormalizeAngles(x, y, z);
  return vector3f(x, y, z);
  // Necesario?
}

void CComponent_Transform::LTranslate(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled or (x == 0 and y == 0 and z == 0)) return;

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
  if(!enabled or (x == 0 and y == 0 and z == 0)) return;

  // Como Rotate(), sólo que primero se aplica la rotación, y luego la orientación (mientras que en Rotate se aplica primero la orientación y luego la rotación).
  gMath.NormalizeAngles(x, y, z);

  glm::vec3 EulerAngles(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  angle = angle * glm::quat(EulerAngles);
}

void CComponent_Transform::Translate(vector3f v)
{
  Translate(v.x, v.y, v.z);
}

void CComponent_Transform::Translate(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled or (x == 0 and y == 0 and z == 0)) return;

  position.x += x;
  position.y += y;
  position.z += z;

}

void CComponent_Transform::SetPosition(vector3f v)
{
  SetPosition(v.x, v.y, v.z);
}

void CComponent_Transform::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled) return;

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
  if(!enabled or (x == 0 and y == 0 and z == 0)) return;

  gMath.NormalizeAngles(x, y, z);

  glm::vec3 EulerAngles(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  angle = glm::quat(EulerAngles) * angle;
}

void CComponent_Transform::SetAngle(vector3f v)
{
  SetAngle(v.x, v.y, v.z);
}

void CComponent_Transform::SetAngle(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled) return;

  gMath.NormalizeAngles(x, y, z);

  glm::vec3 EulerAngles(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  angle = glm::quat(EulerAngles);

}

void CComponent_Transform::Scale(vector3f v)
{
  Scale(v.x, v.y, v.z);
}

void CComponent_Transform::Scale(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled or (x == 1 and y == 1 and z == 1)) return;

  // Multiplicar en vez de sumar
  scale.x *= x;
  scale.y *= y;
  scale.z *= z;
}

void CComponent_Transform::SetScale(vector3f v)
{
  SetScale(v.x, v.y, v.z);
}

void CComponent_Transform::SetScale(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled) return;

  scale.x = x;
  scale.y = y;
  scale.z = z;
}

void CComponent_Transform::ApplyTransform()
{
  if(gameObject->GetParent())
    ApplyParentTransform(gameObject->GetParent());

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
  glTranslatef(parent->Transform()->position.x, parent->Transform()->position.y, parent->Transform()->position.z);
  // Orientación
  glMultMatrixf((const float*)glm::value_ptr(glm::toMat4(parent->Transform()->angle)));                  // <- GLOBAL
  // Escala
  glScalef(parent->Transform()->scale.x, parent->Transform()->scale.y, parent->Transform()->scale.z);

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

void CComponent_Transform::LookAt(GLfloat x, GLfloat y, GLfloat z, vector3f up_vector, vector3f forward_vector)
{
  LookAt(vector3f(x, y, z), up_vector, forward_vector);
}

void CComponent_Transform::LookAt(vector3f target, vector3f up, vector3f forward)
{
  //http://stackoverflow.com/questions/12435671/quaternion-lookat-function
  vector3f forwardVector = (target - Position()).normalize();

  float dot = forward * forwardVector;

  if (gMath.abs(dot - (-1.0f)) < 0.000001f)
  {
    angle = glm::angleAxis(3.1415926535897932f, up.to_glm());
    return;
  }
  if (gMath.abs(dot - (1.0f)) < 0.000001f)
  {
    return;
  }

  float rotAngle = (float)acos(dot);
  vector3f rotAxis = forward % forwardVector;
  gMath.normalize(rotAxis);

  angle = (glm::angleAxis(rotAngle, rotAxis.to_glm()));

  // http://gamedev.stackexchange.com/questions/53129/quaternion-look-at-with-up-vector
  /*vector3f forward_l = (target - Position()).normalize();
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

//  mx::Vector3f third2 = axis2 % up_w;
//  if (third2 * up_l < 0)
//  {
//    angle2 = - angle2;
//  }
  //glm::quat q2 = mx::axis_angle_to_quaternion(angle2, axis2);
  glm::quat q2 = glm::angleAxis(angle2, axis2.to_glm());

  this->angle = (q2 * q1);*/

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

vector3f CComponent_Transform::up()
{
  glm::vec3 y_axis(0.f, 1.f, 0.f);
  glm::vec3 out = glm::rotate(angle, y_axis);

  return vector3f(out.x, out.y, out.z);
}

vector3f CComponent_Transform::left()
{
  glm::vec3 x_axis(1.f, 0.f, 0.f);
  glm::vec3 out = glm::rotate(angle, x_axis);

  return vector3f(out.x, out.y, out.z);
}

vector3f CComponent_Transform::forward()
{
  glm::vec3 z_axis(0.f, 0.f, 1.f);
  glm::vec3 out = glm::rotate(angle, z_axis);

  return vector3f(out.x, out.y, out.z);
}

GLfloat CComponent_Transform::pitch()
{
  vector3f new_z_axis = forward();
  new_z_axis.x = 0;

  GLfloat output = gMath.acos((new_z_axis.dot_product(gMath.Z_AXIS))/(new_z_axis.length()));
  if(new_z_axis.y < 0)
    output *= -1;

  gMath.NormalizeAngle(output);
  return output;
}

GLfloat CComponent_Transform::yaw()
{
  vector3f new_x_axis = left();
  new_x_axis.y = 0;

  GLfloat output = gMath.acos((new_x_axis.dot_product(gMath.X_AXIS))/(new_x_axis.length()));
  if(new_x_axis.z < 0)
    output *= -1;

  gMath.NormalizeAngle(output);
  return output;
}

GLfloat CComponent_Transform::roll()
{
  vector3f new_y_axis = forward();
  new_y_axis.z = 0;

  GLfloat output = gMath.acos((new_y_axis.dot_product(gMath.Y_AXIS))/(new_y_axis.length()));
  if(new_y_axis.x < 0)
    output *= -1;

  gMath.NormalizeAngle(output);
  return output;
}

void rotate_by_parent(glm::quat& angle, CGameObject* self)
{
  if(self == NULL)
    return;
  else
  {
    rotate_by_parent(angle, self->GetParent());
    angle = self->Transform()->angle * angle;
  }
}

vector3f CComponent_Transform::Rotation()
{
  glm::quat langle(angle);
  rotate_by_parent(langle, gameObject);

//  vector3f new_y_axis = forward();
//  new_y_axis.z = 0;
//  GLfloat roll = gMath.acos((new_y_axis.dot_product(gMath.Y_AXIS))/(new_y_axis.length()));
//  if(new_y_axis.x < 0)
//    roll *= -1;
//  gMath.NormalizeAngle(roll);

  // ¿?¿?


  //return langle;
}

vector3f CComponent_Transform::LRotation()
{
  return vector3f(pitch(), yaw(), roll());
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
