#include "_object.h"

#include "components/_component_camera.h"
#include "components/_component_transform.h"

#include "systems/_render.h"
#include "systems/_debug.h"
#include "systems/_data.h"
#include "systems/_manager.h"
#include "systems/_resource.h"
#include "systems/_shader.h"

#include "engine/_engine.h"

CSystem_Render gSystem_Render;
CSystem_Render& gRender = gSystem_Render;

bool CSystem_Render::Init()
{
  if(enabled) return true;

  GLInfo.resize(6);

  //multitexture_supported = vbos_supported = false;

  int sampling_usability, sampling_max_value;
  GetMaxSamples(sampling_usability, sampling_max_value);

  // Set MaxSamplers
  stringstream ss;
  ss << sampling_max_value;
  GLInfo[4] = ss.str();

  int user_sampling_usability = gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_MULTISAMPLING");
  int user_sampling_max_value = gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_MULTISAMPLING_VALUE");

  // Crear una ventana "dummy" para ver los valores de multisampling permitidos.
  if(user_sampling_usability <= 0 or user_sampling_max_value <= 0 )
  {
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
  }
  else if(user_sampling_usability > 0)
  {
    if(sampling_usability)
    {
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
      if(user_sampling_max_value > sampling_max_value)
      {
        gSystem_Debug.log("WARNING!! From Render: Trying to set multisampling samples to %d, being the max value %d. Using %d.", user_sampling_max_value, sampling_max_value, sampling_max_value);
        gSystem_Debug.console_warning_msg("From Render: Trying to set multisampling samples to %d, being the max value %d. Using %d.", user_sampling_max_value, sampling_max_value, sampling_max_value);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, sampling_max_value);
      }
      else if(!gSystem_Math.IsPowerOfTwo(user_sampling_max_value))
      {
        gSystem_Debug.log("WARNING!! From Render: Trying to set multisampling samples to %d, which is non power of 2. Using %d.", user_sampling_max_value, sampling_max_value);
        gSystem_Debug.console_warning_msg("From Render: Trying to set multisampling samples to %d, which is non power of 2. Using %d.", user_sampling_max_value, sampling_max_value);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, sampling_max_value);
      }
      else
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, user_sampling_max_value);
    }
    else
    {
      gSystem_Debug.log("WARNING!! From Render: Trying to enable multisampling (antialising), which is not supported.");
      gSystem_Debug.console_warning_msg("From Render: Trying to enable multisampling (antialising), which is not supported.");

      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
    }
  }

  window = SDL_CreateWindow( gEngine.GetTitle().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH"), gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_HEIGHT"), SDL_WINDOW_OPENGL);
  if(!window)
  {
    gSystem_Debug.error("From CSystem_Render: Could not create window: %s", SDL_GetError());
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not create window. Check log.txt");
    return false;
  }

  string videomode = gSystem_Data_Storage.GetString("__RENDER_RESOLUTION_WINDOW_MODE");
  videomode = GO_Utils::string_to_lower(videomode);

  if (videomode == "fullscreen")
    gSystem_Render.SetFullScreenWindow(Render::fullscreen);
  else if (videomode == "fullwindowed")
    gSystem_Render.SetFullScreenWindow(Render::fullwindowed);
  else
    gSystem_Render.SetFullScreenWindow(Render::windowed);

  GLcontext = SDL_GL_CreateContext(window);

  glEnable(GL_MULTISAMPLE);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0, 0, 0, 1);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);

  glEnable( GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Avoid to some transparent polygons write the z-depth
  //glAlphaFunc ( GL_GREATER, 0.15 ) ;
  //glEnable ( GL_ALPHA_TEST ) ;

  glClearColor(0, 0, 0, 1.f);
  glClearDepth(1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  //glDepthFunc(GL_LESS);

  glEnable(GL_SCISSOR_TEST);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );

  //glEnable(GL_PERSPECTIVE_CORRECTION);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);


  glEnable(GL_COLOR_MATERIAL);

  //glEnable(GL_BLEND);
  //glBlendFunc(GL_ONE, GL_ONE);

  GLenum error = glGetError();
  if(error != GL_NO_ERROR)
  {
    gSystem_Debug.error("From CSystem_Render: Could not init OpenGL: %s", gluErrorString(error));
    return false;
  }

  GLenum err = glewInit();
  if(err != GLEW_OK)
  {
    gSystem_Debug.error("From CSystem_Render: Could not init GLEW: %s", glewGetErrorString(err));
    return false;
  }

  // ->PorHacer Hay que tener en cuenta los detalles de la compatibilidad de opengl (y extensiones de GLEW).
  if (!GLEW_ARB_instanced_arrays)
  {
    gSystem_Debug.error("-> %s", glewGetErrorString(err));
    return false;
  }

  /*if(!glewIsSupported("GL_multitexture"))
  {
    gSystem_Debug.error("From CSystem_Render: GLEW error: GL_multitexture NOT supported!");
    return false;
  }

  if(!glewIsSupported("GL_vertex_buffer_object"))
  {
    gSystem_Debug.error("From CSystem_Render: GLEW error: GL_vertex_buffer_object NOT supported!");
    return false;
  }*/

  if(!InitSkyboxVBO() or !InitGridVBO()) return false;

  current_camera = -1;

  GUI_Camera = gSystem_GameObject_Manager.AddGameObject("__RENDER_GUI_CAMERA");
  GUI_Camera->Camera()->clear = false;
  GUI_Camera->Camera()->viewmode = viewmode::ortho;
  GUI_Camera->Camera()->ApplyChanges();
  GUI_Camera->Preserve();

  /*if(!glewIsSupported("GL_EXT_texture_env_combine"))
  {
    gSystem_Debug.error("From CSystem_Render: GLEW error: GL_EXT_texture_env_combine NOT supported!");
    return false;
  }*/

  quadratic = gluNewQuadric();

  // Other renders
  if(!CComponent_Transform::InitRenderVBO() or !CComponent_Particle_Emitter::InitRenderVBO() or !CComponent_GUI_Texture::InitRenderVBO()) return false;

  CSystem::Init();

  SetGLInfo();

  return true;
}

