#include "components/_component_gui.h"

#include "systems/_resource.h"
#include "systems/_render.h"
#include "systems/_shader.h"

using namespace std;

GLuint CComponent_GUI_Texture::m_GUITextureVBOVertices = 0;
GLuint CComponent_GUI_Texture::m_GUITextureVBOTexCoords = 0;
GLuint CComponent_GUI_Texture::m_GUITextureVAO = 0;

CComponent_GUI_Texture::CComponent_GUI_Texture(CGameObject* gameObject): CComponent(gameObject)
{
  texture_name = "";

  pixel_offset_x = pixel_offset_y = 0;
  width = height = 1.f;

  color(1.f, 1.f, 1.f, 1.f);
}

CComponent_GUI_Texture::~CComponent_GUI_Texture()
{

}

// Problemático: deben reconstruirse estos VBOs cada vez que se cambie el tamaño de la pantalla :L
//   Solución temporal -> Actualizar el VBO de vértices en cada iteración.

bool CComponent_GUI_Texture::InitRenderVBO()
{
  const GLfloat GUITexture_TexCoords[][2]
  {
    {0.f, 1.f}, {1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f}
  };

  glGenVertexArrays(1, &m_GUITextureVAO);
  if(!m_GUITextureVAO)
  {
    gSystem_Debug.error("From CComponent_Transform: Could not generate Transform VAO.");
    return false;
  }

  glGenBuffers( 1, &m_GUITextureVBOVertices );
  glGenBuffers( 1, &m_GUITextureVBOTexCoords );

  if(!m_GUITextureVBOVertices or !m_GUITextureVBOTexCoords)
  {
    gSystem_Debug.error("From CComponent_Transform: Could not generate Transform VBO.");

    glDeleteVertexArrays(1, &m_GUITextureVAO);
    glDeleteBuffers( 1, &m_GUITextureVBOVertices );
    glDeleteBuffers( 1, &m_GUITextureVBOTexCoords );

    return false;
  }

  glBindVertexArray(m_GUITextureVAO);

  glBindBuffer( GL_ARRAY_BUFFER, m_GUITextureVBOTexCoords );
  glBufferData( GL_ARRAY_BUFFER, 6*3*sizeof(GLfloat), GUITexture_TexCoords, GL_DYNAMIC_DRAW );
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer( GL_ARRAY_BUFFER, m_GUITextureVBOVertices );
  glBufferData( GL_ARRAY_BUFFER, 6*3*sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW );
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);

  return true;
}

void CComponent_GUI_Texture::CloseRenderVBO()
{
  glDeleteVertexArrays(1, &m_GUITextureVAO);
  glDeleteBuffers( 1, &m_GUITextureVBOVertices );
  glDeleteBuffers( 1, &m_GUITextureVBOTexCoords );
}

void CComponent_GUI_Texture::UpdateVBO()
{
  int w, h;
  gSystem_Render.GetWindowSize(&w, &h);

  if(w != 0 and h != 0)
  {
    const GLfloat GUITexture_Vertices[][3]
    {
      {-width/2 + (float)pixel_offset_x/w,  height/2 + (float)pixel_offset_y/h, 0.f},
      { width/2 + (float)pixel_offset_x/w,  height/2 + (float)pixel_offset_y/h, 0.f},
      { width/2 + (float)pixel_offset_x/w, -height/2 + (float)pixel_offset_y/h, 0.f},
      {-width/2 + (float)pixel_offset_x/w, -height/2 + (float)pixel_offset_y/h, 0.f}
    };

    glBindVertexArray(m_GUITextureVAO);

    glBindBuffer( GL_ARRAY_BUFFER, m_GUITextureVBOVertices );
    glBufferData( GL_ARRAY_BUFFER, 6*3*sizeof(GLfloat), GUITexture_Vertices, GL_DYNAMIC_DRAW );
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
  }
}

bool CComponent_GUI_Texture::HitTest(GLfloat x, GLfloat y)
{
  int w, h;
  gSystem_Render.GetWindowSize(&w, &h);

  x /= w;
  y /= h;

  if(x >= 0.f + (float)pixel_offset_x/w and x <= width + (float)pixel_offset_x/w and y <= 0.f + (float)pixel_offset_y/h and  y >= -height + (float)pixel_offset_y/h)
    return true;

  return false;
}

