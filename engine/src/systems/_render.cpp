#include "_object.h"

#include "components/_component_camera.h"
#include "components/_component_transform.h"

#include "systems/_render.h"
#include "systems/_debug.h"
#include "systems/_data.h"
#include "systems/_manager.h"
#include "systems/_resource.h"

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

  if(!glewIsSupported("GL_ARB_multitexture"))
  {
    gSystem_Debug.error("From CSystem_Render: GLEW error: GL_ARB_multitexture NOT supported!");
    return false;
  }

  if(!glewIsSupported("GL_ARB_vertex_buffer_object"))
  {
    gSystem_Debug.error("From CSystem_Render: GLEW error: GL_ARB_vertex_buffer_object NOT supported!");
    return false;
  }

  InitSkyboxVBO();

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

  glGenBuffersARB( 1, &m_nVBOVertices );
  glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );
  glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_nSkyboxVertexCount*3*sizeof(GLfloat), m_pVertices, GL_STATIC_DRAW_ARB );

  glGenBuffersARB( 1, &m_nVBOTexCoords );
  glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );
  glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_nSkyboxVertexCount*2*sizeof(GLfloat), m_pTexCoords, GL_STATIC_DRAW_ARB );
}

void CSystem_Render::Close()
{
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
  GLenum error = glGetError();
  if(error != GL_NO_ERROR)
  {
    //gSystem_Debug.msg_box(debug::error, ERROR_RENDER, "From CSystem_Render: OpenGL error: %s", gluErrorString(error) );
    gSystem_Debug.console_error_msg("From CSystem_Render: OpenGL error: %s", gluErrorString(error));
  }
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
    DrawSkybox(cam);
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
  glDisable(GL_DEPTH_TEST);
  glLoadIdentity();

  GUI_Camera->Camera()->SetViewport();
  GUI_Camera->Camera()->SetUp();

  for(vector<CComponent_GUI_Texture*>::iterator it = gui_textures.begin(); it != gui_textures.end(); it++)
  {
    (*it)->OnRender();
  }
  glEnable(GL_DEPTH_TEST);
}

void CSystem_Render::RenderGrid(int rows, int cols)
{
  glPushMatrix();

  glBindTexture(GL_TEXTURE_2D, 0);

  GLfloat cols_size = gSystem_Data_Storage.GetFloat("__RENDER_TRANSFORM_GRID_COLS_SIZE");
  GLfloat rows_size = gSystem_Data_Storage.GetFloat("__RENDER_TRANSFORM_GRID_ROWS_SIZE");

  glTranslatef((-cols*cols_size)/2.f, 0.f, -(rows*rows_size)/2.f);

  glBegin(GL_LINES);
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
  glEnd();

  glPopMatrix();
}


bool CSystem_Render::DrawSkybox(CComponent_Camera* cam)
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

  glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );
  glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );
  glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );
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



