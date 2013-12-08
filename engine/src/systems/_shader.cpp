#include "systems/_shader.h"
#include "systems/_debug.h"

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

// Shader manager
/*class CSystem_Shader_Manager: public CSystem
{
  private:
    map<string, CShader*> shaders;
    static const char* DEFAULT_SHADER;

  public:
    CSystem_Shader_Manager();
    ~CSystem_Shader_Manager();

    bool Init();
    void Close();

    //void OnLoop();

    CShader* LoadShader(const string& name, const string& vertFile, const string& fragFile = "", const string& geomFile = "");
    CShader* GetShader(const string& name);

  private:
    CShader* Load(const string& name, const string& vertexFile, const string& fragmentFile, const string& geometryFile);
    void Clear(CShader* inShader);

    bool LoadShader(const string& name, uint inShaderType,  const std::string& inFileName, uint& inOutShader);
    char** LoadSource(const string& name, int& outLineCount, const std::string& inFileName);
};*/

const char* CSystem_Shader_Manager::DEFAULT_SHADER = "__DefaultShader";
const int MAX_LINE_LENGTH = 1024;

bool CSystem_Shader_Manager::Init()
{
  CShader* theDefaultShader = new CShader();
  shaders[DEFAULT_SHADER] = theDefaultShader;

  return true;
}

void CSystem_Shader_Manager::Close()
{
  map<string, CShader*>::iterator iter;
  for (iter = shaders.begin(); iter != shaders.end(); ++iter)
  {
    Clear(iter->second);
    delete iter->second;
  }
  shaders.clear();
}

CShader* CSystem_Shader_Manager::GetShader(const string& name)
{
  map<string, CShader*>::iterator i = shaders.find(name);
  if(i != shaders.end())
    return (i->second);

  // if load/link unsuccessfully, return default shader which program = 0
  return shaders[DEFAULT_SHADER];
}

CShader* CSystem_Shader_Manager::LoadShader(const string& name, const string& vertFile, const string& fragFile, const string& geomFile)
{
  CShader* r_shader = Load(name, vertFile, fragFile, geomFile);
  if (r_shader != NULL)
  {
    shaders[name] = r_shader;
    return r_shader;
  }
  return shaders[DEFAULT_SHADER];
}

CShader* CSystem_Shader_Manager::Load(const string& name, const string& vertexFile, const string& fragmentFile, const string& geometryFile)
{
  unsigned int vertShader = 0, fragShader = 0, geomShader = 0, programShader = 0;
  bool loadStatus;

  // load and compile vertex. geometric and fragment sources
  loadStatus = LoadShader(name, GL_VERTEX_SHADER, vertexFile, vertShader);
  loadStatus &= LoadShader(name, GL_FRAGMENT_SHADER, fragmentFile, fragShader);
  // if geometry file is provided, load it
  if (geometryFile != "")
    loadStatus &= LoadShader(name, GL_GEOMETRY_SHADER_EXT, geometryFile, geomShader);

  if (loadStatus)
  {
    // create a program
    programShader = glCreateProgram();

    if (programShader == 0)
      return NULL;

    // attach the vertex and fragment shader codes, and the geometric if available
    glAttachShader(programShader, vertShader);

    if (geomShader != 0)
      glAttachShader(programShader, geomShader);

    glAttachShader(programShader, fragShader);

    // link
    glLinkProgram(programShader);

    // check link status
    GLint status = GL_FALSE;
    glGetProgramiv(programShader, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
      // The link has failed, check log info
      int logLength = 1;
      glGetProgramiv(programShader, GL_INFO_LOG_LENGTH, &logLength);

      char* infoLog = (char*) malloc(logLength + 1);
      glGetProgramInfoLog(programShader, logLength, &logLength, infoLog);
      gSystem_Debug.error("Failed to link the shader \"%s\": %s", name.c_str(), infoLog);
      free(infoLog);

      return NULL;
    }

    // check if the shader will run in the current OpenGL state
    glValidateProgram(programShader);
    glGetProgramiv(programShader, GL_VALIDATE_STATUS, &status);
    if (status != GL_TRUE)
    {
      gSystem_Debug.error("Shader \"%s\" program will not run in this OpenGL environment!", name.c_str());
      return NULL;
    }

    // the program has been loaded/linked successfully
    CShader* theResult = new CShader;
    theResult->SetVertShader(vertShader);
    theResult->SetFragShader(fragShader);
    theResult->SetGeomShader(geomShader);
    theResult->SetProgram(programShader);

    return theResult;
  }

  return NULL;
}

