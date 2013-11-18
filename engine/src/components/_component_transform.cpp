#include "components/_component_transform.h"
#include "_systems.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Transform);

CComponent_Transform::CComponent_Transform(CGameObject* gameObject): CComponent(gameObject)
{
  //id = ccomponents::transform;

  position.x = position.y = position.z = 0;
  //angle.x = angle.y = angle.z = 0;
  scale.x = scale.y = scale.z = 1.f;

  glm::vec3 EulerAngles(0, 0, 0);
  angle = glm::quat(EulerAngles);
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
  if(gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM"))
  {
    GLboolean depth_state;
    glGetBooleanv(GL_DEPTH_TEST, &depth_state);

    if(depth_state) glDisable(GL_DEPTH_TEST);
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

    if(depth_state) glEnable(GL_DEPTH_TEST);
  }
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
  // Mover basandose en la rotación (cosa extraña)
  // Translate X
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  /*glRotatef(angle.y, 0.f, 1.f, 0.f);
  glRotatef(angle.x, 1.f, 0.f, 0.f);
  glRotatef(angle.z, 0.f, 0.f, 1.f);*/
  glRotatef(_RAD_TO_DEG(angle.w), angle.x, angle.y, angle.z);

  glTranslatef(x, y, z);

  GLdouble matrix[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, matrix);

  position.x += matrix[12];
  position.y += matrix[13];
  position.z += matrix[14];

  /*cout << "Matrix: " << endl;
  for(uint i = 0; i < 4; i++)
  {
    for(uint j = 0; j < 4; j++)
      cout << setw(10) << setprecision(3) << matrix[4*j + i] << " ";

    cout << endl;
  }*/

  glPopMatrix();
}

void CComponent_Transform::LRotate(GLfloat x, GLfloat y, GLfloat z)
{
  // Mover basandose en la rotación (cosa extraña)
  // Translate X
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

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
  angle.y = D = -asin( mat[2]);        /* Calculate Y-axis angle */
  C           =  cos( angle.y );
  angle.y    *= RADIANS;

  if ( fabs( C ) > 0.005 )              /* Gimball lock? */
  {
    _trx      =  mat[10] / C;           /* No, so get X-axis angle */
    _try      = -mat[6]  / C;

    angle.x  = atan2( _try, _trx ) * RADIANS;

    _trx      =  mat[0] / C;            /* Get Z-axis angle */
    _try      = -mat[1] / C;

    angle.z  = atan2( _try, _trx ) * RADIANS;
  }
  else                                  /* Gimball lock has occurred */
  {
    angle.x  = 0;                       /* Set X-axis angle to zero */

    _trx      = mat[5];                 /* And calculate Z-axis angle */
    _try      = mat[4];

    angle.z  = atan2( _try, _trx ) * RADIANS;
  }

  NormalizeAngles();

  /*cout << "Matrix: " << endl;
  for(uint i = 0; i < 4; i++)
  {
    for(uint j = 0; j < 4; j++)
      cout << setw(10) << setprecision(3) << mat[4*j + i] << " ";

    cout << endl;
  }*/

  glPopMatrix();
}

void CComponent_Transform::Translate(vector3f v)
{
  Translate(v.x, v.y, v.z);
}

void CComponent_Transform::Translate(GLfloat x, GLfloat y, GLfloat z)
{
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
//  glm::vec3 EulerAngles(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
//  glm::quat rotacion = glm::quat(EulerAngles);

  //glm::vec3 ea(EulerAngles().x + x, EulerAngles().y + y, EulerAngles().z + z);
  //cout << "EULER: " << EulerAngles().x << " " << EulerAngles().y << " " << EulerAngles().z << " " << endl;
  /*GLfloat rx = EulerAngles().x + x;
  GLfloat ry = EulerAngles().y + y;
  GLfloat rz = EulerAngles().z + z;

  NormalizeAngles(rx, ry, rz);
  SetAngle(rx, ry, rz);*/
  //SetAngle(EulerAngles().x + x, EulerAngles().y + y, EulerAngles().z + z);

  //angle = glm::quat(ea);

  /*glm::quat QuatAroundX = glm::quat( 1.0, 0.f, 0.f, _DEG_TO_RAD(x) );
  glm::quat QuatAroundY = glm::quat( 0.f, 1.f, 0.f, _DEG_TO_RAD(y) );
  glm::quat QuatAroundZ = glm::quat( 0.f, 0.f, 1.f, _DEG_TO_RAD(z) );

  angle = QuatAroundX * QuatAroundY * QuatAroundZ * angle;*/

  //glm::vec3 ea(_DEG_TO_RAD(EulerAngles().x + x), _DEG_TO_RAD(EulerAngles().y + y), _DEG_TO_RAD( EulerAngles().z + z));
  //angle = glm::quat(ea);

  //glm::vec3 ea(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  //angle = glm::quat(ea) * angle;

  glm::vec3 ea(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  glm::quat rotacion = glm::quat(ea);
  angle = rotacion * angle;

  //Quaternion finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;
  //angle = rotacion * angle;
  // Comprobar que no se pasen de 360
  /*angle.x += x;
  angle.y += y;
  angle.z += z;*/

  /*int num = gameObject->GetNumChildren();
  for(int i = 0; i < num && num != 0; i++)
    gameObject->GetChild(i)->GetComponent<CComponent_Transform>()->Rotate(x, y, z);*/

  //NormalizeAngles();
  //cout << "Current angles: " << angle << endl;
}

void CComponent_Transform::SetAngle(vector3f v)
{
  SetAngle(v.x, v.y, v.z);
}

void CComponent_Transform::SetAngle(GLfloat x, GLfloat y, GLfloat z)
{
  /*glm::quat QuatAroundX = glm::quat( 1.0, 0.f, 0.f, _DEG_TO_RAD(x) );
  glm::quat QuatAroundY = glm::quat( 0.f, 1.f, 0.f, _DEG_TO_RAD(y) );
  glm::quat QuatAroundZ = glm::quat( 0.f, 0.f, 1.f, _DEG_TO_RAD(z) );
  angle = QuatAroundX * QuatAroundY * QuatAroundZ;*/

  NormalizeAngles(x, y, z);

  glm::vec3 EulerAngles(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  angle = glm::quat(EulerAngles);


  //glm::vec3 EulerAngles(_DEG_TO_RAD(x), _DEG_TO_RAD(y), _DEG_TO_RAD(z));
  //angle = glm::quat(EulerAngles);
  /* Pseudocode
  Quaternion QuatAroundX = Quaternion( Vector3(1.0,0.0,0.0), EulerAngle.x );
  Quaternion QuatAroundY = Quaternion( Vector3(0.0,1.0,0.0), EulerAngle.y );
  Quaternion QuatAroundZ = Quaternion( Vector3(0.0,0.0,1.0), EulerAngle.z );
  Quaternion finalOrientation = QuatAroundX * QuatAroundY * QuatAroundZ;*/

  /*angle.x = x;
  angle.y = y;
  angle.z = z;

  NormalizeAngles();*/
  /*vector3f input(x, y, z);

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
  glLoadIdentity();
  if(gameObject->GetParent())
    ApplyParentTransform(gameObject->GetParent());


  glTranslatef(position.x, position.y, position.z);

//  glRotatef(angle.y, 0.f, 1.f, 0.f);
//  glRotatef(angle.z, 0.f, 0.f, 1.f);
//  glRotatef(angle.x, 1.f, 0.f, 0.f);

//  glRotatef(angle.z, 0.f, 0.f, 1.f);
//  glRotatef(angle.y, 0.f, 1.f, 0.f);
//  glRotatef(angle.x, 1.f, 0.f, 0.f);

  //glRotatef(angle.y, 0.f, 1.f, 0.f);
  //glRotatef(angle.x, cos(angle.y * M_PI/180), 0, sin(angle.y*M_PI/180)); // <- CASI!
  //glRotatef(angle.z, cos(angle.y * M_PI/180), 0.f, sin(angle.y*M_PI/180));
//  glRotatef(angle.x, cos(angle.y * M_PI/180), 0, sin(angle.y*M_PI/180));
//  glRotatef(angle.z, cos(angle.y * M_PI/180), cos(angle.x * M_PI/180), sin(angle.y*M_PI/180));

//  glRotatef(angle.z, 0.f, 0.f, 1.f);
//  glRotatef(angle.y, 0.f, 1.f, 0.f);
//  glRotatef(angle.x, 1.f, 0.f, 0.f);

  /*glRotatef(angle.y, 0.f, 1.f, 0.f);
  glRotatef(angle.z, 0.f, 0.f, 1.f);
  glRotatef(angle.x, 1.f, 0.f, 0.f);*/

  glRotatef(_RAD_TO_DEG(angle.w), angle.x, angle.y, angle.z);

  glScalef(scale.x, scale.y, scale.z);
}

void CComponent_Transform::ApplyParentTransform(CGameObject* parent)
{
  // Se puede usar en una pila sin problemas y sin llamadas recursivas
  // se evita el parámetro "parent"
  if(parent == NULL)
    return;

  ApplyParentTransform(parent->GetParent());

  glTranslatef(parent->transform()->position.x, parent->transform()->position.y, parent->transform()->position.z);

  //glRotatef(_RAD_TO_DEG(parent->transform()->angle.w), parent->transform()->angle.x, parent->transform()->angle.y, parent->transform()->angle.z);

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