void CSystem_Render::SetGLInfo()
{
  string aux = (char*)glGetString(GL_VENDOR);
  GLInfo[0] = aux;

  aux = (char*)glGetString(GL_RENDERER);
  GLInfo[1] = aux;

  aux = (char*)glGetString(GL_VERSION);
  GLInfo[2] = aux;

  aux = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
  GLInfo[3] = aux;

  GLint num_ext = 0;
  glGetIntegerv(GL_NUM_EXTENSIONS, &num_ext);
  aux = "";
  for(int i = 0; i < num_ext and num_ext != 0; i++)
  {
    char* extension = (char*)glGetStringi(GL_EXTENSIONS, i);
    if(extension)
    {
      aux += extension;
      if(i != num_ext - 1)
        aux += " ";
    }
  }
  GLInfo[5] = aux;
}

bool CSystem_Render::InitSkyboxVBO()
{
  float r = 1.005f;

  uint m_nSkyboxVertexCount = 24;
  const GLfloat m_pVertices [][3] =
  {
    /* Top    */ { r,  1.0f, -r}, {-r,  1.0f, -r}, {-r,  1.0f,  r}, { r,  1.0f,  r},
    /* Bottom */ { r, -1.0f,  r}, {-r, -1.0f,  r}, {-r, -1.0f, -r}, { r, -1.0f, -r},
    /* Left   */ { 1.0f,  r, -r}, { 1.0f,  r,  r}, { 1.0f, -r,  r}, { 1.0f, -r, -r},
    /* Right  */ {-1.0f,  r,  r}, {-1.0f,  r, -r}, {-1.0f, -r, -r}, {-1.0f, -r,  r},
    /* Front  */ { r,  r,  1.0f}, {-r,  r,  1.0f}, {-r, -r,  1.0f}, { r, -r,  1.0f},
    /* Back   */ {-r,  r, -1.0f}, { r,  r, -1.0f}, { r, -r, -1.0f}, {-r, -r, -1.0f}
  };

  const GLfloat m_pTexCoords [][2] =
  {
    /* Top    */ { 512/2048.f, 1536/1536.f}, {1024/2048.f, 1536/1536.f}, {1024/2048.f, 1024/1536.f}, { 512/2048.f, 1024/1536.f},
    /* Bottom */ { 512/2048.f,  512/1536.f}, {1024/2048.f,  512/1536.f}, {1024/2048.f,    0/1536.f}, { 512/2048.f,    0/1536.f},
    /* Left   */ {   0/2048.f, 1024/1536.f}, { 512/2048.f, 1024/1536.f}, { 512/2048.f,  512/1536.f}, {   0/2048.f,  512/1536.f},
    /* Right  */ {1024/2048.f, 1024/1536.f}, {1536/2048.f, 1024/1536.f}, {1536/2048.f,  512/1536.f}, {1024/2048.f,  512/1536.f},
    /* Front  */ { 512/2048.f, 1024/1536.f}, {1024/2048.f, 1024/1536.f}, {1024/2048.f,  512/1536.f}, { 512/2048.f,  512/1536.f},
    /* Back   */ {1536/2048.f, 1024/1536.f}, {2048/2048.f, 1024/1536.f}, {2048/2048.f,  512/1536.f}, {1536/2048.f,  512/1536.f}
  };

  glGenVertexArrays(1, &m_SkyboxVAO);
  if(!m_SkyboxVAO)
  {
    gSystem_Debug.error("From Render: Could not generate Skybox VAO.");
    return false;
  }
  //glBindVertexArray(m_SkyboxVAO);

  glGenBuffers( 1, &m_SkyboxVBOVertices );
  glGenBuffers( 1, &m_SkyboxVBOTexCoords );
  if(!m_SkyboxVBOVertices or !m_SkyboxVBOTexCoords)
  {
    gSystem_Debug.error("From Render: Could not generate Skybox VBO.");
    return false;
  }

  glBindVertexArray(m_SkyboxVAO);

  glBindBuffer( GL_ARRAY_BUFFER, m_SkyboxVBOVertices );
  glBufferData( GL_ARRAY_BUFFER, m_nSkyboxVertexCount*3*sizeof(GLfloat), m_pVertices, GL_STATIC_DRAW );
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer( GL_ARRAY_BUFFER, m_SkyboxVBOTexCoords );
  glBufferData( GL_ARRAY_BUFFER, m_nSkyboxVertexCount*2*sizeof(GLfloat), m_pTexCoords, GL_STATIC_DRAW );
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  return true;
}