void CSystem_Shader_Manager::Clear(CShader* inShader)
{
  if (inShader->GetProgram() != 0)
  {
    glDeleteProgram(inShader->GetProgram());
  }

  if (inShader->GetVert() != 0)
  {
    glDeleteShader(inShader->GetVert());
  }

  if (inShader->GetGeom() != 0)
  {
    glDeleteShader(inShader->GetGeom());
  }

  if (inShader->GetFrag() != 0)
  {
    glDeleteShader(inShader->GetFrag());
  }
}

bool CSystem_Shader_Manager::LoadShader(const string& name, uint inShaderType,  const std::string& inFileName, uint& inOutShader)
{
  if (inFileName.empty())
  {
    gSystem_Debug.error("\"%s\" shader's filename is empty!", name.c_str());
    return false;
  }

  // load shader source from file
  int sourceSize = 0;
  char** pShaderSource = LoadSource(sourceSize, inFileName);
  if (pShaderSource == NULL)
  {
    gSystem_Debug.error("Cannot load file source \"%s\" for shader \"%s\"", inFileName.c_str(), name.c_str());
    return false;
  }

  // create shader pointer
  inOutShader = glCreateShader(inShaderType);
  if (inOutShader == 0)
  {
    gSystem_Debug.error("Cannot create shader \"%s\", type: %u", name.c_str(), inShaderType);
    return false;
  }

  // compile shader
  glShaderSource(inOutShader, sourceSize, (const GLchar**) pShaderSource, NULL);
  glCompileShader(inOutShader);

  // free up the source
  for (int i = 0; i < sourceSize; ++i)
  {
    if (pShaderSource[i] != NULL)
      free(pShaderSource[i]);
  }
  free(pShaderSource);

  // check compilation success
  GLint status = GL_FALSE;
  glGetShaderiv(inOutShader, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE)
  {
    // fail to compile, check the log
    int logLength = 1;
    glGetShaderiv(inOutShader, GL_INFO_LOG_LENGTH, &logLength);

    char* infoLog = (char*) malloc(logLength + 1);
    glGetShaderInfoLog(inOutShader, logLength, &logLength, infoLog);
    gSystem_Debug.error("Failed to compile shader \"%s\" from \"%s\"", name.c_str(), inFileName.c_str());
    gSystem_Debug.error("%s", infoLog);
    free(infoLog);

    return false;
  }

  return true;
}

char** CSystem_Shader_Manager::LoadSource(int& outLineCount, const std::string& inFileName)
{
  char line[MAX_LINE_LENGTH];
  int length = 0, lineLength;
  GLchar** pSource;

  outLineCount = 0;
  FILE* pFile = fopen(inFileName.c_str(), "r");
  if (pFile == NULL)
  {
    gSystem_Debug.error("Cannot open file: %s\n", inFileName.c_str());
    return NULL;
  }

  // get the total length of the code
  while (fgets(line, MAX_LINE_LENGTH, pFile) != NULL)
  {
    lineLength = strlen(line);
    if (lineLength > MAX_LINE_LENGTH)
    {
      gSystem_Debug.error("Cannot read lines longer than %i characters in file: %s", MAX_LINE_LENGTH, inFileName.c_str());
      outLineCount = 0;
      fclose(pFile);
      return NULL;
    }
    length += lineLength + 1;
    ++outLineCount;
  }

  rewind(pFile);
  pSource = (GLchar**) malloc(sizeof(GLchar*) * (outLineCount));
  if (pSource == NULL)
  {
    gSystem_Debug.error("Out of memory!\n");
    return NULL;
  }

  length = 0;
  int i = 0;
  while (fgets(line, MAX_LINE_LENGTH, pFile) != NULL)
  {
    lineLength = strlen(line);
    pSource[i] = (GLchar*) malloc(sizeof(GLchar) * (lineLength + 1));
    if (pSource[i] == NULL)
    {
      printf("Out of memory!\n");
      return NULL;
    }

    GLchar* ps = pSource[i];
    GLchar* pl = line;

    // concatenates the string
    while (*pl != '\0')
      *ps++ = *pl++;

    *ps++ = '\0';
    ++i;
  }

  fclose(pFile);

  return pSource;
}

