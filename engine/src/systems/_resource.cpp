#include "systems/_resource.h"
#include "systems/_debug.h"
#include "systems/_mixer.h"

CSystem_Resources gSystem_Resources;

CResource::CResource(): rc_file(""), type(resources::base)
{

}

CResource::~CResource()
{
  Clear();
}

/** Mesh **/

bool CResource_Mesh::LoadFile(string file, string arguments)
{
  // http://nickthecoder.wordpress.com/2013/01/20/mesh-loading-with-assimp/
  Assimp::Importer importer;
  //aiProcessPreset_TargetRealtime_Fast has the configs you'll need
  const aiScene *scene = importer.ReadFile(file.c_str() ,aiProcessPreset_TargetRealtime_Fast);
  if(!scene)
  {
    gSystem_Debug.console_error_msg("From Resource %s: ASSIMP: (%s).", file.c_str(), importer.GetErrorString());
    return false;
  }

  aiMesh *mesh = scene->mMeshes[0];

  if(!mesh)
  {
    gSystem_Debug.console_error_msg("From Resource %s: No mesh detected.", file.c_str());
    return false;
  }

  numTriangles = mesh->mNumFaces*3;
  numUvCoords = mesh->GetNumUVChannels();

  /*if (mesh->HasPositions())      vertexArray.resize(mesh->mNumFaces*3*3);
  if (mesh->HasNormals())        normalArray.resize(mesh->mNumFaces*3*3);
  if (mesh->HasTextureCoords(0)) uvArray.resize(mesh->mNumFaces*3*2);

  for(unsigned int i=0;i<mesh->mNumFaces;i++)
  {
    const aiFace& face = mesh->mFaces[i];

    //foreach index
    for(int j=0;j<3;j++)//assume all faces are triangulated
    {
      if (mesh->HasTextureCoords(0))
      {
        aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
        for(uint k = 0; k < 2; k++)
          uvArray.push_back(uv[k]);
      }

      if (mesh->HasNormals())
      {
        aiVector3D normal = mesh->mNormals[face.mIndices[j]];
        for(uint k = 0; k < 3; k++)
          normalArray.push_back(normal[k]);
      }

      if (mesh->HasPositions())
      {
        aiVector3D pos = mesh->mVertices[face.mIndices[j]];
        for(uint k = 0; k < 3; k++)
          vertexArray.push_back(pos[k]);
      }
    }
  }*/

  vertexArray = new float[mesh->mNumFaces*3*3];
  normalArray = new float[mesh->mNumFaces*3*3];
  uvArray =     new float[mesh->mNumFaces*3*2];

  for(unsigned int i=0;i<mesh->mNumFaces;i++)
  {
    const aiFace& face = mesh->mFaces[i];

    //foreach index
    for(int j=0;j<3;j++)//assume all faces are triangulated
    {
      if (mesh->HasTextureCoords(0))
      {
        aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
        memcpy(uvArray,&uv,sizeof(float)*2);
        uvArray+=2;
      }

      if (mesh->HasNormals())
      {
        aiVector3D normal = mesh->mNormals[face.mIndices[j]];
        memcpy(normalArray,&normal,sizeof(float)*3);
        normalArray+=3;
      }

      if (mesh->HasPositions())
      {
        aiVector3D pos = mesh->mVertices[face.mIndices[j]];
        memcpy(vertexArray,&pos,sizeof(float)*3);
        vertexArray+=3;
      }
    }
  }

  if (mesh->HasTextureCoords(0))  uvArray     -= mesh->mNumFaces*3*2;
  if (mesh->HasNormals())         normalArray -= mesh->mNumFaces*3*3;
  if (mesh->HasPositions())       vertexArray -= mesh->mNumFaces*3*3;

  rc_file = file;

  return true;
}

void CResource_Mesh::Clear()
{
  /*vertexArray.clear();
  normalArray.clear();
  uvArray.clear();*/

  if(vertexArray) delete vertexArray;
  if(normalArray) delete normalArray;
  if(uvArray)     delete uvArray;

  vertexArray = normalArray = uvArray = NULL;
  numTriangles = numUvCoords = 0;
}

void CResource_Mesh::Render()
{
  //http://nickthecoder.wordpress.com/2013/01/20/mesh-loading-with-assimp/
  // Tal vez es conveniente no llamar a todas estas de golpe
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3,GL_FLOAT, 0, &vertexArray[0]);
  glNormalPointer(GL_FLOAT, 0, &normalArray[0]);

  //glClientActiveTexture(GL_TEXTURE0_ARB);

  glTexCoordPointer(2, GL_FLOAT, 0, &uvArray[0]);

  glDrawArrays(GL_TRIANGLES, 0, numTriangles);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

/** Texture **/