bool CSystem_Render::InitGridVBO()
{
  glGenVertexArrays(1, &m_GridVAO);
  if(!m_GridVAO)
  {
    gSystem_Debug.error("From Render: Could not generate Grid VAO.");
    return false;
  }
  //glBindVertexArray(m_GridVAO);

  m_GridVBOVertices = m_GridVBOColors = m_GridVAO = 0;

  glGenBuffers( 1, &m_GridVBOVertices );
  glGenBuffers( 1, &m_GridVBOColors );
  if(!m_GridVBOVertices or !m_GridVBOColors)
  {
    gSystem_Debug.error("From Render: Could not generate Grid VBO.");
    return false;
  }

  m_GridVBO_numcols = gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM_GRID_COLS");
  m_GridVBO_numrows = gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM_GRID_ROWS");

  UpdateGridVBO(m_GridVBO_numcols, m_GridVBO_numrows);

  return true;
}

void CSystem_Render::UpdateGridVBO(int ncols, int nrows)
{
  m_GridVBO_numcols = ncols;
  m_GridVBO_numrows = nrows;

  if( m_GridVBO_numcols > 0 and m_GridVBO_numrows > 0)
  {
    int nLines = (m_GridVBO_numcols*5 + 1) + (m_GridVBO_numrows*5 + 1);
    GLfloat pVertices[nLines*2][3];
    GLfloat pColors[nLines*2][3];

    uint index = 0;

      // White vertical lines
    for(uint i = 0; i < m_GridVBO_numcols*5 + 1; i++, index += 2)
    {
      pVertices[index][0] = i*0.2; pVertices[index][1] = 0; pVertices[index][2] = 0; //= {index, 0, 0};
      pVertices[index+1][0] = i*0.2; pVertices[index+1][1] = 0; pVertices[index+1][2] = m_GridVBO_numrows; //pVertindexces.push_back({index, 0, (float)m_GrindexdVBO_numcols});

      if(i%5 == 0)
      {
        pColors[index][0] = pColors[index][1] = pColors[index][2] = 1.f;
        pColors[index+1][0] = pColors[index+1][1] = pColors[index+1][2] = 1.f;

      }
      else
      {
        pColors[index][0] = pColors[index][1] = pColors[index][2] = 0.5f;
        pColors[index+1][0] = pColors[index+1][1] = pColors[index+1][2] = 0.5f;
      }
    }
      // White horizontal lines
    for(uint i = 0; i < m_GridVBO_numrows*5 + 1; i++, index += 2)
    {
      pVertices[index][0] = 0; pVertices[index][1] = 0; pVertices[index][2] = i*0.2; //= {index, 0, 0};
      pVertices[index+1][0] = m_GridVBO_numcols; pVertices[index+1][1] = 0; pVertices[index+1][2] = i*0.2; //pVertindexces.push_back({index, 0, (float)m_GrindexdVBO_numcols});

      if(i%5 == 0)
      {
        pColors[index][0] = pColors[index][1] = pColors[index][2] = 1.f;
        pColors[index+1][0] = pColors[index+1][1] = pColors[index+1][2] = 1.f;
      }
      else
      {
        pColors[index][0] = pColors[index][1] = pColors[index][2] = 0.5f;
        pColors[index+1][0] = pColors[index+1][1] = pColors[index+1][2] = 0.5f;
      }
    }

    glBindVertexArray(m_GridVAO);

    glBindBuffer( GL_ARRAY_BUFFER, m_GridVBOVertices );
    glBufferData( GL_ARRAY_BUFFER, nLines*2*3*sizeof(GLfloat), pVertices, GL_DYNAMIC_DRAW );
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer( GL_ARRAY_BUFFER, m_GridVBOColors );
    glBufferData( GL_ARRAY_BUFFER, nLines*2*3*sizeof(GLfloat), pColors, GL_DYNAMIC_DRAW );
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  }
}

