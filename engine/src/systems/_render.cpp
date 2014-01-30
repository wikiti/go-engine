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
  CSystem::Init();

  //multitexture_supported = vbos_supported = false;

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
  glEnable(GL_MULTISAMPLE);

  window = SDL_CreateWindow( gEngine.GetTitle().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH"), gSystem_Data_Storage.GetInt("__RESOLUTION_HEIGHT"), SDL_WINDOW_OPENGL);
  if(!window)
  {
    gSystem_Debug.error("From CSystem_Render: Could not create window: %s", SDL_GetError());
    gSystem_Debug.msg_box(ERROR_FATAL_INIT, "Could not create window. Check log.txt");
    return false;
  }

  GLcontext = SDL_GL_CreateContext(window);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0, 0, 0, 1);
  glEnable(GL_TEXTURE_2D);
  glShadeModel(GL_SMOOTH);

  glEnable( GL_BLEND );
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(0, 0, 0, 1.f);
  glClearDepth(1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glEnable(GL_SCISSOR_TEST);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);

  glEnable(GL_COLOR_MATERIAL);

  //glEnable(GL_BLEND);
  //glBlendFunc(GL_ONE, GL_ONE);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

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

  InitSkyboxVBO();
  InitGridVBO();

  current_camera = -1;

  GUI_Camera = gSystem_GameObject_Manager.AddGameObject("__RENDER_GUI_CAMERA");
  GUI_Camera->Camera()->clear = false;
  GUI_Camera->Camera()->viewmode = viewmode::ortho;
  GUI_Camera->Camera()->ApplyChanges();

  /*if(!glewIsSupported("GL_EXT_texture_env_combine"))
  {
    gSystem_Debug.error("From CSystem_Render: GLEW error: GL_EXT_texture_env_combine NOT supported!");
    return false;
  }*/

  quadratic = gluNewQuadric();

  // Other renders
  CComponent_Transform::InitRenderVBO();

  return true;
}

void CSystem_Render::InitSkyboxVBO()
{
  float r = 1.005f;

  uint m_nSkyboxVertexCount = 24;
  const GLfloat m_pVertices [][3] =
  { /* Top    */ { r,  1.0f, -r}, {-r,  1.0f, -r}, {-r,  1.0f,  r}, { r,  1.0f,  r},
    /* Bottom */ { r, -1.0f,  r}, {-r, -1.0f,  r}, {-r, -1.0f, -r}, { r, -1.0f, -r},
    /* Left   */ { 1.0f,  r, -r}, { 1.0f,  r,  r}, { 1.0f, -r,  r}, { 1.0f, -r, -r},
    /* Right  */ {-1.0f,  r,  r}, {-1.0f,  r, -r}, {-1.0f, -r, -r}, {-1.0f, -r,  r},
    /* Front  */ { r,  r,  1.0f}, {-r,  r,  1.0f}, {-r, -r,  1.0f}, { r, -r,  1.0f},
    /* Back   */ {-r,  r, -1.0f}, { r,  r, -1.0f}, { r, -r, -1.0f}, {-r, -r, -1.0f}
  };

  const GLfloat m_pTexCoords [][2] =
  { /* Top    */ { 512/2048.f, 1536/1536.f}, {1024/2048.f, 1536/1536.f}, {1024/2048.f, 1024/1536.f}, { 512/2048.f, 1024/1536.f},
    /* Bottom */ { 512/2048.f,  512/1536.f}, {1024/2048.f,  512/1536.f}, {1024/2048.f,    0/1536.f}, { 512/2048.f,    0/1536.f},
    /* Left   */ {   0/2048.f, 1024/1536.f}, { 512/2048.f, 1024/1536.f}, { 512/2048.f,  512/1536.f}, {   0/2048.f,  512/1536.f},
    /* Right  */ {1024/2048.f, 1024/1536.f}, {1536/2048.f, 1024/1536.f}, {1536/2048.f,  512/1536.f}, {1024/2048.f,  512/1536.f},
    /* Front  */ { 512/2048.f, 1024/1536.f}, {1024/2048.f, 1024/1536.f}, {1024/2048.f,  512/1536.f}, { 512/2048.f,  512/1536.f},
    /* Back   */ {1536/2048.f, 1024/1536.f}, {2048/2048.f, 1024/1536.f}, {2048/2048.f,  512/1536.f}, {1536/2048.f,  512/1536.f}
  };

  glGenBuffers( 1, &m_SkyboxVBOVertices );
  glBindBuffer( GL_ARRAY_BUFFER, m_SkyboxVBOVertices );
  glBufferData( GL_ARRAY_BUFFER, m_nSkyboxVertexCount*3*sizeof(GLfloat), m_pVertices, GL_STATIC_DRAW );

  glGenBuffers( 1, &m_SkyboxVBOTexCoords );
  glBindBuffer( GL_ARRAY_BUFFER, m_SkyboxVBOTexCoords );
  glBufferData( GL_ARRAY_BUFFER, m_nSkyboxVertexCount*2*sizeof(GLfloat), m_pTexCoords, GL_STATIC_DRAW );
}