bool CResource_Texture::LoadFile(string file, string arguments)
{
  using namespace resources;

  uint soil_flags = 0;
  uint flags = texture_linear;
  if(arguments == "mipmap")
    flags = texture_mipmap;
  else if(arguments == "linear")
    flags = texture_linear;
  else if(arguments == "nearest")
    flags = texture_nearest;
  else
    gSystem_Debug.console_error_msg("From Resource %s: Unknow flag \"%s\" for texture. Using \"linear\"", file.c_str(), arguments.c_str());

  soil_flags |= SOIL_FLAG_INVERT_Y;
  if(flags == texture_mipmap)
    soil_flags |= SOIL_FLAG_MIPMAPS;

  ID = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, soil_flags);
  if(ID == 0)
  {
    gSystem_Debug.console_error_msg("From Resource %s: Internal error of SOIL (%s).", file.c_str(), SOIL_last_result());
    return false;
  }

  glBindTexture(GL_TEXTURE_2D, ID);
  switch(flags)
  {
    case texture_linear:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    break;
    case texture_nearest:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    break;
    case texture_mipmap:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    break;
    default: break;
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  rc_file = file;

  return true;
}

bool CResource_Texture::LoadFromMemory(GLuint* data, uint w, uint h, uint n_ch, flags_t flags)
{
  using namespace resources;

  uint soil_flags = 0x00;
  soil_flags |= SOIL_FLAG_INVERT_Y;
  if(flags == texture_mipmap)
    soil_flags |= SOIL_FLAG_MIPMAPS;

  ID = SOIL_create_OGL_texture((GLubyte*)data, w, h, n_ch, SOIL_CREATE_NEW_ID, soil_flags);
  if(ID == 0)
  {
    gSystem_Debug.console_error_msg("SOIL Error: %s", SOIL_last_result());
    return false;
  }

  glBindTexture(GL_TEXTURE_2D, ID);
  switch(flags)
  {
    case texture_linear:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    break;
    case texture_nearest:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    break;
    case texture_mipmap:
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    break;
    default: break;
  }

  return true;
}

void CResource_Texture::Clear()
{
  glDeleteTextures(1, &ID);
  ID = 0;
}


/** Sound **/

bool CResource_Sound::LoadFile(string file, string arguments)
{
  buffer_id = 0;

  stringstream ss(arguments);
  string arg;

  /*ss >> arg;
  while(arg != "")
  {
    if(arg == "loop")
      loop = true;

    // esto sobra ya
    else if(arg[0] == 's' && arg.length() > 1)
    {
      stringstream ss2( arg.substr(1) );
      ALuint source_id;
      ss2 >> source_id;
      if(source_id > 0 && source_id < CSystem_Mixer::NUMBER_SOURCES)
      {
        source_attached = gSystem_Mixer.GetSourceID(source_id);
      }
      else
      {
        gSystem_Debug.console_error_msg("Error from Resource Manager: Invalid source identifier for \"%s\".", file.c_str());
        gSystem_Debug.console_error_msg("Must be between 1 and 30. Using source 1.");
        source_attached = gSystem_Mixer.GetSourceID(1);
      }
    }

    ss >> arg;
    if(ss.eof()) break;
  }*/

  Mix_Chunk *sound = Mix_LoadWAV(file.c_str());
  if(!sound)
  {
    gSystem_Debug.console_error_msg("Error from Resource Manager: Could not load sound file \"%s\": %s.", file.c_str(), Mix_GetError());
    return false;
  }

  ALenum error;

  alGenBuffers(1, &buffer_id);
  error = alGetError();
  if (error != AL_NO_ERROR)
  {
    gSystem_Debug.console_error_msg("Error from Resource Manager: Could not generate buffer for sound file \"%s\": %d.", file.c_str(), error);
    Mix_FreeChunk(sound);

    return false;
  }

  alBufferData(buffer_id, AL_FORMAT_MONO16, sound->abuf, sound->alen, 44100);
  Mix_FreeChunk(sound);

  rc_file = file;

  return true;
}

void CResource_Sound::Clear()
{
  alDeleteBuffers(1, &buffer_id);
}

/** Resources System **/

CSystem_Resources::CSystem_Resources()
{

}

CSystem_Resources::~CSystem_Resources()
{
  ClearResources();
}

bool CSystem_Resources::Init()
{
  CSystem::Init();
  InitEngineResources();

  return true;
}

bool CSystem_Resources::InitEngineResources()
{
  using namespace resources;

  /** Texturas **/
  CResource_Texture* texture_error = new CResource_Texture;

  GLuint texture_error_data[128*128];

  for(uint i = 0; i < 128*128; i++ )
  {
    GLubyte* colors = (GLubyte*)&texture_error_data[i];
    if( (i / 128 & 16) ^ ((i % 128) & 16) )
    {
      colors[0] = 0xFF;
      colors[1] = 0x00;
      colors[2] = 0xFF;
      colors[3] = 0xFF;
    }
    else
    {
      colors[0] = 0x00;
      colors[1] = 0x00;
      colors[2] = 0x00;
      colors[3] = 0x00;
    }
  }

  texture_error->LoadFromMemory(texture_error_data, 128, 128, 4, texture_linear);
  resource_list.insert(pair<string, CResource*>("__NO_TEXTURE", texture_error));

  /** Modelos **/
  // ¿?

  return true;
}


