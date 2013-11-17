#include "_systems.h"
#include "components/_component_camera.h"
#include "components/_component_transform.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Camera);

using namespace viewmode;

CComponent_Camera::CComponent_Camera(CGameObject* gameObject): CComponent(gameObject),
 viewmode(perspective), field_of_view(45.f), near_clip(0.1f), far_clip(200.f), clear(true), target(NULL)
{
  viewport.x = viewport.y = 0;
  //gSystem_Render.GetWindowSize(&viewport.width, &viewport.height);
  viewport.width = viewport.height = 1.f;

  background_color.r = background_color.g = background_color.b = 0;
  background_color.a = 1.f;

  before_render = after_render = NULL;
}

/*CComponent_Camera::CComponent_Camera(CGameObject* gameObject, bool mc, viewmode::viewmode_t vm, viewport_t vp, GLfloat fov, GLfloat nc, GLfloat fc, color_t bc ): CComponent(gameObject),
  main_camera(mc), viewmode(vm), viewport(vp), field_of_view(fov), near_clip(nc), far_clip(fc), target(NULL)
{
  background_color(0.f, 0.f, 0.f, 1.f);
  // LOLWUT
  // SIN USAR!
}*/

CComponent_Camera::~CComponent_Camera()
{

}

void CComponent_Camera::Set(input_t data)
{
  (*this) = *(CComponent_Camera*)data;
}

output_t CComponent_Camera::Get()
{
  return (void*)(this);
}

void CComponent_Camera::ApplyChanges()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if(viewmode == viewmode::ortho)
  {
    glOrtho(0.f, 1.f, 0.f, 1.f, -1.f, 0.f);
  }
  else if(viewmode == viewmode::perspective)
  {
    // ¿Wait wut?
    //gluPerspective(field_of_view, (GLfloat)viewport.width/(GLfloat)viewport.height, near_clip, far_clip);
    gluPerspective(field_of_view,
        (viewport.width*gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH")) / (viewport.height*gSystem_Data_Storage.GetInt("__RESOLUTION_HEIGHT")),
        near_clip, far_clip);
  }

  glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
}

void CComponent_Camera::SetViewport()
{
  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();

  int w = gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH");
  int h = gSystem_Data_Storage.GetInt("__RESOLUTION_HEIGHT");

  glViewport(viewport.x*w, viewport.y*h, viewport.width*w, viewport.height*h);
  glScissor(viewport.x*w, viewport.y*h, viewport.width*w, viewport.height*h);

  //glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
  //glScissor(viewport.x, viewport.y, viewport.width, viewport.height);
}

void CComponent_Camera::Clear()
{
  // ¿Canal alpha?
  if(clear)
  {
    glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}

void CComponent_Camera::SetUp()
{
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixd(projMatrix);

  vector3f* p = &gameObject->transform()->position;   // Camera position
  vector3f* r = &gameObject->transform()->angle;      // Camera angle
  vector3f up(0, 1, 0);                               // Up vector (for "screen rotation")
  vector3f tp(0, 0, 1);                               // Target point

  static bool failure = true;

  if(!target)
  {
    // http://mathworld.wolfram.com/SphericalCoordinates.html
    // Arreglar esto con rotaciones locales!
    tp.z = p->z + cos((r->y)*M_PI/180) * sin((r->x+90)*M_PI/180);
    tp.x = p->x + sin((r->y)*M_PI/180) * sin((r->x+90)*M_PI/180);
    tp.y = p->y + cos((r->x+90)*M_PI/180);
  }
  else
  {
    // Esto no vasta, hay que calcular la posición del objeto por si es hijo, teniendo en cuenta las rotaciones
    // posible restricción: Sólo se puede apuntar a objetos SIN padre.
    if(!target->GetParent())
      tp = target->transform()->position;
    else
    {
      if(failure)
      {
        failure = false;
        gSystem_Debug.console_error_msg("Error from Camera Component of \"%s\" aiming \"%s\":", gameObject->GetName().c_str(), target->GetName().c_str());
        gSystem_Debug.console_error_msg("Cannot aim to another GameObject which have a parent (It must be a root or base GameObject)");
      }
    }

  }

  /*up = tp.normalize().cross_product(tp.normalize().cross_product(vector3f(0, 1, 0)));
  up.x = cos((90 + r->y)*M_PI/180);
  up.y = sin((90 + r->x)*M_PI/180);
  up.z = cos((90 + r->x )*M_PI/180);*/
  // Recalcular vector UP

  gluLookAt(p->x, p->y, p->z,     // Camera position
            tp.x, tp.y, tp.z,     // Target point
            up.x, up.y, up.z);    // Up vector

}
