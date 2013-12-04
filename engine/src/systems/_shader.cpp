#include "systems/_shader.h"

CSystem_Shader_Manager gSystem_Shader_Manager;
CSystem_Shader_Manager& gShader = gSystem_Shader_Manager;

CShader::CShader()
{
  shader_variables.clear();
  VertexShader =  GeometricShader = FragmentShader = Program = 0;
}

CShader::~CShader()
{
  shader_variables.clear();
  VertexShader =  GeometricShader = FragmentShader = Program = 0;
}

int CShader::GetAttributeIndex(const string& varname)
{
  return GetVariableIndex(varname, false);
}

int CShader::GetUniformIndex(const string& varname)
{
  return GetVariableIndex(varname, true);
}

int CShader::GetVariableIndex(const string& varname, bool isUniform)
{
  int output = -1;
  if (Program)
  {
    map<string, int>::iterator it = shader_variables.find(varname);
    if (it == shader_variables.end())
    {
      if (isUniform) // uniform variables
        output = glGetUniformLocation(Program, varname.c_str());
      else // attribute variables
        output = glGetAttribLocation(Program, varname.c_str());
      if(output != -1)
        shader_variables[varname] = output;
    }
    else
      output = it->second;
  }
  return output;
}