void CSystem_Resources::Close()
{
  ClearResources();
  CSystem::Close();
}

bool CSystem_Resources::LoadResourceFile(string rc_file)
{
  // Parse txt (or binary) file

  ifstream is(rc_file.c_str());
  string line;

  if(!is || !is.good())
  {
    line = "Error from Resource Manager: Could not load file: " + rc_file;
    gSystem_Debug.msg_box(ERROR_FILE, line.c_str());
    return false;
  }

  while(getline(is, line, '\n'))
  {
    //gSystem_Debug.log("Linea: %s", line.c_str());
    if(line[0] == '#' || line[0] == '\0' ) // Ignore comments and blank lines
      continue;

    stringstream ss(line);
    string type, name, file, arguments;

    ss >> type >> name;
    getline(ss, file, ':');
    getline(ss, arguments);

    if(!gValidateIdentifier(name))
    {
      gSystem_Debug.console_warning_msg("Error from Resource Manager: Invalid resource name \"%s\" (%s): Can only contain alphanumerics or underscores.", name.c_str(), file.c_str());
      continue;
    }

    if(file.length() > 1) file = file.substr(1);
    if(arguments.length() > 1) arguments = arguments.substr(1);

    if(type == "mesh:")
      LoadResource(name, file, resources::mesh, arguments);
    else if(type == "sound:")
      LoadResource(name, file, resources::sound, arguments);
    else if(type == "texture:")
      LoadResource(name, file, resources::texture, arguments);
    else if(type == "font:")
      LoadResource(name, file, resources::font, arguments);
    else
      gSystem_Debug.error("Error from Resource Manager: Invalid resource type \"%s\" for resource\"%s\".", type.c_str(), name.c_str());
  }

  is.close();

  return true;
}

bool CSystem_Resources::LoadResource(string name, string rc_file, resources::types_t type, string arguments)
{
  // Si ya existe, borrar y poner de nuevo. Si no, crear de 0
  map<string, CResource*>::iterator it = resource_list.find(name);
  if(it != resource_list.end())
    ClearResource(name);

  CResource* new_rc = NULL;
  switch(type)
  {
    case resources::mesh: new_rc = new CResource_Mesh; break;
    case resources::texture: new_rc = new CResource_Texture; break;
    case resources::sound: new_rc = new CResource_Sound; break;
    case resources::font: new_rc = new CResource_Font; break;
    default: gSystem_Debug.error("From CSystem_Resources: From Resource %s: Invalid resource type.", name.c_str(), rc_file.c_str()); return false;
  }

  if(!new_rc->LoadFile(rc_file, arguments))
  {
    gSystem_Debug.error("From CSystem_Resources: Could not load file: %s", rc_file.c_str());
    delete new_rc;
    return false;
  }

  resource_list.insert(pair<string, CResource*>(name, new_rc));

  return true;
}

void CSystem_Resources::AddEmpty(string name)
{
  // Si ya existe, borrar y poner de nuevo. Si no, crear de 0
  CResource* new_rc = NULL;

  map<string, CResource*>::iterator it = resource_list.find(name);
  if(it != resource_list.end())
    ClearResource(name);

  resource_list.insert(pair<string, CResource*>(name, new_rc));
}

void CSystem_Resources::ClearResources()
{
  for(map<string, CResource*>::iterator it = resource_list.begin(); it != resource_list.end(); it++)
  {
    //it->second->Clear();
    delete it->second;
    //resource_list.erase(it);
  }
  resource_list.clear();
}

void CSystem_Resources::ClearResource(string id)
{
  map<string, CResource*>::iterator it = resource_list.find(id);
  if(it != resource_list.end())
  {
    //it->second->Clear();
    delete it->second;
    resource_list.erase(it);
  }
}

CResource_Mesh* CSystem_Resources::GetMesh(string id)
{
  map<string, CResource*>::iterator it = resource_list.find(id);
  if(it != resource_list.end() && it->second->Type() == resources::mesh)
    return (CResource_Mesh*)(it->second);

  // Devolver mesh nula en vez de NULL

  return NULL;
}

CResource_Texture* CSystem_Resources::GetTexture(string id)
{
  map<string, CResource*>::iterator it = resource_list.find(id);
  if(it != resource_list.end() && it->second->Type() == resources::texture)
    return (CResource_Texture*)(it->second);

  // Devolver textura nula en vez de NULL
  //return NULL;
  return (CResource_Texture*)resource_list["__NO_TEXTURE"];
}

CResource_Sound* CSystem_Resources::GetSound(string id)
{
  map<string, CResource*>::iterator it = resource_list.find(id);
  if(it != resource_list.end() && it->second->Type() == resources::sound)
    return (CResource_Sound*)(it->second);

  return NULL;
}

