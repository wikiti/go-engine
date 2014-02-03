#ifndef __SYSTEM_RESOURCES_H_
#define __SYSTEM_RESOURCES_H_

#include "_globals.h"
#include "systems/_system.h"

namespace resources
{
  enum types_t {base, mesh, texture, sound, font};
  enum enum_loadgltexture {texture_mipmap = 0x00, texture_linear = 0x01, texture_nearest = 0x02 }; // metodos para cargar la textura
}

class CResource
{
  protected:
    friend class CSystem_Resources;
    string rc_file;

    resources::types_t type;

  public:
    CResource();
    virtual ~CResource();

    virtual bool LoadFile(string file, string arguments = ""){return true;};
    virtual void Clear(){};

    resources::types_t Type(){ return type; };

    string File(){ return rc_file; }
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
    CResource_Mesh(): CResource(){ numTriangles = numUvCoords = 0; m_ModelVBOVertices = m_ModelVBONormals = m_ModelVBOTexCoords = 0; type = resources::mesh; };
    ~CResource_Mesh(){ Clear(); }

    bool LoadFile(string file, string arguments = "");
    void Clear();

    void Render();
};

class CResource_Texture: public CResource
{
  protected:
    GLuint ID;

  public:
    CResource_Texture(): CResource(){ type = resources::texture; };
    ~CResource_Texture(){ Clear(); }

    bool LoadFile(string file, string arguments = "mipmap");
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
    CResource_Sound(): CResource(){ type = resources::sound; };
    ~CResource_Sound(){ Clear(); }

    ALuint BufferID()
    {
      return buffer_id;
    }

    /*void PlaySound();
    void StopSound();
    void PauseSound();
    void RewindSound();*/

    bool LoadFile(string file, string arguments);
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
    map<string, CResource*> resource_list;
    bool InitEngineResources();

    map<string, CResource*> GetList()
    {
      return resource_list;
    };

  public:
    CSystem_Resources();
    virtual ~CSystem_Resources();

    bool Init();
    void Close();

    bool LoadResourceFile(string rc_file);
      bool LoadResource(string name, string rc_file, resources::types_t type, string arguments = "");
    void AddEmpty(string name);

    void ClearResources();
      void ClearResource(string name);

    CResource_Mesh* GetMesh(string id);
    CResource_Texture* GetTexture(string id);
    CResource_Sound* GetSound(string id);

    //CResource* Get(string id);
};

extern CSystem_Resources gSystem_Resources;


#endif /* __SYSTEM_RESOURCES_H_ */