void CSystem_Render::InitGridVBO()
{
  m_GridVBO_numcols = gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM_GRID_COLS");
  m_GridVBO_numrows = gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM_GRID_ROWS");

  glGenBuffers( 1, &m_GridVBOVertices );
  glGenBuffers( 1, &m_GridVBOColors );

  // Lines vertex
  UpdateSkyboxVBO();

  // Shader test
  if(!gSystem_Shader_Manager.LoadShader("simpleShader", "data/shaders/simple.vert", "data/shaders/simple.frag") or !gSystem_Shader_Manager.CompileShader("simpleShader"))
  {
    gSystem_Debug.msg_box("SHADER ERROR!", "SHADER ERROR!!");
  }
}

void CSystem_Render::UpdateSkyboxVBO()
{
  if( m_GridVBO_numcols > 0 and m_GridVBO_numrows > 0)
  {
                        /*                 White lines                 */
    int nLines = (m_GridVBO_numcols + 1) + (m_GridVBO_numrows + 1); // +...

    //vector< GLfloat* > pVertices(nLines*2);
    //vector< GLfloat* > pColors(nLines*2);

    GLfloat pVertices[nLines*2][3];
    GLfloat pColors[nLines*2][3];

    /*GLfloat** pVertices = new GLfloat*[nLines*2];
    GLfloat** pColors = new GLfloat*[nLines*2];

    for(int i = 0; i < nLines*2; i++)
    {
      pVertices[i] = new GLfloat[3];
      pColors[i] = new GLfloat[3];
    }*/

    uint index = 0;
    // White vertical lines
    for(float i = 0; i <= m_GridVBO_numcols; i++, index += 2)
    {
      pVertices[index][0] = i; pVertices[index][1] = 0; pVertices[index][2] = 0; //= {index, 0, 0};
      pVertices[index+1][0] = i; pVertices[index+1][1] = 0; pVertices[index+1][2] = m_GridVBO_numrows; //pVertindexces.push_back({index, 0, (float)m_GrindexdVBO_numcols});

      pColors[index][0] = pColors[index][1] = pColors[index][2] = 1.f;
      pColors[index+1][0] = pColors[index+1][1] = pColors[index+1][2] = 1.f;
    }
    // White horizontal lines
    for(float i = 0; i <= m_GridVBO_numrows; i++, index += 2)
    {
      pVertices[index][0] = 0; pVertices[index][1] = 0; pVertices[index][2] = i; //= {index, 0, 0};
      pVertices[index+1][0] = m_GridVBO_numcols; pVertices[index+1][1] = 0; pVertices[index+1][2] = i; //pVertindexces.push_back({index, 0, (float)m_GrindexdVBO_numcols});

      pColors[index][0] = pColors[index][1] = pColors[index][2] = 1.f;
      pColors[index+1][0] = pColors[index+1][1] = pColors[index+1][2] = 1.f;

      //pColors.push_back({0.5f, 0.5f, 0.5f});
      //pColors.push_back({0.5f, 0.5f, 0.5f});
    }

    glBindBuffer( GL_ARRAY_BUFFER, m_GridVBOVertices );
    glBufferData( GL_ARRAY_BUFFER, nLines*2*3*sizeof(GLfloat), pVertices[0], GL_DYNAMIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, m_GridVBOColors );
    glBufferData( GL_ARRAY_BUFFER, nLines*2*3*sizeof(GLfloat), pColors[0], GL_DYNAMIC_DRAW );

    /*for(int i = 0; i < nLines*2; i++)
    {
      delete [] pVertices[i];
      delete [] pColors[i];
    }
    delete [] pVertices;
    delete [] pColors;*/
  }
}