void CSystem_Render::Close()
{
  if(!enabled) return;
  CSystem::Close();

  //Destroy VBOs
  glDeleteBuffers(1, &m_SkyboxVBOTexCoords);
  glDeleteBuffers(1, &m_SkyboxVBOVertices);
  glDeleteVertexArrays(1, &m_SkyboxVAO);

  glDeleteBuffers(1, &m_GridVBOVertices);
  glDeleteBuffers(1, &m_GridVBOColors);
  glDeleteVertexArrays(1, &m_GridVAO);

  // Other renders
  CComponent_Transform::CloseRenderVBO();
  CComponent_Particle_Emitter::CloseRenderVBO();
  CComponent_GUI_Texture::CloseRenderVBO();

  CSystem::Close();

  gluDeleteQuadric(quadratic);

  SDL_GL_DeleteContext(GLcontext);
  SDL_DestroyWindow(window);

  camera_list.clear();
}

// En principio, solo hay que liberar las cámaras.
// ->PorHacer estaría bien añadir una opción para que CSystem_Render::Reset() reiniciase la ventana, no solo las cámaras.
bool CSystem_Render::Reset() {
  for(vector<CGameObject*>::iterator it = camera_list.begin(); it != camera_list.end(); ) {
    if(!(*it)->IsPreserved())
      camera_list.erase(it);
    else
      ++it;
  }

  return true;
}

void CSystem_Render::SetMainCamera(CGameObject* camera)
{
  if(!camera)
    return;

  // Control de repeticiones de objetos
  vector<CGameObject*>::iterator it = camera_list.begin();
  camera_list.insert(it, camera);
  camera->Camera()->ApplyChanges();
}

