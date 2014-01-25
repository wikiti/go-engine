#ifndef __CSYSTEM_USERINPUT_
#define __CSYSTEM_USERINPUT_

#include "_globals.h"
#include "_system.h"

// Puede ser de utilidad plantear el uso de teclas tal que así :D
namespace GO_Keystates
{
  enum keystate_t {unpressed = -2, keyup = -1, pressed = 1, keydown = 2 };
}

class CSystem_UserInput: public CSystem
{
  friend class CSystem_Debug;

  protected:
    class CKey
    {
      friend class CSystem_UserInput;

      protected:
        GO_Keystates::keystate_t state;
        SDL_Scancode key;

      public:
        GO_Keystates::keystate_t operator()() {return state;}
        GO_Keystates::keystate_t State(){return state;}
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
    CKey action1; // n
    CKey action2; // m
    CKey action3; // j
    CKey action4; // k

    CKey run;     // L-shift
    CKey crouch;  // L-ctrl
    CKey jump;    // space

      // Mouse
    class CMouse
    {
      protected:
        class CButton
        {
          friend class CSystem_UserInput;
          friend class CMouse;

          protected:
            GO_Keystates::keystate_t state;
            Uint8 button;
            string button_name;

          public:
            GO_Keystates::keystate_t operator()() {return state;}
            GO_Keystates::keystate_t State(){return state;}
            Uint8 Button() {return button;}
            string ButtonName(){ return button_name;}
        };

      public:
        int x, y;
        float y_vel, x_vel;

        int wheel_x, wheel_y;

        CButton mouse1, mouse2, mouse3; // mouseX1, mouseX2;

        void OnEvent();
        void OnKeyEvent();
    };
    CMouse mouse;

  public:
    CSystem_UserInput(): CSystem() {};

    bool Init();
    void Close();

    void OnEvent();
    void OnKeyEvent();

    Uint8 Keyboard(string keyname);
    Uint8 Keyboard(SDL_Scancode key);

    CAxis GetAxis1(){return axis1;}
    CAxis GetAxis2(){return axis2;}

      // Keys/actions
    //bool GetFire1(){return fire1();}
    //bool GetFire2(){return fire2();}
    GO_Keystates::keystate_t GetAction1(){return action1();}
    GO_Keystates::keystate_t GetAction2(){return action2();}
    GO_Keystates::keystate_t GetAction3(){return action3();}
    GO_Keystates::keystate_t GetAction4(){return action4();}

    GO_Keystates::keystate_t GetRun(){return run();}
    GO_Keystates::keystate_t GetCrouch(){return crouch();}
    GO_Keystates::keystate_t GetJump(){return jump();}

};

// Crear 2 objetos: 1 para la configuración de la aplicación, y otra para guardar variables de usuario.
extern CSystem_UserInput gSystem_UserInput;
extern CSystem_UserInput& gUserInput;

#endif /* __CSYSTEM_USERINPUT_ */