void CSystem_Render::Close()
{
  //Destroy VBOs
  glDeleteBuffers(1, &m_SkyboxVBOTexCoords);
  glDeleteBuffers(1, &m_SkyboxVBOVertices);

  glDeleteBuffers(1, &m_GridVBOVertices);
  glDeleteBuffers(1, &m_GridVBOColors);

  // Other renders
  CComponent_Transform::CloseRenderVBO();

  CSystem::Close();

  gluDeleteQuadric(quadratic);

  SDL_GL_DeleteContext(GLcontext);
  SDL_DestroyWindow(window);
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
    it++;
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
    it++;
    camera_list.insert(it, camera);
    camera->Camera()->ApplyChanges();
  }
}

void CSystem_Render::RemoveCamera(const string& camera)
{
  // ouh shit
  for(vector<CGameObject*>::iterator it = camera_list.begin(); it < camera_list.end(); it++)
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

  for(vector<CGameObject*>::iterator it = camera_list.begin(); it < camera_list.end(); it++)
  {
    // Disabled camera
    if(!(*it)->IsEnabled())
      continue;

    CComponent_Camera* cam = (*it)->GetComponent<CComponent_Camera>();

    cam->BeforeRender();
    cam->SetViewport();
    cam->SetUp();
    cam->Clear();


    // Draw Skybox
    RenderSkybox(cam);
    if(gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM_GRID") )
      RenderGrid(gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM_GRID_ROWS"), gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM_GRID_COLS"));

	  for(map<string, CGameObject*>::iterator it2 = gSystem_GameObject_Manager.gameObjects.begin(); it2 != gSystem_GameObject_Manager.gameObjects.end(); it2++)
	  {
	    //glLoadIdentity();
	    glPushMatrix();

      glColor3f(1.f, 1.f, 1.f);
      glBindTexture(GL_TEXTURE_2D, 0);

      it2->second->Transform()->ApplyTransform();
	    it2->second->OnRender();

	    glPopMatrix();

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
      glPushAttrib(GL_POLYGON_BIT);

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glBindTexture(GL_TEXTURE_2D, 0);
      glColor4f(1.f, 1.f, 0.f, 1.f);

      for(map<string, CGameObject*>::iterator it2 = gSystem_GameObject_Manager.gameObjects.begin(); it2 != gSystem_GameObject_Manager.gameObjects.end(); it2++)
      {
        if(it2->second->GetComponent<CComponent_Audio_Source>())
        {
          glPushMatrix();

          it2->second->Transform()->ApplyTransform();
          it2->second->AudioSource()->OnRender();

          glPopMatrix();
        }
      }

      glPopAttrib();
    }

	  if(gSystem_Data_Storage.GetInt("__RENDER_TRANSFORM"))
	  {
	    glClear(GL_DEPTH_BUFFER_BIT);
      glBindTexture(GL_TEXTURE_2D, 0);

	    for(map<string, CGameObject*>::iterator it2 = gSystem_GameObject_Manager.gameObjects.begin(); it2 != gSystem_GameObject_Manager.gameObjects.end(); it2++)
	    {
	       glPushMatrix();

	       it2->second->Transform()->ApplyTransform();
	       it2->second->Transform()->OnRender();

	       glPopMatrix();
	    }
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

  for(vector<CComponent_GUI_Texture*>::iterator it = gui_textures.begin(); it != gui_textures.end(); it++)
  {
    (*it)->OnRender();
  }

  //glEnable(GL_DEPTH_TEST);
}

// http://www.opengl.org/wiki/Tutorial1:_Rendering_shapes_with_glDrawRangeElements,_VAO,_VBO,_shaders_(C%2B%2B_/_freeGLUT)
// http://www.opengl.org/wiki/Tutorial2:_VAOs,_VBOs,_Vertex_and_Fragment_Shaders_(C_/_SDL)
// !! http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/attributes.php

void CSystem_Render::RenderGrid(int rows, int cols)
{
  glPushMatrix();

  glBindTexture(GL_TEXTURE_2D, 0);

  GLfloat cols_scale = gSystem_Data_Storage.GetFloat("__RENDER_TRANSFORM_GRID_COLS_SCALE");
  GLfloat rows_scale = gSystem_Data_Storage.GetFloat("__RENDER_TRANSFORM_GRID_ROWS_SCALE");

  GLfloat p_projection_matrix[16];
  glGetFloatv(GL_PROJECTION_MATRIX, p_projection_matrix);

  GLfloat p_modelview_matrix[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, p_modelview_matrix);
  glm::mat4 modelview_matrix = glm::make_mat4(p_modelview_matrix);

  /*glBegin(GL_LINES);
    // Horizontal lines.

    for (int i = 0; i <= rows; i++)
    {
      if(i % 5) glColor3f(0.5f, 0.5f, 0.5f);
      else glColor3f(1.f, 1.f, 1.f);

      glVertex3f(0, 0, i*cols_size);
      glVertex3f(cols*cols_size, 0, i*cols_size);
    }
    // Vertical lines.
    for (int i = 0; i <= cols; i++)
    {
      if(i % 5) glColor3f(0.5f, 0.5f, 0.5f);
      else glColor3f(1.f, 1.f, 1.f);

      glVertex3f(i*rows_size, 0, 0);
      glVertex3f(i*rows_size, 0, rows*rows_size);
    }
  glEnd();*/

  CShader* simpleShader = gSystem_Shader_Manager.GetShader("simpleShader");
  glUseProgram(simpleShader->GetProgram());

  modelview_matrix = glm::translate(modelview_matrix, glm::vec3(-(rows*rows_scale)/2.f, 0.f, (-cols*cols_scale)/2.f)); //glTranslatef(-(rows*rows_scale)/2.f, 0.f, (-cols*cols_scale)/2.f);
  modelview_matrix = glm::scale(modelview_matrix, glm::vec3(rows_scale, 0.f, cols_scale)); //glScalef(rows_scale, 0.f, cols_scale);


  glUniformMatrix4fv(simpleShader->GetUniformIndex("ProjMatrix") , 1, GL_FALSE, &p_projection_matrix[0]);
  glUniformMatrix4fv(simpleShader->GetUniformIndex("ModelViewMatrix") , 1, GL_FALSE, glm::value_ptr(modelview_matrix));

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glBindBuffer( GL_ARRAY_BUFFER, m_GridVBOVertices );
  glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );
  glBindBuffer( GL_ARRAY_BUFFER, m_GridVBOColors );
  glColorPointer( 3, GL_FLOAT, 0, (char *) NULL );


  int nLines = (m_GridVBO_numcols + 1) + (m_GridVBO_numrows + 1);
  glDrawArrays( GL_LINES, 0, nLines*2);

  //glBindAttribLocation("in_Color");

  // Usar mejor glDrawArraysInstanced(), o algo, ya que esto es muy lento (n*m)
  /*for (int i = 0; i <= rows; i++)
  {
    for(int j = 0; j <= cols; j++)
    {
      glUniform3f(simpleShader->GetUniformIndex("in_Translation") , i, 0, j);
      glDrawArrays( GL_LINES, 0, 20 );
    }

  }*/

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);

  glUseProgram(0);

  glPopMatrix();
}


