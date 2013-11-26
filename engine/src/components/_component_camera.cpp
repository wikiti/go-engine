#include "systems/_data.h"
#include "components/_component_camera.h"
#include "components/_component_transform.h"

#include "_object.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Camera);

using namespace viewmode;

CComponent_Camera::CComponent_Camera(CGameObject* gameObject): CComponent(gameObject),
 viewmode(perspective), field_of_view(45.f), near_clip(0.1f), far_clip(200.f), clear(true), target(NULL)
{
  disable_gui = false;

  viewport.x = viewport.y = 0;
  //gSystem_Render.GetWindowSize(&viewport.width, &viewport.height);
  viewport.width = viewport.height = 1.f;

  background_color.r = background_color.g = background_color.b = 0;
  background_color.a = 1.f;

  before_render = after_render = NULL;

  skybox_texture = "";

  // Invisible de cara al sistema
  pivot = new CGameObject("__camera_pivot");
  pivot->Init();
  pivot->transform()->position.z = 1;

  gameObject->AddChild(pivot);
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
  gameObject->RemoveChild("__camera_pivot");
  delete pivot;
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
  //if(!enabled) return;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if(viewmode == viewmode::ortho)
  {
    glOrtho(0.f, 1.f, 0.f, 1.f, -1.f, 0.f);
  }
  else if(viewmode == viewmode::perspective)
  {
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

  //glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
  //glScissor(viewport.x, viewport.y, viewport.width, viewport.height);

  int w = gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH");
  int h = gSystem_Data_Storage.GetInt("__RESOLUTION_HEIGHT");

  glViewport(viewport.x*w, viewport.y*h, viewport.width*w, viewport.height*h);
  glScissor(viewport.x*w, viewport.y*h, viewport.width*w, viewport.height*h);
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
  //if(!enabled) return; // ¿?

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixd(projMatrix);

  vector3f p = gameObject->transform()->Position();       // Camera position
  vector3f up(0, 1, 0);                                   // Up vector (for "screen rotation")
  vector3f tp(0, 0, 1);                                   // Target point

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Solución temporal: usar un pivote anclado al objeto que manipule la cámara como un hijo.
  // Mientras la cámara rote, se aplicarán las rotaciones al pivote, lo que permitirá a la cámara mirar libremente.
  //  -> La solución es un poco burda, pero sencilla. Al menos, no complicamos las cosas con procesos matemáticos innecesarios.
  //
  // Nota: Recalcular la posición global del pivote o del objetivo en cada iteración, para cada cámara,
  //   puede resultar pesado si la cámara está anidada a otros objetos. Sería conveniente declarar un
  //   previous_pos y un pos para evitar recalcular si son iguales (ganamos GPU y CPU a cambio de 48 bytes).

  // Por cierto, falta recalcular el vector UP, que dependerá del ángulo de la cámara (Eje local Z).

  if(!target) // Añadir pivote y calcular su posición global:
    tp = pivot->transform()->Position(); // Inservible...
  else        // O coger la posición global del objeto.
    tp = target->transform()->Position();

  gluLookAt(p.x, p.y, p.z,     // Camera position
            tp.x, tp.y, tp.z,     // Target point
            up.x, up.y, up.z);    // Up vector
}
