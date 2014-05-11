#ifndef __SYSTEM_RESOURCES_H_
#define __SYSTEM_RESOURCES_H_

#include "_globals.h"
#include "systems/_system.h"

namespace Resources
{
  enum types_t {base, mesh, texture, sound, font};
  enum enum_loadgltexture {texture_none = 0x00, texture_mipmap = 0x01, texture_linear = 0x02, texture_nearest = 0x04 }; // metodos para cargar la textura
}

class CResource
{
  protected:
    friend class CSystem_Resources;
    std::string rc_file;

    Resources::types_t type;

  public:
    CResource();
    virtual ~CResource();

    virtual bool LoadFile(std::string file, std::string arguments = ""){return true;};
    virtual void Clear(){};

    Resources::types_t Type(){ return type; };

    std::string File(){ return rc_file; }
};

class CResource_Mesh: public CResource
{
  private:
    friend class CSystem_Resources;
    /*vector<GLfloat> vertexArray;
    vector<GLfloat> normalArray;
    vector<GLfloat> uvArray;*/

    int numTriangles, numUvCoords;
    GLuint m_ModelVBOVertices;
    GLuint m_ModelVBONormals;
    GLuint m_ModelVBOTexCoords;
    GLuint m_ModelVAO;

  public:
    CResource_Mesh(): CResource(){ numTriangles = numUvCoords = 0; m_ModelVBOVertices = m_ModelVBONormals = m_ModelVBOTexCoords = 0; type = Resources::mesh; };
    ~CResource_Mesh(){ Clear(); }

    bool LoadFile(std::string file, std::string arguments = "");
    void Clear();

    void Render();
};

class CResource_Texture: public CResource
{
  protected:
    GLuint ID;

  public:
    CResource_Texture(): CResource(){ type = Resources::texture; };
    ~CResource_Texture(){ Clear(); }

    bool LoadFile(std::string file, std::string arguments = "mipmap");
    bool LoadFromMemory(GLuint* data, uint w, uint h, uint ch, flags_t flags = 0x00);
    void Clear();

    inline int width()
    {
      int w;
      glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &w);
      return w;
    }

    inline int height()
    {
      int h;
      glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
      return h;
    }

    GLuint GetID()
    {
      return ID;
    }
};

class CResource_Sound: public CResource
{
  protected:
    friend class CSystem_Mixer;

    ALuint buffer_id;

  public:
    CResource_Sound(): CResource(){ type = Resources::sound; };
    ~CResource_Sound(){ Clear(); }

    ALuint BufferID()
    {
      return buffer_id;
    }

    /*void PlaySound();
    void StopSound();
    void PauseSound();
    void RewindSound();*/

    bool LoadFile(std::string file, std::string arguments);
    void Clear();
};

class CResource_Font: public CResource
{

};

// Etc

/**
 * Formato fichero de resources (.rc):
 * ----------------------------------
 * # Comentario
 *
 * # Linea superior en blanco
 * # Formato de resource:
 * # tipo: nombre_rc ruta_al_fichero
 * mesh: nombre ruta/fichero
 * # Fin de fichero
 */

class CSystem_Resources: public CSystem
{
  protected:
    std::map<std::string, CResource*> resource_list;
    bool InitEngineResources();

    std::map<std::string, CResource*> GetList()
    {
      return resource_list;
    };

  public:
    CSystem_Resources();
    virtual ~CSystem_Resources();

    bool Init();
    void Close();
    bool Reset();

    bool LoadResourceFile(std::string rc_file);
      bool LoadResource(std::string name, std::string rc_file, Resources::types_t type, std::string arguments = "");
    void AddEmpty(std::string name);

    void ClearNonEngineResources();
    void ClearResources();
      void ClearResource(std::string name);

    CResource_Mesh* GetMesh(std::string id);
    CResource_Texture* GetTexture(std::string id);
    CResource_Sound* GetSound(std::string id);

    //CResource* Get(std::string id);
};

extern CSystem_Resources gSystem_Resources;
extern CSystem_Resources& gResources;

#endif /* __SYSTEM_RESOURCES_H_ */
