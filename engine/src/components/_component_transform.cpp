#include "components/_component_transform.h"
#include "systems/_other.h"
#include "systems/_shader.h"

#include "_object.h"

//BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Transform);

GLuint CComponent_Transform::m_TransformVBOVertices = 0;
GLuint CComponent_Transform::m_TransformVBOColors = 0;
GLuint CComponent_Transform::m_TransformVAO = 0;

// Test
// https://forum.libcinder.org/topic/extract-position-rotations-and-scale-from-a-matrix44f-and-vice-versa
void decompose(glm::mat4 matrix, glm::vec3& scaling, glm::quat& rotation, glm::vec3& position)
{
  // extract translation
  position.x = matrix[3][0];
  position.y = matrix[3][1];
  position.z = matrix[3][2];

  // extract the rows of the matrix

  glm::vec3 columns[3] =
  {
      glm::vec3(matrix[0].x, matrix[0].y, matrix[0].z),
      glm::vec3(matrix[1][0], matrix[1][1], matrix[1][2]),
      glm::vec3(matrix[2][0], matrix[2][1], matrix[2][2])
  };

  glm::vec3 test(columns[0]);

  // extract the scaling factors
  scaling.x = columns[0].length();
  scaling.y = columns[1].length();
  scaling.z = columns[2].length();

  // and remove all scaling from the matrix
  if(scaling.x)
  {
    columns[0] /= scaling.x;
  }
  if(scaling.y)
  {
    columns[1] /= scaling.y;
  }
  if(scaling.z)
  {
    columns[2] /= scaling.z;
  }

  // build a 3x3 rotation matrix
  glm::mat3 m(columns[0].x, columns[1].x, columns[2].x, columns[0].y, columns[1].y, columns[2].y,
      columns[0].z, columns[1].z, columns[2].z);

  // and generate the rotation quaternion from it
  rotation = glm::quat(m);
}


bool CComponent_Transform::InitRenderVBO()
{
  const GLfloat transform_vertices[][3]
  {
    {0.f, 0.f, 0.f}, {0.5f, 0.0f, 0.0f},
    {0.f, 0.f, 0.f}, {0.0f, 0.5f, 0.0f},
    {0.f, 0.f, 0.f}, {0.0f, 0.0f, 0.5f}
  };

  const GLfloat transform_colors[][3]
  {
    {1.f, 0.f, 0.f}, {1.0f, 0.0f, 0.0f},
    {0.f, 1.f, 0.f}, {0.0f, 1.0f, 0.0f},
    {0.f, 0.f, 1.f}, {0.0f, 0.0f, 1.0f}
  };

  glGenVertexArrays(1, &m_TransformVAO);
  if(!m_TransformVAO)
  {
    gSystem_Debug.error("From CComponent_Transform: Could not generate Transform VAO.");
    return false;
  }

  glGenBuffers( 1, &m_TransformVBOVertices );
  glGenBuffers( 1, &m_TransformVBOColors );

  if(!m_TransformVBOVertices or !m_TransformVBOColors)
  {
    gSystem_Debug.error("From CComponent_Transform: Could not generate Transform VBO.");
    return false;
  }

  glBindVertexArray(m_TransformVAO);

  glBindBuffer( GL_ARRAY_BUFFER, m_TransformVBOVertices );
  glBufferData( GL_ARRAY_BUFFER, 6*3*sizeof(GLfloat), transform_vertices, GL_STATIC_DRAW );
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer( GL_ARRAY_BUFFER, m_TransformVBOColors );
  glBufferData( GL_ARRAY_BUFFER, 6*3*sizeof(GLfloat), transform_colors, GL_STATIC_DRAW );
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);

  return true;
}

