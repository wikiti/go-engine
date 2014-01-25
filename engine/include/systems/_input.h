#ifndef __CSYSTEM_USERINPUT_
#define __CSYSTEM_USERINPUT_

#include "_globals.h"
#include "_system.h"

class CSystem_UserInput: public CSystem
{
  protected:
    friend class CSystem_Debug;


  public:
    CSystem_UserInput(): CSystem() {};

    bool Init();
    void Close();
};

// Crear 2 objetos: 1 para la configuración de la aplicación, y otra para guardar variables de usuario.
extern CSystem_UserInput gSystem_UserInput;
extern CSystem_UserInput& gUserInput;

#endif /* __CSYSTEM_USERINPUT_ */