void CSystem_Render::SetMainCamera(const string& cam)
{
  CGameObject* camera = gSystem_GameObject_Manager[cam];
  if(!camera)
    return;

  // ¿?
  vector<CGameObject*>::iterator it = camera_list.begin();
  camera_list.insert(it, camera);
  camera->Camera()->ApplyChanges();
}

void CSystem_Render::UnSetMainCamera(const string& camera)
{
  vector<CGameObject*>::iterator it = camera_list.begin();
  camera_list.erase(it);
}

void CSystem_Render::AddCamera(CGameObject* camera)
{
  if(camera)
  {
    camera_list.push_back(camera);
    camera->Camera()->ApplyChanges();
  }
}

void CSystem_Render::AddCamera(const string& name)
{
  CGameObject* camera = gSystem_GameObject_Manager[name];
  if(camera)
  {
    camera_list.push_back(camera);
    camera->Camera()->ApplyChanges();
  }
}

void CSystem_Render::AddCameraPrior(CGameObject* camera)
{
  if(camera_list.size() >= 1 && camera)
  {
    vector<CGameObject*>::iterator it = camera_list.begin();
    ++it;
    camera_list.insert(it, camera);
    camera->Camera()->ApplyChanges();
  }
}

void CSystem_Render::AddCameraPrior(const string& name)
{
  CGameObject* camera = gSystem_GameObject_Manager[name];
  if(camera_list.size() >= 1 && camera)
  {
    vector<CGameObject*>::iterator it = camera_list.begin();
    ++it;
    camera_list.insert(it, camera);
    camera->Camera()->ApplyChanges();
  }
}

void CSystem_Render::RemoveCamera(const string& camera)
{
  // ouh shit
  for(vector<CGameObject*>::iterator it = camera_list.begin(); it < camera_list.end(); ++it)
  {
    if( (*it)->GetName() == camera )
      camera_list.erase(it);
  }
}

void CSystem_Render::OnLoop()
{
  /*GLenum error = glGetError();
  if(error != GL_NO_ERROR)
  {
    //gSystem_Debug.msg_box(debug::error, ERROR_RENDER, "From CSystem_Render: OpenGL error: %s", gluErrorString(error) );
    gSystem_Debug.console_error_msg("From CSystem_Render: OpenGL error %d: %s", error, gluErrorString(error));
  }*/
  __GL_CHECK_ERRORS();
}

