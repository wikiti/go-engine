#include "systems/_data.h"
#include "systems/_manager.h"
#include "components/_component_camera.h"
#include "components/_component_transform.h"

#include "_object.h"

//BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Camera);

using namespace Viewmode;

const char* Viewmode::viewmode_s[] = {"perspective", "ortho", "ortho_screen"};

const char* Viewmode::viewmode_to_string(viewmode_t c)
{
  if(c < 0 or c >= __viewmode_not_defined)
    c = __viewmode_not_defined;

  return viewmode_s[c];
}

viewmode_t Viewmode::string_to_viewmode(const string& c)
{
  for(uint i = 0; i < Viewmode::__viewmode_not_defined; i++)
    if(c == viewmode_s[i])
      return (viewmode_t)i;

  return __viewmode_not_defined;
}

CComponent_Camera::CComponent_Camera(CGameObject* gameObject): CComponent(gameObject),
 viewmode(perspective), field_of_view(45.f), near_clip(0.1f), far_clip(200.f), clear(true), target("")
{
  disable_gui = false; // ¿?

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
  pivot->Transform()->position.z = 1;

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

void CComponent_Camera::SetTarget(CGameObject* obj)
{
  if(obj) target = obj->GetName();
}


void CComponent_Camera::ApplyChanges()
{
  //if(!enabled) return;

  if(viewmode == Viewmode::ortho)
  {
    //glOrtho(0.f, 1.f, 0.f, 1.f, -1.f, 0.f);
    projMatrix = glm::ortho(0.f, 1.f, 0.f, 1.f, -1.f, 0.f);
  }
  if(viewmode == Viewmode::ortho_screen)
  {
    const float r_w = gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH");
    const float r_h = gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_HEIGHT");

    projMatrix = glm::ortho(0.f, r_w, 0.f, r_h, -1.f, 0.f);
  }
  else if(viewmode == Viewmode::perspective)
  {
    //gluPerspective(field_of_view, (GLfloat)viewport.width/(GLfloat)viewport.height, near_clip, far_clip);
    //gluPerspective(field_of_view,
        //(viewport.width*gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH")) / (viewport.height*gSystem_Data_Storage.GetInt("__RESOLUTION_HEIGHT")),
        //near_clip, far_clip);
    projMatrix = glm::perspective(gSystem_Math.deg_to_rad(field_of_view),
        (viewport.width*gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH")) / (viewport.height*gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_HEIGHT")),
        near_clip, far_clip);
  }

  //glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
}

void CComponent_Camera::SetViewport()
{
  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();

  //glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
  //glScissor(viewport.x, viewport.y, viewport.width, viewport.height);

  int w = gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH");
  int h = gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_HEIGHT");

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

  vector3f p =  gameObject->Transform()->Position();       // Camera position
  vector3f up = gameObject->Transform()->up();             // Up vector (for "screen rotation")
  vector3f tp(0, 0, 1);                                    // Target point

  if(viewmode == Viewmode::ortho or viewmode == Viewmode::ortho_screen) // Añadir rotaciones o algo
    return;

  // Solución temporal: usar un pivote anclado al objeto que manipule la cámara como un hijo.
  // Mientras la cámara rote, se aplicarán las rotaciones al pivote, lo que permitirá a la cámara mirar libremente.
  //  -> La solución es un poco burda, pero sencilla. Al menos, no complicamos las cosas con procesos matemáticos innecesarios.
  //
  // Nota: Recalcular la posición global del pivote o del objetivo en cada iteración, para cada cámara,
  //   puede resultar pesado si la cámara está anidada a otros objetos. Sería conveniente declarar un
  //   previous_pos y un pos para evitar recalcular si son iguales (ganamos GPU y CPU a cambio de 48 bytes).

  CGameObject* target_p = gSystem_GameObject_Manager[target];

  if(!target_p) // Añadir pivote y calcular su posición global:
    tp = pivot->Transform()->Position(); // Inservible...
  else        // O coger la posición global del objeto.
    tp = target_p->Transform()->Position();

  // ->xBUG En CComponent_Camera::Setup() Caso fatal; hay que tomar un valor válido del vector "up"
  //if((p - tp).normalize() == up or (p - tp).normalize() == vector3f())
    //up.x += 0.05f;

  modelViewMatrix = glm::lookAt(p.to_glm(), tp.to_glm(), up.to_glm());
}