bool CSystem_Render::RenderSkybox(CComponent_Camera* cam)
{
  if(cam->skybox_texture == "")
    return false;

  // Idea sencilla: dibujamos un cubo de tamaño 1x1 sin depth_test justo donde esta la cámara.
  // Todo lo demás se dibujará encima, por lo que creará un efecto de "tamaño inmenso".
  // http://content.gpwiki.org/index.php/Sky_Box
  glPushMatrix();

  glColor3f(1.f, 1.f, 1.f);
  glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture(cam->skybox_texture )->GetID());

  vector3f position = cam->gameObject->Transform()->Position();
  glTranslatef(position.x, position.y, position.z);

  uint m_nSkyboxVertexCount = 24;

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glBindBuffer( GL_ARRAY_BUFFER, m_SkyboxVBOVertices );
  glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );
  glBindBuffer( GL_ARRAY_BUFFER, m_SkyboxVBOTexCoords );
  glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );

  glDrawArrays( GL_QUADS, 0, m_nSkyboxVertexCount );

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  glClear(GL_DEPTH_BUFFER_BIT);
  glPopMatrix();

  return true;
}

void CSystem_Render::Clear()
{
  glScissor(0, 0, gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH"), gSystem_Data_Storage.GetInt("__RESOLUTION_HEIGHT"));

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