void CSystem_Render::OnRender()
{
  Clear();

  current_camera = 0;
  // Usar un vector para guardar los objetos GUI que se vayan encontrando en la primera iteración.
  // vector<CGameObject*> gui_texts;
  vector<CComponent_GUI_Texture*> gui_textures;

  for(vector<CGameObject*>::iterator it = camera_list.begin(); it < camera_list.end(); ++it)
  {
    // Disabled camera
    if(!(*it)->IsEnabled())
      continue;

    CComponent_Camera* cam = (*it)->GetComponent<CComponent_Camera>();

    cam->BeforeRender();
    cam->SetViewport();
    cam->SetUp();
    cam->Clear();

    //glm::mat4& projMatrix = cam->projMatrix;


    // Draw Skybox
    RenderSkybox(cam);
    if(gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM_GRID") )
      RenderGrid(cam);

    /*glMatrixMode(GL_PROJECTION);
    glLoadMatrixf( glm::value_ptr(cam->projMatrix) );

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(cam->modelViewMatrix));*/

	  for(map<string, CGameObject*>::iterator it2 = gSystem_GameObject_Manager.gameObjects.begin(); it2 != gSystem_GameObject_Manager.gameObjects.end(); it2++)
	  {
      //glColor3f(1.f, 1.f, 1.f);
      glBindTexture(GL_TEXTURE_2D, 0);

      glm::mat4 local_modelViewMatrix = it2->second->Transform()->ApplyTransform(cam->modelViewMatrix);
	    it2->second->OnRender(cam->projMatrix, local_modelViewMatrix);

	    //CComponent_GUI_Font* gui_font = it2->second->GetComponent<CComponent_GUI_Font>();
	    CComponent_GUI_Texture* gui_texture = it2->second->GetComponent<CComponent_GUI_Texture>();

	    if(current_camera == 0 && gui_texture)
	      gui_textures.push_back(gui_texture);
      //if(current_camera == 0 && gui_font)
        //gui_textures.push_back(gui_font);
	  }

	  // Other renders
    if(gSystem_Data_Storage.GetInt("__RENDER_SOUND_RADIUS"))
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glBindTexture(GL_TEXTURE_2D, 0);

      CShader* simpleShader = gSystem_Shader_Manager.UseShader("__simpleGLUShader");
      glUniformMatrix4fv(simpleShader->GetUniformIndex("ProjMatrix") , 1, GL_FALSE, glm::value_ptr(cam->projMatrix));
      glUniform4f(simpleShader->GetUniformIndex("in_Color"), 1.0, 1.0, 0.0, 1.0);

      for(map<string, CGameObject*>::iterator it2 = gSystem_GameObject_Manager.gameObjects.begin(); it2 != gSystem_GameObject_Manager.gameObjects.end(); it2++)
      {
        if(it2->second->GetComponent<CComponent_Audio_Source>())
        {
          glm::mat4 local_modelViewMatrix = it2->second->Transform()->ApplyTransform(cam->modelViewMatrix);
          glUniformMatrix4fv(simpleShader->GetUniformIndex("ModelViewMatrix") , 1, GL_FALSE, glm::value_ptr(local_modelViewMatrix));
          it2->second->AudioSource()->OnRender(cam->projMatrix, cam->modelViewMatrix);
        }
      }

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

	  if(gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM"))
	  {
	    glClear(GL_DEPTH_BUFFER_BIT);
      glBindTexture(GL_TEXTURE_2D, 0);

      CShader* simpleShader = gSystem_Shader_Manager.UseShader("__flatShader");
      glUniformMatrix4fv(simpleShader->GetUniformIndex("ProjMatrix") , 1, GL_FALSE, glm::value_ptr(cam->projMatrix));

      glBindVertexArray(CComponent_Transform::m_TransformVAO);
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);

      glBindBuffer( GL_ARRAY_BUFFER, CComponent_Transform::m_TransformVBOVertices );
      glBindBuffer( GL_ARRAY_BUFFER, CComponent_Transform::m_TransformVBOColors );

	    for(map<string, CGameObject*>::iterator it2 = gSystem_GameObject_Manager.gameObjects.begin(); it2 != gSystem_GameObject_Manager.gameObjects.end(); it2++)
	    {
        glm::mat4 local_modelViewMatrix = it2->second->Transform()->ApplyTransform(cam->modelViewMatrix);
	      glUniformMatrix4fv(simpleShader->GetUniformIndex("ModelViewMatrix") , 1, GL_FALSE, glm::value_ptr(local_modelViewMatrix));

	      it2->second->Transform()->OnRender(local_modelViewMatrix, cam->projMatrix);
	    }

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);
      glBindVertexArray(0);
	  }

    cam->AfterRender();
    current_camera++;
  }
  current_camera = -1;

  // Render GUI
  glClear(GL_DEPTH_BUFFER_BIT);
  //glDisable(GL_DEPTH_TEST);

  GUI_Camera->Camera()->SetViewport();
  GUI_Camera->Camera()->SetUp();

  for(vector<CComponent_GUI_Texture*>::iterator it = gui_textures.begin(); it != gui_textures.end(); ++it)
  {
    if((*it)->gameObject->enabled)
      (*it)->OnRender(GUI_Camera->Camera()->modelViewMatrix, GUI_Camera->Camera()->projMatrix);
  }

  //glEnable(GL_DEPTH_TEST);
}

// http://www.opengl.org/wiki/Tutorial1:_Rendering_shapes_with_glDrawRangeElements,_VAO,_VBO,_shaders_(C%2B%2B_/_freeGLUT)
// http://www.opengl.org/wiki/Tutorial2:_VAOs,_VBOs,_Vertex_and_Fragment_Shaders_(C_/_SDL)
// !! http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/attributes.php