void CComponent_Camera::parseDebug(string command)
{
  stringstream ss(command);
  string attrib;
  ss >> attrib;

  if(attrib == "help" or attrib == "?" or attrib == "")
  {
    printDebug();

    return;
  }

  /*
    Viewmode::viewmode_t viewmode;
    viewportf_t viewport;

    GLfloat field_of_view;
    GLfloat near_clip;
    GLfloat far_clip;

    colorf_t background_color;

    string skybox_texture;
    string target;
   */

  if(attrib == "disable_gui" or attrib == "clear")
  {
    bool data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }

    if(attrib == "disable_gui")
      disable_gui = data; // ¿?
    else if(attrib == "clear")
      clear = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%d\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), (int)data );
  }
  else if(attrib == "viewport")
  {
    viewportf_t data;

    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }

    viewport = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%s\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), data.str().c_str() );
  }
  else if(attrib == "viewmode")
  {
    string data_s;
    ss >> data_s;

    viewmode_t data = (viewmode_t)string_to_viewmode(data_s);

    if(ss.fail() or data == __viewmode_not_defined)
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }

    viewmode= data;

    // Necesary
    ApplyChanges();

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%s\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), data_s.c_str() );
  }
  else if(attrib == "field_of_view" or attrib == "near_clip" or attrib == "far_clip")
  {
    float data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }

    if(attrib == "field_of_view")
      field_of_view = data;
    else if(attrib == "near_clip")
      near_clip = data;
    else if(attrib == "far_clip")
      far_clip = data;

    // Necesary
    ApplyChanges();

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%f\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), data );
  }
  else if(attrib == "background_color")
  {
    colorf_t data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }

    background_color = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%s\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), data.str().c_str() );
  }
  else if(attrib == "background_color")
  {
    colorf_t data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }

    background_color = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%s\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), data.str().c_str() );
  }
  else if(attrib == "target" or attrib == "skybox_texture")
  {
    string data;
    ss >> data;

    if(ss.fail() and data != "")
    {
      gSystem_Debug.console_error_msg(
          "From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"",
          gameObject->GetName().c_str(),
          Components::component_to_string((Components::components_t) GetID()));
      return;
    }

    if(attrib == "target")
      target = data;
    else if(attrib == "skybox_texture")
      skybox_texture = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%s\".",
        gameObject->GetName().c_str(),
        Components::component_to_string((Components::components_t) GetID()), attrib.c_str(),
        data.c_str());
  }
  else
  {
    gSystem_Debug.console_error_msg("From component %s - %s: Unknow attribute \"%s\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str() );
  }

}

void CComponent_Camera::printDebug()
{
  gSystem_Debug.console_warning_msg("Component %s uses the following attributes:", Components::component_to_string( (Components::components_t)GetID()));
  gSystem_Debug.console_warning_msg("Attribute             Type          Value");
  gSystem_Debug.console_warning_msg("-----------------------------------------");
  gSystem_Debug.console_warning_msg("disable_gui           bool          %d", (int)disable_gui);
  gSystem_Debug.console_warning_msg("clear                 bool          %d", (int)clear);
  gSystem_Debug.console_warning_msg("viewport              viewportf_t   %s", viewport.str().c_str());
  gSystem_Debug.console_warning_msg("viewmode              viewmode_t    %s", viewmode_to_string(viewmode));
  gSystem_Debug.console_warning_msg("field_of_view         float         %f", field_of_view);
  gSystem_Debug.console_warning_msg("near_clip             float         %f", near_clip);
  gSystem_Debug.console_warning_msg("far_clip              float         %f", far_clip);
  gSystem_Debug.console_warning_msg("background_color      colorf_t      %s", background_color.str().c_str());
  gSystem_Debug.console_warning_msg("skybox_texture        string        %s", skybox_texture.c_str());
  gSystem_Debug.console_warning_msg("target                string        %s", skybox_texture.c_str());
}
