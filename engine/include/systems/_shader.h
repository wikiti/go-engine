#ifndef __CSYSTEM_SHADER_MANAGER_H_
#define __CSYSTEM_SHADER_MANAGER_H_

#include "_globals.h"
#include "_system.h"

/* References:
 *   - http://luugiathuy.com/2012/04/glsl-shader-manager-opengl/
 *   - https://github.com/luugiathuy/GLSLShaderManager
 */

// ¿Deberíamos poner los shaders como Resources?
// ¿O dejarlos como sistema independiente? <-

class CShader
{
  friend class CSystem_Shader_Manager;
  protected:
    std::map<std::string, int> shader_variables;

    uint VertexShader;
    uint GeometricShader;
    uint FragmentShader;
    uint Program;

    bool link_status;

    //enum type_t {attribute = false, uniform = true};

  public:
    CShader();
    ~CShader();

    inline uint GetVert() { return VertexShader; }
    inline uint GetGeom() { return GeometricShader; }
    inline uint GetFrag() { return FragmentShader; }
    inline uint GetProgram() { return Program; }

    inline void SetVertShader(uint value) { VertexShader = value; }
    inline void SetGeomShader(uint value) { GeometricShader = value; }
    inline void SetFragShader(uint value) { FragmentShader = value; }
    inline void SetProgram(uint value) { Program = value; }

    int GetAttributeIndex(const std::string& varname);
    int GetUniformIndex(const std::string& varname);

  protected:
    int GetVariableIndex(const std::string& varname, bool isUniform);
};

class CSystem_Shader_Manager: public CSystem
{
  private:
    std::map<std::string, CShader*> shaders;
    static const char* DEFAULT_SHADER;

    std::string last_shader_used;

  public:
    CSystem_Shader_Manager(): CSystem() {};
    ~CSystem_Shader_Manager() {};

    bool Init();
      bool InitMainShaders();
    void Close();
    bool Reset();

    //void OnLoop();

    CShader* LoadShader(const std::string& name, const std::string& vertFile, const std::string& fragFile = "", const std::string& geomFile = "");
    CShader* LinkShader(const std::string& name);
    CShader* GetShader(const std::string& name);

    CShader* UseShader(const std::string& name = "");

  private:
    CShader* Load(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile, const std::string& geometryFile = "");
    void Clear(CShader* inShader);

    bool LoadShader(const std::string& name, uint inShaderType, const std::string& inFileName, uint& inOutShader);
      char** LoadSource(int& outLineCount, const std::string& inFileName);

    CShader* LoadShaderStr(const std::string& name, const char** inVertCode, const char** inFragCode, const char** inGeomCode = NULL);
    bool LoadStr(const std::string& name, uint inShaderType, const char** inShaderCode, uint& inOutShader);
    // ¿?
};

extern CSystem_Shader_Manager gSystem_Shader_Manager;
extern CSystem_Shader_Manager& gShader;

#endif /* __CSYSTEM_SHADER_MANAGER_H_ */
