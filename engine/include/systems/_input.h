#ifndef __CSYSTEM_USERINPUT_
#define __CSYSTEM_USERINPUT_

#include "_globals.h"
#include "_system.h"

// Puede ser de utilidad plantear el uso de teclas tal que así :D
/*namespace GO_Keystates
{
  enum keystates_t {unpressed = 0, pressed, keydown, keyup, };
}*/

class CSystem_UserInput: public CSystem
{
  friend class CSystem_Debug;

  protected:
    class CKey
    {
      friend class CSystem_UserInput;

      protected:
        bool state;
        SDL_Scancode key;

      public:
        bool operator()() {return state;}
        bool State(){return state;}
        SDL_Scancode Key() {return key;}
        const char* KeyName() {return SDL_GetScancodeName(key);}
    };

    class CAxis
    {
      friend class CSystem_UserInput;

      public:
       float vertical;
       float horizontal;

       CKey v_up, v_down, h_left, h_right;
    };

    // raw keyboard
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);;

  public:
      // Axis - para el futuro, se sopotarán joysticks
    CAxis axis1; // wasd
    CAxis axis2; // arrows

      // Keys/actions
    //CKey fire1;   // mouse1
    //CKey fire2;   // mouse2
    CKey action1; // n
    CKey action2; // m
    CKey action3; // j
    CKey action4; // k

    CKey run;     // L-shift
    CKey crouch;  // L-ctrl
    CKey jump;    // space

  public:
    CSystem_UserInput(): CSystem() {};

    bool Init();
    void Close();

    void OnEvent();
    void OnLoop();

    Uint8 Keyboard(string keyname);
    Uint8 Keyboard(SDL_Scancode key);

    CAxis GetAxis1(){return axis1;}
    CAxis GetAxis2(){return axis2;}

      // Keys/actions
    //bool GetFire1(){return fire1();}
    //bool GetFire2(){return fire2();}
    bool GetAction1(){return action1();}
    bool GetAction2(){return action2();}
    bool GetAction3(){return action3();}
    bool GetAction4(){return action4();}

    bool GetRun(){return run();}
    bool GetCrouch(){return crouch();}
    bool GetJump(){return jump();}

};

// Crear 2 objetos: 1 para la configuración de la aplicación, y otra para guardar variables de usuario.
extern CSystem_UserInput gSystem_UserInput;
extern CSystem_UserInput& gUserInput;

#endif /* __CSYSTEM_USERINPUT_ */
