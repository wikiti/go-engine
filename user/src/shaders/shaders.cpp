#include "shaders/shaders.h"

const char* VERTEX_SHADER_FILE_NAME          = "data/shaders/simple.vert";
const char* FRAGMENT_SHADER_FILE_NAME        = "data/shaders/simple.frag";

const char* SHADER_UNIF_PROJMAT              = "ProjMatrix";


bool SetGameObjects_Instance1_Shaders()
{
  //CShader* simpleShader = gShader.LoadShader("example1", VERTEX_SHADER_FILE_NAME, FRAGMENT_SHADER_FILE_NAME);
  //glUseProgram(simpleShader->GetProgram());
  //glUniformMatrix4fv(simpleShader->GetUniformIndex(SHADER_UNIF_PROJMAT), 1, GL_FALSE, &projMatrix[0]);

  return true;
}


