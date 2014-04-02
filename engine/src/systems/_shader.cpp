#include "systems/_shader.h"
#include "systems/_debug.h"

CSystem_Shader_Manager gSystem_Shader_Manager;
CSystem_Shader_Manager& gShader = gSystem_Shader_Manager;

using namespace std;

CShader::CShader()
{
  shader_variables.clear();
  VertexShader =  GeometricShader = FragmentShader = Program = 0;
  link_status = false;
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

const char* CSystem_Shader_Manager::DEFAULT_SHADER = "__flatShader";
const int MAX_LINE_LENGTH = 1024;

bool CSystem_Shader_Manager::Init()
{
  if(enabled) return true;

  if(!InitMainShaders())
    return false;

  last_shader_used = DEFAULT_SHADER;
  //CShader* theDefaultShader = new CShader();
  //shaders[DEFAULT_SHADER] = theDefaultShader;

  CSystem::Init();

  return true;
}

bool CSystem_Shader_Manager::InitMainShaders()
{
    // Flat shader
  const char* __flatShader_VertexCode[] =
  {
    "uniform mat4 ProjMatrix;"
    "uniform mat4 ModelViewMatrix;"

    "attribute vec4 in_Position;"
    "attribute vec4 in_Color;"

    "varying vec4 frag_Color;"

    "void main(void)"
    "{"
      "mat4 MVPMatrix = ProjMatrix * ModelViewMatrix;"
      "gl_Position = MVPMatrix * in_Position;"

      "frag_Color = in_Color;"
    "}"

  };

  const char* __flatShader_FragmentCode[] =
  {
    "varying vec4 frag_Color;"

    "void main(void)"
    "{"
      "gl_FragColor = frag_Color;"
    "}"
  };

  CShader* shader = gSystem_Shader_Manager.LoadShaderStr("__flatShader", __flatShader_VertexCode, __flatShader_FragmentCode);
  if(!shader) return false;

  glBindAttribLocation(shader->GetProgram(), 0, "in_Position");
  glBindAttribLocation(shader->GetProgram(), 1, "in_Color");

  if(!gSystem_Shader_Manager.LinkShader("__flatShader"))
      return false;

  shaders[DEFAULT_SHADER] = shader;

  // -----------------------------------------------------

    // Simple GLU shader
  const char* __simpleGLUShader_VertexCode[] =
  {
    "uniform mat4 ProjMatrix;"
    "uniform mat4 ModelViewMatrix;"
    "uniform vec4 in_Color;"

    "varying vec4 frag_Color;"

    "void main(void)"
    "{"
      "mat4 MVPMatrix = ProjMatrix * ModelViewMatrix;"
      "gl_Position = MVPMatrix * gl_Vertex;"

      "frag_Color = in_Color;"
    "}"
  };

  const char* __simpleGLUShader_FragmentCode[] =
  {
    "varying vec4 frag_Color;"

    "void main(void)"
    "{"
      "gl_FragColor = frag_Color;"
    "}"
  };

  shader = gSystem_Shader_Manager.LoadShaderStr("__simpleGLUShader", __simpleGLUShader_VertexCode, __simpleGLUShader_FragmentCode);
  if(!shader)
    return false;

  if(!gSystem_Shader_Manager.LinkShader("__simpleGLUShader"))
    return false;

  // -----------------------------------------------------

    // Texture simple shader
  // http://stackoverflow.com/questions/6686741/fragment-shader-glsl-for-texture-color-and-texture-color
  const char* __textureShader_VertexCode[] =
  {
    "uniform mat4 ProjMatrix;"
    "uniform mat4 ModelViewMatrix;"
    "uniform float textureFlag;"
    "uniform vec4 in_Color;"

    "attribute vec4 in_Position;"
    "attribute vec2 in_TexCoords;"

    "varying vec2 frag_TexCoords;"
    "varying vec4 frag_Color;"
    "varying float frag_textureFlag;"

    "void main(void)"
    "{"
        "mat4 MVPMatrix = ProjMatrix * ModelViewMatrix;"
        "gl_Position = MVPMatrix * in_Position;"

        "frag_TexCoords = in_TexCoords;"
        "frag_Color = in_Color;"
        "frag_textureFlag = textureFlag;"
    "}"
  };

  const char* __textureShader_FragmentCode[] =
  {
    "uniform sampler2D texture;"

    "varying vec2 frag_TexCoords;"
    "varying vec4 frag_Color;"
    "varying float frag_textureFlag;"

    "void main(void)"
    "{"
      "gl_FragColor = mix(frag_Color, texture2D(texture, frag_TexCoords) * frag_Color, frag_textureFlag);"
    "}"
  };

  shader = gSystem_Shader_Manager.LoadShaderStr("__textureShader", __textureShader_VertexCode, __textureShader_FragmentCode);
  if(!shader)
    return false;

  glBindAttribLocation(shader->GetProgram(), 0, "in_Position");
  glBindAttribLocation(shader->GetProgram(), 1, "in_TexCoords");

  if(!gSystem_Shader_Manager.LinkShader("__textureShader"))
    return false;

  // -----------------------------------------------------

    // Simple GLU shader
  const char* __particlesShader_VertexCode[] =
  {
    "uniform mat4 ProjMatrix;"
    "uniform mat4 ModelViewMatrix;"
    "uniform float textureFlag;"

    "attribute vec4 in_Vertex;"
    //"attribute vec2 in_TexCoords;"

    "attribute vec4 in_Position;" // <-- vec3?
    "attribute vec2 in_AngleScale;"
    "attribute vec4 in_Color;"

    "varying vec4 frag_Color;"
    "varying vec2 frag_TexCoords;"
    "varying float frag_textureFlag;"

    // Recomiendo meter todas las funciones de transformación en una única que haga todas las cosas.

    "mat4 translate(mat4 m, vec3 v)"
    "{"
      "mat4 translated = m;"
      "translated[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];"

      "return translated;"
    "}"

    "mat4 scale(mat4 m, vec3 v)"
    "{"
      "if(v == vec3(1.0, 1.0, 1.0))"
        "return m;"
      "mat4 output = mat4(0);"

      "output[0] = m[0] * v[0];"
      "output[1] = m[1] * v[1];"
      "output[2] = m[2] * v[2];"
      "output[3] = m[3];"

      "return output;"
    "}"

    "mat4 rotate(mat4 m, float angle, vec3 v)"
    "{"
      "if(angle == 0)"
        "return m;"

      "angle = radians(angle);   "

      "float c = cos(angle);"
      "float s = sin(angle);"

      "vec3 axis = normalize(v);"
      "vec3 temp = (1.0 - c) * axis;"

      "mat4 Rotate = mat4(0.0);"

      "Rotate[0][0] = c + temp[0] * axis[0];"
      "Rotate[0][1] = 0.0 + temp[0] * axis[1] + s * axis[2];"
      "Rotate[0][2] = 0.0 + temp[0] * axis[2] - s * axis[1];"

      "Rotate[1][0] = 0.0 + temp[1] * axis[0] - s * axis[2];"
      "Rotate[1][1] = c + temp[1] * axis[1];"
      "Rotate[1][2] = 0.0 + temp[1] * axis[2] + s * axis[0];"

      "Rotate[2][0] = 0.0 + temp[2] * axis[0] + s * axis[1];"
      "Rotate[2][1] = 0.0 + temp[2] * axis[1] - s * axis[0];"
      "Rotate[2][2] = c + temp[2] * axis[2];"

      "mat4 Result = mat4(0.0);"
      "Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];"
      "Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];"
      "Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];"
      "Result[3] = m[3];"

      "return Result;"
    "}"

    "mat4 makebillboard(mat4 inputmat)"
    "{"
      "mat4 outputmat = mat4(1.0);"
      "for(int i = 0; i < 3; i++)"
      "{"
        "for(int j = 0; j < 3; j++)"
        "{"
          "if(i == j)"
            "outputmat[i][j] = 1.0;"
          "else {"
            "outputmat[i][j] = 0.0; }"
        "}"
        "outputmat[i][3] = inputmat[i][3];"
      "}"

      "for(int i = 0; i < 4; i++)"
        "outputmat[3][i] = inputmat[3][i];"

      "return outputmat;"
    "}"

    "void main(void)"
    "{"
      "frag_Color = in_Color;"
      //"frag_TexCoords = in_TexCoords;"
      "frag_TexCoords = in_Vertex*2 + vec2(0.5, 0.5);"
      "frag_textureFlag = textureFlag;"

      //"mat4 MVPMatrix = translate(ModelViewMatrix, vec3(in_Position.x, in_Position.y, in_Position.z));" // Translate
      //"MVPMatrix = makebillboard(MVPMatrix);"                                                           //Makebillboard
      //"MVPMatrix = rotate(MVPMatrix, in_AngleScale[0], vec3(0.0, 0.0, 1.0));"                           // Rotate
      //"MVPMatrix = scale(MVPMatrix, vec3(in_AngleScale[1], in_AngleScale[1], 1.f));"                    // Scale
      //"MVPMatrix = ProjMatrix * MVPMatrix;"                                                             // Project

      "mat4 MVPMatrix = ProjMatrix * scale(rotate(makebillboard(translate(ModelViewMatrix, vec3(in_Position.x, in_Position.y, in_Position.z))), in_AngleScale[0], vec3(0.0, 0.0, 1.0)) , vec3(in_AngleScale[1], in_AngleScale[1], 1.f));"
      "gl_Position = MVPMatrix * in_Vertex;"
    "}"
  };

  const char* __particlesShader_FragmentCode[] =
  {
    "uniform sampler2D texture;"

    "varying vec2 frag_TexCoords;"
    "varying vec4 frag_Color;"
    "varying float frag_textureFlag;"

    "void main(void)"
    "{"
      //"gl_FragColor = mix(frag_Color, texture2D(texture, frag_TexCoords) * frag_Color, frag_textureFlag);"
      "gl_FragColor = mix(frag_Color, texture2D(texture, frag_TexCoords) * frag_Color, frag_textureFlag);"
    "}"
  };

  shader = gSystem_Shader_Manager.LoadShaderStr("__particlesShader", __particlesShader_VertexCode, __particlesShader_FragmentCode);
  if(!shader)
    return false;

  glBindAttribLocation(shader->GetProgram(), 0, "in_Vertex");
  glBindAttribLocation(shader->GetProgram(), 1, "in_Position");
  glBindAttribLocation(shader->GetProgram(), 2, "in_AngleScale");
  glBindAttribLocation(shader->GetProgram(), 3, "in_Color");

  if(!gSystem_Shader_Manager.LinkShader("__particlesShader"))
    return false;

  return true;
}

void CSystem_Shader_Manager::Close()
{
  if(!enabled) return;
  CSystem::Close();

  map<string, CShader*>::iterator iter;
  for (iter = shaders.begin(); iter != shaders.end(); ++iter)
  {
    Clear(iter->second);
    delete iter->second;
  }
  shaders.clear();
}

bool CSystem_Shader_Manager::Reset()
{
  Close();
  return Init();
}

CShader* CSystem_Shader_Manager::GetShader(const string& name)
{
  map<string, CShader*>::iterator i = shaders.find(name);
  if(i != shaders.end())
    return (i->second);

  // if load/link unsuccessfully, return default shader which program = 0
  return shaders[DEFAULT_SHADER];
}

CShader* CSystem_Shader_Manager::UseShader(const string& name)
{
  if(name == "")
  {
    glUseProgram(0);
    last_shader_used = name;

    return NULL;
  }
  else
  {
    CShader* r_shader = GetShader(name);
    if(r_shader)
    {
      if(name != last_shader_used)
      {
        glUseProgram(r_shader->GetProgram());
        last_shader_used = name;

        if(r_shader == shaders[DEFAULT_SHADER] and name != DEFAULT_SHADER)
          gSystem_Debug.console_warning_msg("From Shader Manager: Warning. Using DEFAULT_SHADER instead of \"%s\" (not exists)", name.c_str());
      }
    }

    return r_shader;
  }
}

CShader* CSystem_Shader_Manager::LoadShader(const std::string& name, const std::string& vertFile, const std::string& fragFile, const std::string& geomFile)
{
  CShader* r_shader = Load(name, vertFile, fragFile, geomFile);
  if (r_shader != NULL)
  {
    shaders[name] = r_shader;
    return r_shader;
  }
  return shaders[DEFAULT_SHADER];
}

CShader* CSystem_Shader_Manager::LinkShader(const string& name)
{
  map<string, CShader*>::iterator it = shaders.find(name);
  if(it == shaders.end())
    return NULL;

  if(it->second->link_status)
    return it->second;
  else
  {
    // link
    glLinkProgram(it->second->Program);

    // check link status
    GLint status = GL_FALSE;
    glGetProgramiv(it->second->Program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
      // The link has failed, check log info
      int logLength = 1;
      glGetProgramiv(it->second->Program, GL_INFO_LOG_LENGTH, &logLength);

      char* infoLog = (char*) malloc(logLength + 1);
      glGetProgramInfoLog(it->second->Program, logLength, &logLength, infoLog);
      gSystem_Debug.error("Failed to link the shader \"%s\": %s", name.c_str(), infoLog);
      free(infoLog);

      return NULL;
    }

    // check if the shader will run in the current OpenGL state
    glValidateProgram(it->second->Program);
    glGetProgramiv(it->second->Program, GL_VALIDATE_STATUS, &status);
    if (status != GL_TRUE)
    {
      gSystem_Debug.error("Shader \"%s\" program will not run in this OpenGL environment!", name.c_str());
      return NULL;
    }

    it->second->link_status = true;
    // the program has been loaded/linked successfully

    return it->second;
  }
}

CShader* CSystem_Shader_Manager::Load(const string& name, const string& vertexFile, const string& fragmentFile, const string& geometryFile)
{
  GLuint vertShader = 0, fragShader = 0, geomShader = 0, programShader = 0;
  bool loadStatus;

  // load and compile vertex. geometric and fragment sources
  loadStatus = LoadShader(name, GL_VERTEX_SHADER, vertexFile, vertShader);
  loadStatus &= LoadShader(name, GL_FRAGMENT_SHADER, fragmentFile, fragShader);
  // if geometry file is provided, load it
  if (geometryFile != "")
    loadStatus &= LoadShader(name, GL_GEOMETRY_SHADER_EXT, geometryFile, geomShader);

  if(!loadStatus)
    return NULL;
  else
  {
    programShader = glCreateProgram();
    if (programShader == 0)
    {
      gSystem_Debug.error("From Shader Manager: Could not generate glProgram (Shader) for %s", name.c_str());
      return NULL;
    }

    // attach the vertex and fragment shader codes, and the geometric if available
    glAttachShader(programShader, vertShader);
    if (geomShader != 0) glAttachShader(programShader, geomShader);
    glAttachShader(programShader, fragShader);

    // the program has been loaded/linked successfully
    CShader* output = new CShader;
    output->SetVertShader(vertShader);
    output->SetFragShader(fragShader);
    output->SetGeomShader(geomShader);
    output->SetProgram(programShader);

    return output;
  }
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

CShader* CSystem_Shader_Manager::LoadShaderStr(const string& name, const char** inVertCode, const char** inFragCode, const char** inGeomCode)
{
  CShader* r_shader = NULL;

  GLuint vertShader = 0, fragShader = 0, geomShader = 0, programShader = 0;
  bool loadStatus;

  // load and compile vertex. geometric and fragment sources
  loadStatus = LoadStr(name, GL_VERTEX_SHADER, inVertCode, vertShader);
  loadStatus &= LoadStr(name, GL_FRAGMENT_SHADER, inFragCode, fragShader);
  // if geometry file is provided, load it
  if (inGeomCode != NULL)
    loadStatus &= LoadStr(name, GL_GEOMETRY_SHADER_EXT, inGeomCode, geomShader);

  if(!loadStatus)
    return NULL;
  else
  {
    programShader = glCreateProgram();
    if (programShader == 0)
    {
      gSystem_Debug.error("From Shader Manager: Could not generate glProgram (Shader) for %s", name.c_str());
      return NULL;
    }

    // attach the vertex and fragment shader codes, and the geometric if available
    glAttachShader(programShader, vertShader);
    if (geomShader != 0) glAttachShader(programShader, geomShader);
    glAttachShader(programShader, fragShader);

    // the program has been loaded/linked successfully
    r_shader = new CShader;
    r_shader->SetVertShader(vertShader);
    r_shader->SetFragShader(fragShader);
    r_shader->SetGeomShader(geomShader);
    r_shader->SetProgram(programShader);

    //return r_shader;
  }

  if (r_shader != NULL)
  {
    shaders[name] = r_shader;
    return r_shader;
  }

  return shaders[DEFAULT_SHADER];
}

bool CSystem_Shader_Manager::LoadStr(const string& name, uint inShaderType, const char** inShaderCode, uint& inOutShader)
{
  // load shader source from file
  //int sourceSize = strlen(inShaderCode[0]);

  if(inShaderCode == NULL)
  {
    gSystem_Debug.error("From Shader Manager: No code strings detected when loading \"%s\".", name.c_str());
    return false;
  }

  // Get source size


  // create shader pointer
  inOutShader = glCreateShader(inShaderType);
  if(inOutShader == 0)
  {
    gSystem_Debug.error("From Shader Manager: Cannot create shader \"%s\", type: %u", name.c_str(), inShaderType);
    return false;
  }

  // compile shader
  glShaderSource(inOutShader, 1, (const GLchar**)&inShaderCode[0], NULL);
  glCompileShader(inOutShader);

  // free up the source
  /*for(int i = 0; i < sourceSize; ++i)
  {
    if(pShaderSource[i] != NULL)
      free(pShaderSource[i]);
  }
  free(pShaderSource);*/

  // check compilation success
  GLint status = GL_FALSE;
  glGetShaderiv(inOutShader, GL_COMPILE_STATUS, &status);
  if(status != GL_TRUE)
  {
    // fail to compile, check the log
    int logLength = 1;
    glGetShaderiv(inOutShader, GL_INFO_LOG_LENGTH, &logLength);

    char* infoLog = (char*) malloc(logLength + 1);
    glGetShaderInfoLog(inOutShader, logLength, &logLength, infoLog);
    gSystem_Debug.error("From Shader Manager: Failed to compile shader \"%s\"", name.c_str());
    gSystem_Debug.error("%s", infoLog);
    free(infoLog);

    return false;
  }

  return true;

  return true;
}

bool CSystem_Shader_Manager::LoadShader(const std::string& name, uint inShaderType, const std::string& inFileName, uint& inOutShader)
{
  if (inFileName.empty())
  {
    gSystem_Debug.error("From Shader Manager: \"%s\" shader's filename is empty!", name.c_str());
    return false;
  }

  // load shader source from file
  int sourceSize = 0;
  char** pShaderSource = LoadSource(sourceSize, inFileName);
  if (pShaderSource == NULL)
  {
    gSystem_Debug.error("From Shader Manager: Cannot load file source \"%s\" for shader \"%s\"", inFileName.c_str(), name.c_str());
    return false;
  }

  // create shader pointer
  inOutShader = glCreateShader(inShaderType);
  if (inOutShader == 0)
  {
    gSystem_Debug.error("From Shader Manager: Cannot create shader \"%s\", type: %u", name.c_str(), inShaderType);
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
    gSystem_Debug.error("From Shader Manager: Failed to compile shader \"%s\" from \"%s\"", name.c_str(), inFileName.c_str());
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
    gSystem_Debug.error("From Shader Manager: Cannot open file: %s\n", inFileName.c_str());
    return NULL;
  }

  // get the total length of the code
  while (fgets(line, MAX_LINE_LENGTH, pFile) != NULL)
  {
    lineLength = strlen(line);
    if (lineLength > MAX_LINE_LENGTH)
    {
      gSystem_Debug.error("From Shader Manager: Cannot read lines longer than %i characters in file: %s", MAX_LINE_LENGTH, inFileName.c_str());
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
    gSystem_Debug.error("From Shader Manager: Out of memory!\n");
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
      printf("From Shader Manager: Out of memory!\n");
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

