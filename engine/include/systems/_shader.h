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
  protected:
    map<string, int> shader_variables;

    uint VertexShader;
    uint GeometricShader;
    uint FragmentShader;
    uint Program;

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

    int GetAttributeIndex(const string& varname);
    int GetUniformIndex(const string& varname);

  protected:
    int GetVariableIndex(const string& varname, bool isUniform);
};

class CSystem_Shader_Manager: public CSystem
{
  private:
    map<string, CShader*> shaders;
    static const char* DEFAULT_SHADER;

  public:
    CSystem_Shader_Manager(): CSystem() {};
    ~CSystem_Shader_Manager() {};

    bool Init();
    void Close();

    //void OnLoop();

    CShader* LoadShader(const string& name, const string& vertFile, const string& fragFile = "", const string& geomFile = "");
    CShader* GetShader(const string& name);

  private:
    CShader* Load(const string& name, const string& vertexFile, const string& fragmentFile, const string& geometryFile = "");
    void Clear(CShader* inShader);

    bool LoadShader(const string& name, uint inShaderType,  const std::string& inFileName, uint& inOutShader);
    // ¿?
    char** LoadSource(int& outLineCount, const std::string& inFileName);
};

extern CSystem_Shader_Manager gSystem_Shader_Manager;
extern CSystem_Shader_Manager& gShader;

#endif /* __CSYSTEM_SHADER_MANAGER_H_ */