void CComponent_GUI_Texture::OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix)
{
  if(!enabled) return;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture(texture_name)->GetID());

  modelViewMatrix = glm::translate(modelViewMatrix, gameObject->Transform()->position.to_glm());
  //modelViewMatrix = modelViewMatrix * gameObject->Transform()->angle; // ¿?
  modelViewMatrix = modelViewMatrix * glm::mat4_cast(gameObject->Transform()->angle);
  modelViewMatrix = glm::scale(modelViewMatrix,  gameObject->Transform()->scale.to_glm());

  CShader* simpleShader = gSystem_Shader_Manager.UseShader("__textureShader");
  glUniformMatrix4fv(simpleShader->GetUniformIndex("ProjMatrix") , 1, GL_FALSE, glm::value_ptr(projMatrix));
  glUniformMatrix4fv(simpleShader->GetUniformIndex("ModelViewMatrix") , 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
  glUniform1f(simpleShader->GetUniformIndex("textureFlag"), 1.0f);
  glUniform4f(simpleShader->GetUniformIndex("in_Color"), color.r, color.g, color.b, color.a);

  UpdateVBO();

  glBindVertexArray(m_GUITextureVAO);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer( GL_ARRAY_BUFFER, m_GUITextureVBOVertices );
  glBindBuffer( GL_ARRAY_BUFFER, m_GUITextureVBOTexCoords );

  glDrawArrays( GL_QUADS, 0, 4);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glBindVertexArray(0);

  /*glTranslatef(position.x, position.y, position.z);
  //glRotatef(rotation_z, 0.f, 0.f, 1.f);
  glMultMatrixf((const float*)glm::value_ptr(glm::toMat4(gameObject->Transform()->angle)));
  glScalef(scale.x, scale.y, scale.z);

  int w, h;
  gSystem_Render.GetWindowSize(&w, &h);

  glBegin(GL_QUADS);
    glTexCoord2d(0,1); glVertex3f(-width/2 + (float)pixel_offset_x/w,  height/2 + (float)pixel_offset_y/h, 0.f);
    glTexCoord2d(1,1); glVertex3f( width/2 + (float)pixel_offset_x/w,  height/2 + (float)pixel_offset_y/h, 0.f);
    glTexCoord2d(1,0); glVertex3f( width/2 + (float)pixel_offset_x/w, -height/2 + (float)pixel_offset_y/h, 0.f);
    glTexCoord2d(0,0); glVertex3f(-width/2 + (float)pixel_offset_x/w, -height/2 + (float)pixel_offset_y/h, 0.f);
  glEnd();*/
}


void CComponent_GUI_Texture::parseDebug(string command)
{
  stringstream ss(command);
  string attrib;

  ss >> attrib;

  /*
   string texture_name;

   GLuint pixel_offset_x, pixel_offset_y;
   GLfloat width, height;

   colorf_t color;
   */

  if(attrib == "help" or attrib == "?" or attrib == "")
  {
    printDebug();

    return;
  }

  if(attrib == "texture_name")
  {
    string data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg(
          "From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"",
          gameObject->GetName().c_str(),
          Components::component_to_string((Components::components_t) GetID()));

      return;
    }

    texture_name = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%s\".",
        gameObject->GetName().c_str(),
        Components::component_to_string((Components::components_t) GetID()), attrib.c_str(),
        data.c_str());
  }
  else if(attrib == "pixel_offset_x" or attrib == "pixel_offset_y")
  {
    unsigned int data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg(
          "From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"",
          gameObject->GetName().c_str(),
          Components::component_to_string((Components::components_t) GetID()));

      return;
    }

    if(attrib == "pixel_offset_x")
      pixel_offset_x = data;
    else if(attrib == "pixel_offset_y")
      pixel_offset_y = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%d\".",
        gameObject->GetName().c_str(),
        Components::component_to_string((Components::components_t) GetID()), attrib.c_str(),
        data);
  }
  else if(attrib == "width" or attrib == "height")
  {
    float data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg(
          "From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"",
          gameObject->GetName().c_str(),
          Components::component_to_string((Components::components_t) GetID()));

      return;
    }

    if(attrib == "width")
      width = data;
    else if(attrib == "height")
      height = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%f\".",
        gameObject->GetName().c_str(),
        Components::component_to_string((Components::components_t) GetID()), attrib.c_str(),
        data);
  }
  else if(attrib == "color")
  {
    colorf_t data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg(
          "From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"",
          gameObject->GetName().c_str(),
          Components::component_to_string((Components::components_t) GetID()));

      return;
    }

    color = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%s\".",
        gameObject->GetName().c_str(),
        Components::component_to_string((Components::components_t) GetID()), attrib.c_str(),
        data.str().c_str());
  }
  else
  {
    gSystem_Debug.console_error_msg("From component %s - %s: Unknow attribute \"%s\".",
        gameObject->GetName().c_str(),
        Components::component_to_string((Components::components_t) GetID()), attrib.c_str());
  }
}

void CComponent_GUI_Texture::printDebug()
{
  gSystem_Debug.console_warning_msg("Component %s uses the following attributes:",
      Components::component_to_string((Components::components_t) GetID()));
  gSystem_Debug.console_warning_msg("Attribute      Type                Value");
  gSystem_Debug.console_warning_msg("----------------------------------------");
  gSystem_Debug.console_warning_msg("texture_name        string         %s", texture_name.c_str());
  gSystem_Debug.console_warning_msg("pixel_offset_x      unsigned int   %d", pixel_offset_x);
  gSystem_Debug.console_warning_msg("pixel_offset_y      unsigned int   %d", pixel_offset_y);
  gSystem_Debug.console_warning_msg("width               float          %f", width);
  gSystem_Debug.console_warning_msg("height              float          %f", height);
  gSystem_Debug.console_warning_msg("color               colorf_t       %s", color.str().c_str());
}
