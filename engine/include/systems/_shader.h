#ifndef __CSYSTEM_SHADER_MANAGER_H_
#define __CSYSTEM_SHADER_MANAGER_H_

#include "_globals.h"
#include "_system.h"

class CSystem_Shader_Manager: public CSystem
{

  private:

  public:
    CSystem_Shader_Manager() {};

    bool Init();
    void Close();

    void OnLoop();
};

extern CSystem_Shader_Manager gSystem_Shader_Manager;
extern CSystem_Shader_Manager& gShader;

#endif /* __CSYSTEM_SHADER_MANAGER_H_ */
