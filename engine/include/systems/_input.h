#ifndef __CSYSTEM_USERINPUT_
#define __CSYSTEM_USERINPUT_

#include "_globals.h"
#include "_system.h"

/*
 * keyup     (-2) => Instante en el que se deja de presionar la tecla.
 * unpressed (-1) => Tecla sin presionar.
 * keydown   ( 1) => Instante en el que se presiona la tecla.
 * pressed   ( 2) => Tecla presionada.
 *
 * Ciclo normal de estados: unpressed -> keydown -> pressed -> keyup -> unpressed -> ...
 *
 * Si el valor es menor que 0, se considera la tecla como "no activada".
 * Si el valor es mayor que 0, se considera la tecla como "activada".
 */

namespace GO_Keystates
{
  enum keystate_t {keyup = -2, unpressed = -1, keydown = 1, pressed = 2};
}

class CSystem_UserInput: public CSystem
{
  friend class CSystem_Debug;

  protected:
    class CButton
    {
        friend class CSystem_UserInput;
        friend class CMouse;
        friend class CJoystick;

      protected:
        GO_Keystates::keystate_t state;
        Uint8 button;
        string button_name;

      public:
        GO_Keystates::keystate_t operator()(){return state;}
        GO_Keystates::keystate_t State(){return state;}
        Uint8 Button(){return button;}
        string ButtonName(){return button_name;}
    };

    class CKey
    {
      friend class CSystem_UserInput;

      protected:
        GO_Keystates::keystate_t state; // Estado (presionado, no presionado, siendo presionado, siendo des-presionado)
        SDL_Scancode key;

      public:
        GO_Keystates::keystate_t operator()() {return state;}
        GO_Keystates::keystate_t State(){return state;}
        SDL_Scancode Key() {return key;}
        const char* KeyName() {return SDL_GetScancodeName(key);}
    };

    class CKeyAxis
    {
      friend class CSystem_UserInput;

      public:
       float vertical;
       float horizontal;

       CKey v_up, v_down, h_left, h_right;
    };

    class CAxis
    {
      friend class CSystem_UserInput;

      public:
       float value;
    };

    class CMouse
    {
      friend class CSystem_UserInput;

      protected:
        bool moved, scrolled;
        bool mouse1_key, mouse2_key, mouse3_key;

      public:
        int x, y;
        float y_vel, x_vel;

        int wheel_x, wheel_y;

        CButton mouse1, mouse2, mouse3; // mouseX1, mouseX2;

        void OnLoop();
        void OnEvent();
        void OnKeyEvent();

    };

    class CJoystick
    {
      friend class CSystem;

      protected:
        SDL_Joystick* joystick;

        vector<CAxis> axis;
        vector<CAxis[2]> balls;
        vector<CButton> buttons;

        void Init();
        void Close();

        void OnEvent();
        void OnKeyEvent();
        void OnLoop();

      public:
        vector<CAxis> GetAxis(){return axis;}
        vector<CAxis[2]> GetBalls(){return balls;}
        vector<CButton> GetButtons() {return buttons;}
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
    CMouse mouse;

      // Joysticks
    vector<CJoystick> joysticks;

  public:
    CSystem_UserInput(): CSystem() {};

    bool Init();
    void Close();

    void OnLoop();
    void OnEvent();
    void OnKeyEvent();

    Uint8 Keyboard(string keyname);
    Uint8 Keyboard(SDL_Scancode key);

    /*CAxis GetAxis1(){return axis1;}
    CAxis GetAxis2(){return axis2;}

      // Keys/actions
    GO_Keystates::keystate_t GetAction1(){return action1();}
    GO_Keystates::keystate_t GetAction2(){return action2();}
    GO_Keystates::keystate_t GetAction3(){return action3();}
    GO_Keystates::keystate_t GetAction4(){return action4();}

    GO_Keystates::keystate_t GetRun(){return run();}
    GO_Keystates::keystate_t GetCrouch(){return crouch();}
    GO_Keystates::keystate_t GetJump(){return jump();}*/

      // Mouse stuff
    void ShowMouse(bool show = true);
    void SetMousePos(int x = INT_MAX, int y = INT_MAX);
    void SetMouseTrap(bool mode = true);

};

// Crear 2 objetos: 1 para la configuración de la aplicación, y otra para guardar variables de usuario.
extern CSystem_UserInput gSystem_UserInput;
extern CSystem_UserInput& gUserInput;

#endif /* __CSYSTEM_USERINPUT_ */
