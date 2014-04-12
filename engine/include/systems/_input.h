#ifndef __CSYSTEM_USERINPUT_
#define __CSYSTEM_USERINPUT_

#include "_globals.h"
#include "_system.h"
#include "systems/_debug.h"

#define __CSYSTEM_USERINPUT_JOYSTICK_REBUILD_TIMEOUT 500.0f

namespace Input
{
  enum keystate_t {key_keyup = -2, key_unpressed = -1, key_keydown = 1, key_pressed = 2};
  /*
   * key_keyup     (-2) => Instante en el que se deja de presionar la tecla.
   * key_unpressed (-1) => Tecla sin presionar.
   * key_keydown   ( 1) => Instante en el que se presiona la tecla.
   * key_pressed   ( 2) => Tecla presionada.
   *
   * Ciclo normal de estados: unpressed -> keydown -> pressed -> keyup -> unpressed -> ...
   *
   * Si el valor es menor que 0, se considera la tecla como "no activada".
   * Si el valor es mayor que 0, se considera la tecla como "activada".
   */

  enum pov_pos_t {
    pov_leftup   = SDL_HAT_LEFTUP,     pov_up     = SDL_HAT_UP,             pov_rightup   = SDL_HAT_RIGHTUP,
    pov_left     = SDL_HAT_LEFT,       pov_center = SDL_HAT_CENTERED,       pov_right     = SDL_HAT_RIGHT,
    pov_leftdown = SDL_HAT_LEFTDOWN,   pov_down   = SDL_HAT_DOWN,           pov_rightdown = SDL_HAT_RIGHTDOWN
  };

  enum button_t {button_unpressed = 0, button_pressed = 1};
}

class CSystem_UserInput: public CSystem
{
  friend class CSystem_Debug;

  public:
    class CButton
    {
      friend class CSystem_UserInput;
      friend class CMouse;
      friend class CJoystick;

      protected:
        Input::keystate_t state;
        Uint8 button;
        std::string button_name;

      public:
        Input::keystate_t operator()(){return state;}
        Input::keystate_t State(){return state;}
        Uint8 Button(){return button;}
        std::string ButtonName(){return button_name;}
    };

    class CJoyButton
    {
      friend class CSystem_UserInput;
      friend class CMouse;
      friend class CJoystick;

      protected:
        Input::button_t state;
        Uint8 button;
        std::string button_name;

      public:
        Input::button_t operator()(){return state;}
        Input::button_t State(){return state;}
        Uint8 Button(){return button;}
        std::string ButtonName(){return button_name;}
    };

    class CKey
    {
      friend class CSystem_UserInput;

      protected:
        Input::keystate_t state; // Estado (presionado, no presionado, siendo presionado, siendo des-presionado)
        SDL_Scancode key;

      public:
        Input::keystate_t operator()() {return state;}
        Input::keystate_t State(){return state;}
        SDL_Scancode Key() {return key;}
        const char* KeyName() {return SDL_GetScancodeName(key);}

      protected:
        void OnInput(const CSystem_UserInput& input);
    };

    class CBall
    {
      friend class CSystem_UserInput;

      public:
        int dx, dy;
    };

    class CKeyAxis
    {
      friend class CSystem_UserInput;

      public:
        float vertical, horizontal;

        CKey v_up, v_down, h_left, h_right;

      protected:
        void OnInput(const CSystem_UserInput& input);
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

        bool Init();

        void OnLoop();
        void OnEvent();
        void OnInput();

      public:
        int x, y;
        float y_vel, x_vel;

        int wheel_x, wheel_y;

        CButton mouse1, mouse2, mouse3; // mouseX1, mouseX2;
    };

    class CJoystick
    {
      friend class CSystem_UserInput;

      protected:
        SDL_Joystick* joystick;
        std::string joystick_name;

      public:
        std::vector<CAxis> axes;         // -- read only
        std::vector<CBall> balls;        // -- read only
        std::vector<CJoyButton> buttons; // -- read only
        std::vector<Uint8> povs;         // -- read only
        /*
         * See http://wiki.libsdl.org/SDL_JoystickGetHat?highlight=%28%5CbCategoryJoystick%5Cb%29%7C%28CategoryEnum%29%7C%28CategoryStruct%29
         * pov_leftup     pov_up         pov_rightup
         * pov_left       pov_centered   pov_right
         * pov_leftdown   pov_down       pov_rightdown
         */

      protected:
        bool Init(int index);
        void Close();

        //void OnEvent();
        void OnInput();
        //void OnLoop(); // Debería usarse para comprobar si un joystick ha sido desconectado :L <- medianamente necesario

      public:
        /*vector<CAxis> GetAxes(){return axes;}
        vector<CBall> GetBalls(){return balls;}
        vector<CJoyButton> GetButtons() {return buttons;}
        vector<Uint8> GetPovs(){return povs;}*/

        inline std::string GetName(){return joystick_name;}
        inline bool CheckStatus()
        {
          if(SDL_JoystickGetAttached(joystick) == SDL_FALSE)
          {
            gDebug.console_error_msg("Joystick %s disconnected: %s", joystick_name.c_str(), SDL_GetError());
            return false;
          }
          return true;
        }
    };

  protected:
    // raw keyboard
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

    // joysticks
    std::vector<CJoystick> joysticks;

    bool rebuild_joysticks;
    float rebuild_joysticks_timeout;
    void CheckJoysticks();
      bool RebuildJoysticks();

  public:
      // Axis - para el futuro, se sopotarán joysticks
    CKeyAxis axis1; // wasd
    CKeyAxis axis2; // arrows

      // Keys/actions
    CKey action1; // n
    CKey action2; // m
    CKey action3; // j
    CKey action4; // k

    CKey run;     // L-shift
    CKey crouch;  // L-ctrl
    CKey jump;    // space

    CKey console; // F10

      // Mouse
    CMouse mouse;


  public:
    CSystem_UserInput(): CSystem() {};

    bool Init();
    void Close();
    bool Reset();

    void OnLoop();
    void OnEvent();
    void OnInput();

    Uint8 Keyboard(std::string keyname);
    Uint8 Keyboard(SDL_Scancode key);

    std::vector<CJoystick> GetJoysticks() {return joysticks;}
    /*unsigned int GetNumJoysticks() {return joysticks.size();}
    CJoystick* GetJoystick(int index)
    {
      return ((index < 0 or index > joysticks.size())? NULL : &joysticks[index]);
    }*/


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

typedef CSystem_UserInput GO_InputClasses;

// Crear 2 objetos: 1 para la configuración de la aplicación, y otra para guardar variables de usuario.
extern CSystem_UserInput gSystem_UserInput;
extern CSystem_UserInput& gUserInput;

#endif /* __CSYSTEM_USERINPUT_ */