void CComponent_Transform::CloseRenderVBO()
{
  glDeleteBuffers(1, &m_TransformVBOVertices);
  glDeleteBuffers(1, &m_TransformVBOColors);

  glDeleteVertexArrays(1, &m_TransformVAO);
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

void CComponent_Transform::OnRender(glm::mat4 modelViewMatrix, glm::mat4 projMatrix)
{
  if(!enabled) return;

  //CShader* simpleShader = gSystem_Shader_Manager.GetShader("__flatShader");
  //glUseProgram(simpleShader->GetProgram());
  /*CShader* simpleShader = gSystem_Shader_Manager.UseShader("__flatShader");

  glUniformMatrix4fv(simpleShader->GetUniformIndex("ProjMatrix") , 1, GL_FALSE, glm::value_ptr(projMatrix));
  glUniformMatrix4fv(simpleShader->GetUniformIndex("ModelViewMatrix") , 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

  glBindTexture(GL_TEXTURE_2D, 0);

  glBindVertexArray(m_TransformVAO);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer( GL_ARRAY_BUFFER, m_TransformVBOVertices );
  glBindBuffer( GL_ARRAY_BUFFER, m_TransformVBOColors );*/

  glDrawArrays( GL_LINES, 0, 6);

  /*glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindVertexArray(0);*/

  //glUseProgram(0);

}

// ->xPORHACER Habría que echarle un vistazo a la función CComponent_Transform::EulerAngles(), ya que no devuelve los valores de rotación "deseados", o mejor dicho, esperados
vector3f CComponent_Transform::EulerAngles()
{
  //return vector3f(pitch(), yaw(), roll());

  /*glm::vec3 ea = glm::eulerAngles(angle);
  // Pasar a grados
  return vector3f(_RAD_TO_DEG(ea.x), _RAD_TO_DEG(ea.y), _RAD_TO_DEG(ea.z));*/
  glm::vec3 ea = glm::eulerAngles(angle);

  GLfloat nx = _RAD_TO_DEG(ea.x);
  GLfloat ny = _RAD_TO_DEG(ea.y);
  GLfloat nz = _RAD_TO_DEG(ea.z);

  gMath.NormalizeAngles(nx, ny, nz);
  return vector3f(nx, ny, nz);
//  float x = _RAD_TO_DEG(atan2(2*(angle.y*angle.z + angle.w*angle.x), angle.w*angle.w - angle.x*angle.x - angle.y*angle.y + angle.z*angle.z));
//  float y = _RAD_TO_DEG(asin(-2*(angle.x*angle.z - angle.w*angle.y)));
//  float z = _RAD_TO_DEG(atan2(2*(angle.x*angle.y + angle.w*angle.z), angle.w*angle.w + angle.x*angle.x - angle.y*angle.y - angle.z*angle.z));

  //gMath.NormalizeAngles(x, y, z);
  //return vector3f(x, y, z);
  // Necesario?
}

void CComponent_Transform::LTranslate(GLfloat x, GLfloat y, GLfloat z)
{
  if(!enabled or (x == 0 and y == 0 and z == 0)) return;

  // Mover basandose en la rotación (cosa extraña)
  // Translate X
  /*glMatrixMode(GL_MODELVIEW);
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

  glPopMatrix();*/
  glm::mat4 transformMatrix = glm::translate( glm::mat4(1.0) * glm::toMat4(angle), glm::vec3(x, y, z));
  position.x += transformMatrix[3][0];
  position.y += transformMatrix[3][1];
  position.z += transformMatrix[3][2];
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
  /* SetAngle(v.x, v.y, v.z) */
  if(!enabled) return;

  v = gSystem_Math.NormalizeAngles(v);
  v = gSystem_Math.deg_to_rad(v);
  angle = glm::quat(v.to_glm());
}

void CComponent_Transform::SetAngle(GLfloat x, GLfloat y, GLfloat z)
{
  SetAngle(vector3f(x, y, z));

  /*if(!enabled) return;

  gMath.NormalizeAngles(x, y, z);

  glm::vec3 EulerAngles(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  angle = glm::quat(EulerAngles);*/

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

glm::mat4 CComponent_Transform::ApplyTransform(const glm::mat4& modelviewMatrix)
{
  //transformMatrix = modelviewMatrix;
  glm::mat4 transformMatrix = modelviewMatrix;

  if(gameObject->GetParent())
    ApplyParentTransform(gameObject->GetParent(), transformMatrix);

  // Posición
  // -> glTranslatef(position.x, position.y, position.z);
  // Orientación
  // -> glMultMatrixf((const float*)glm::value_ptr(glm::toMat4(angle)));                  // <- GLOBAL
  //glMultMatrixf((const float*)glm::value_ptr(glm::inverse(glm::toMat4(angle))));  // <- LOCAL
  // Escala
  // -> glScalef(scale.x, scale.y, scale.z);
  transformMatrix = glm::translate(transformMatrix, position.to_glm());
  transformMatrix = transformMatrix * (glm::toMat4(angle));
  transformMatrix = glm::scale(transformMatrix, scale.to_glm());

  //glLoadMatrixf((const float*)glm::value_ptr(transformMatrix));
  return transformMatrix;
}

void CComponent_Transform::ApplyParentTransform(CGameObject* parent, glm::mat4& transformMatrix)
{
  //   if(!parent->transform()->enabled) return;
  // Se puede usar en una pila sin problemas y sin llamadas recursivas
  // se evita el parámetro "parent"
  if(parent == NULL)
    return ;

  ApplyParentTransform(parent->GetParent(), transformMatrix);

  // Posición
  //glTranslatef(parent->Transform()->position.x, parent->Transform()->position.y, parent->Transform()->position.z);
  // Orientación
  //glMultMatrixf((const float*)glm::value_ptr(glm::toMat4(parent->Transform()->angle)));                  // <- GLOBAL
  // Escala
  //glScalef(parent->Transform()->scale.x, parent->Transform()->scale.y, parent->Transform()->scale.z);

  transformMatrix = glm::translate(transformMatrix, parent->Transform()->position.to_glm());
  transformMatrix = transformMatrix * (glm::toMat4(parent->Transform()->angle));
  transformMatrix = glm::scale(transformMatrix, parent->Transform()->scale.to_glm());

  //glLoadMatrixf((const float*)glm::value_ptr(transformMatrix));

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

  /*glMatrixMode(GL_MODELVIEW);
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
  return vector3f(matrix[12], matrix[13], matrix[14]);*/
  //glm::mat4 matrix = ApplyParentTransform(gameObject->GetParent());
  glm::mat4 matrix = ApplyTransform(glm::mat4(1.0));
  return vector3f(matrix[3][0], matrix[3][1], matrix[3][2]);
}

void CComponent_Transform::LookAt(GLfloat x, GLfloat y, GLfloat z, vector3f up_vector, vector3f forward_vector)
{
  LookAt(vector3f(x, y, z), up_vector, forward_vector);
}

// ->BUG La función CComponent_Transform::LookAt() no funciona como debería, ya que se carga completamente la orientación del vector. Esto es, jode ejes de la cámara.
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
  rotAxis = gMath.normalize(rotAxis);

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
  float pitch = _RAD_TO_DEG(atan2(2*angle.x*angle.w - 2*angle.y*angle.z, 1 - 2*angle.x*angle.x - 2*angle.z*angle.z));
  pitch = gMath.NormalizeAngle(pitch);
  return  pitch;
}

GLfloat CComponent_Transform::yaw()
{
  float yaw = _RAD_TO_DEG(atan2(2*angle.y*angle.w - 2*angle.x*angle.z, 1 - 2*angle.y*angle.y - 2*angle.z*angle.z));
  yaw = gMath.NormalizeAngle(yaw);
  return yaw;
}

GLfloat CComponent_Transform::roll()
{
  float roll = _RAD_TO_DEG(asin(2*angle.x*angle.y + 2*angle.z*angle.w));
  roll = gMath.NormalizeAngle(roll);
  return roll;
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


void CComponent_Transform::parseDebug(string command)
{
  stringstream ss(command);
  string attrib;
  vector3f data;

  ss >> attrib;

  if(attrib == "help" or attrib == "?" or attrib == "")
  {
    printDebug();

    return;
  }

  ss >> data;

  if(ss.fail())
  {
    gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), components::component_to_string( (components::components)GetID()) );
    return;
  }

  if(attrib == "position")
  {
    position = data;
  }
  else if(attrib == "scale")
  {
    scale = data;
  }
  else if(attrib == "angle")
  {
    data = gSystem_Math.NormalizeAngles(data);
    vector3f rads = gSystem_Math.deg_to_rad(data);
    angle = glm::quat(rads.to_glm());
    //gSystem_Math.NormalizeAngles(data);
    //angle = glm::quat();
    //LRotate(data.x, data.y, data.z);
  }
  else
  {
    gSystem_Debug.console_error_msg("From component %s - %s: Unknow attribute \"%s\".", gameObject->GetName().c_str(), components::component_to_string( (components::components)GetID()), attrib.c_str() );
  }

  gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%s\".", gameObject->GetName().c_str(), components::component_to_string( (components::components)GetID()), attrib.c_str(), data.str().c_str() );
}

void CComponent_Transform::printDebug()
{
  gSystem_Debug.console_warning_msg("Component %s uses the following attributes:", components::component_to_string( (components::components)GetID()));
  gSystem_Debug.console_warning_msg("Attribute      Type                Value");
  gSystem_Debug.console_warning_msg("----------------------------------------");
  gSystem_Debug.console_warning_msg("position       vector3f            %s", position.str().c_str());
  gSystem_Debug.console_warning_msg("scale          vector3f            %s", scale.str().c_str());
  gSystem_Debug.console_warning_msg("angle          vector3f(degrees)   %s", LRotation().str().c_str());
}