void CSystem_Render::RenderGrid(CComponent_Camera* cam)
{
  GLfloat cols_scale = gSystem_Data_Storage.GetFloat("__RENDER_TRANSFORM_GRID_COLS_SCALE");
  GLfloat rows_scale = gSystem_Data_Storage.GetFloat("__RENDER_TRANSFORM_GRID_ROWS_SCALE");

  GLint ncols = gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM_GRID_COLS");
  GLint nrows = gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM_GRID_ROWS");

  if((GLuint)ncols != m_GridVBO_numcols or (GLuint)nrows != m_GridVBO_numrows)
    UpdateGridVBO(ncols, nrows);

  glm::mat4 local_modelViewMatrix = cam->modelViewMatrix;
  local_modelViewMatrix = glm::translate(local_modelViewMatrix, glm::vec3((-ncols*cols_scale)/2.f, 0.f, (-nrows*rows_scale)/2.f));
  local_modelViewMatrix = glm::scale(local_modelViewMatrix, glm::vec3(rows_scale, 0.f, cols_scale));

  CShader* simpleShader = gSystem_Shader_Manager.UseShader("__flatShader");

  glUniformMatrix4fv(simpleShader->GetUniformIndex("ProjMatrix") , 1, GL_FALSE, glm::value_ptr(cam->projMatrix));
  glUniformMatrix4fv(simpleShader->GetUniformIndex("ModelViewMatrix") , 1, GL_FALSE, glm::value_ptr(local_modelViewMatrix));

  glBindTexture(GL_TEXTURE_2D, 0);

  glBindVertexArray(m_GridVAO);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer( GL_ARRAY_BUFFER, m_GridVBOVertices );
  glBindBuffer( GL_ARRAY_BUFFER, m_GridVBOColors );

  int nLines = (m_GridVBO_numcols*5 + 1) + (m_GridVBO_numrows*5 + 1);
  glDrawArrays( GL_LINES, 0, nLines*2);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glBindVertexArray(0);
  //glUseProgram(0);
}


bool CSystem_Render::RenderSkybox(CComponent_Camera* cam)
{
  if(!cam or cam->skybox_texture == "")
    return false;

  // Idea sencilla: dibujamos un cubo de tamaño 1x1 sin depth_test justo donde esta la cámara.
  // Todo lo demás se dibujará encima, por lo que creará un efecto de "tamaño inmenso".
  // http://content.gpwiki.org/index.php/Sky_Box

  glColor3f(1.f, 1.f, 1.f);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture(cam->skybox_texture )->GetID());

  vector3f position = cam->gameObject->Transform()->Position();
  glm::mat4 local_modelViewMatrix = cam->modelViewMatrix;//glTranslatef(position.x, position.y, position.z);
  local_modelViewMatrix = glm::translate(local_modelViewMatrix, position.to_glm());

  uint m_nSkyboxVertexCount = 24;

  CShader* simpleShader = gSystem_Shader_Manager.UseShader("__textureShader");

  glUniformMatrix4fv(simpleShader->GetUniformIndex("ProjMatrix") , 1, GL_FALSE, glm::value_ptr(cam->projMatrix));
  glUniformMatrix4fv(simpleShader->GetUniformIndex("ModelViewMatrix") , 1, GL_FALSE, glm::value_ptr(local_modelViewMatrix));
  glUniform1i(simpleShader->GetUniformIndex("texture"), 0);
  glUniform1f(simpleShader->GetUniformIndex("textureFlag"), 1.0f);
  glUniform4f(simpleShader->GetUniformIndex("in_Color"), 1.0, 1.0, 1.0, 1.0);

  glBindVertexArray(m_SkyboxVAO);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer( GL_ARRAY_BUFFER, m_SkyboxVBOVertices );
  glBindBuffer( GL_ARRAY_BUFFER, m_SkyboxVBOTexCoords );

  glDrawArrays( GL_QUADS, 0, m_nSkyboxVertexCount);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glClear(GL_DEPTH_BUFFER_BIT);

  //glUseProgram(0);

  return true;
}

void CSystem_Render::Clear()
{
  glScissor(0, 0, gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH"), gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_HEIGHT"));

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



