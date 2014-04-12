#include "systems/_input.h"
#include "systems/_data.h"
#include "systems/_debug.h"
#include "systems/_render.h"

CSystem_UserInput gSystem_UserInput;
CSystem_UserInput& gUserInput = gSystem_UserInput;

using namespace std;

//const Uint8* CSystem_UserInput::keyboard = SDL_GetKeyboardState(NULL);

using namespace Input;

void CSystem_UserInput::CKey::OnInput(const CSystem_UserInput& input = gSystem_UserInput)
{
  if ((state == Input::key_keyup or state == Input::key_unpressed) and input.keyboard[key])
    state = Input::key_keydown;
  else if ((state == Input::key_keydown or state == Input::key_pressed) and !input.keyboard[key])
    state = Input::key_keyup;
  else if (state == Input::key_keydown and input.keyboard[key])
    state = Input::key_pressed;
  else if (state == Input::key_keyup and !input.keyboard[key])
    state = Input::key_unpressed;
}

void CSystem_UserInput::CKeyAxis::OnInput(const CSystem_UserInput& input = gSystem_UserInput)
{
  // ---- Para los ejes, usamos máquinas de estado para alternar entre los estados unpressed, keydown, key_pressed y keyup.
  // - Up
  if((v_up.state == key_keyup or v_up.state == key_unpressed) and input.keyboard[v_up.key])
  {
    vertical += 1.0;
    v_up.state = key_keydown;
  }
  else if((v_up.state == key_keydown or v_up.state == key_pressed) and !input.keyboard[v_up.key])
  {
    vertical -= 1.0;
    v_up.state = key_keyup;
  }
  else if(v_up.state == key_keydown and input.keyboard[v_up.key])
    v_up.state = key_pressed;
  else if(v_up.state == key_keyup and !input.keyboard[v_up.key])
    v_up.state = key_unpressed;

  // - Down
  if((v_down.state == key_keyup or v_down.state == key_unpressed) and input.keyboard[v_down.key])
  {
    vertical -= 1.0;
    v_down.state = key_keydown;
  }
  else if((v_down.state == key_keydown or v_down.state == key_pressed) and !input.keyboard[v_down.key])
  {
    vertical += 1.0;
    v_down.state = key_keyup;
  }
  else if(v_down.state == key_keydown and input.keyboard[v_down.key])
    v_down.state = key_pressed;
  else if(v_down.state == key_keyup and !input.keyboard[v_down.key])
    v_down.state = key_unpressed;

  // - Left
  if((h_left.state == key_keyup or h_left.state == key_unpressed) and input.keyboard[h_left.key])
  {
    horizontal -= 1.0;
    h_left.state = key_keydown;
  }
  else if((h_left.state == key_keydown or h_left.state == key_pressed) and !input.keyboard[h_left.key])
  {
    horizontal += 1.0;
    h_left.state = key_keyup;
  }
  else if(h_left.state == key_keydown and input.keyboard[h_left.key])
    h_left.state = key_pressed;
  else if(h_left.state == key_keyup and !input.keyboard[h_left.key])
    h_left.state = key_unpressed;

  // - Right
  if((h_right.state == key_keyup or h_right.state == key_unpressed) and input.keyboard[h_right.key])
  {
    horizontal += 1.0;
    h_right.state = key_keydown;
  }
  else if((h_right.state == key_keydown or h_right.state == key_pressed) and !input.keyboard[h_right.key])
  {
    horizontal -= 1.0;
    h_right.state = key_keyup;
  }
  else if(h_right.state == key_keydown and input.keyboard[h_right.key])
    h_right.state = key_pressed;
  else if(h_right.state == key_keyup and !input.keyboard[h_right.key])
    h_right.state = key_unpressed;
}

bool CSystem_UserInput::Init()
{
  if(enabled) return true;

  // Axis
  axis1.horizontal = axis2.horizontal = 0;
  axis1.vertical = axis2.vertical = 0;

  axis1.v_up.state = axis1.v_down.state = axis1.h_left.state = axis1.h_right.state = key_unpressed;
  axis2.v_up.state = axis2.v_down.state = axis2.h_left.state = axis2.h_right.state = key_unpressed;

  axis1.v_up.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS1_UP_KEY").c_str());
  axis1.v_down.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS1_DOWN_KEY").c_str());
  axis1.h_left.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS1_LEFT_KEY").c_str());
  axis1.h_right.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS1_RIGHT_KEY").c_str());

  axis2.v_up.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS2_UP_KEY").c_str());
  axis2.v_down.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS2_DOWN_KEY").c_str());
  axis2.h_left.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS2_LEFT_KEY").c_str());
  axis2.h_right.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS2_RIGHT_KEY").c_str());

    // Actions
  action1.state = action2.state = action3.state = action4.state = key_unpressed;
  action1.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_ACTION1_KEY").c_str());
  action2.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_ACTION2_KEY").c_str());
  action3.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_ACTION3_KEY").c_str());
  action4.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_ACTION4_KEY").c_str());

  run.state = crouch.state = jump.state = key_unpressed;
  run.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_RUN_KEY").c_str());
  crouch.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_CROUCH_KEY").c_str());
  jump.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_JUMP_KEY").c_str());

  console.state = key_unpressed;
  console.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_CONSOLE_KEY").c_str());

    // Mouse
  if(!mouse.Init())
  {
    gSystem_Debug.msg_box(Debug::error, ERROR_INIT, "From UserInput: Could not load mouse device");
  }

    // Joystick
  if(!RebuildJoysticks())
  {
    gSystem_Debug.msg_box(Debug::error, ERROR_INIT, "From UserInput: Could not load joysticks devices");
  }
  rebuild_joysticks = false;
  rebuild_joysticks_timeout = 0.f;

  CSystem::Init();

  return true;
}

void CSystem_UserInput::Close()
{
  if(!enabled) return;
  CSystem::Close();

    // Joystick
  //SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
  for(vector<CJoystick>::iterator it = joysticks.begin(); it != joysticks.end(); ++it)
  {
    (*it).Close();
  }
  joysticks.clear();
}

bool CSystem_UserInput::Reset()
{
  // Un poco brusco, pero es lo ideal
  Close();

  return Init();
}

void CSystem_UserInput::OnInput()
{
  // Axis
  axis1.OnInput();
  axis2.OnInput();

  // -- Actions
  action1.OnInput();
  action2.OnInput();
  action3.OnInput();
  action4.OnInput();

  run.OnInput();
  crouch.OnInput();
  jump.OnInput();

  console.OnInput();

  // Mouse
  mouse.OnInput();

  // Comprobar estado antes de actualizar, por seguridad
  //CheckJoysticks();
  for(vector<CJoystick>::iterator it = joysticks.begin(); it != joysticks.end(); ++it)
    (*it).OnInput();
}

void CSystem_UserInput::OnEvent()
{
  mouse.OnEvent();
}

void CSystem_UserInput::OnLoop()
{
  mouse.OnLoop();
  CheckJoysticks();
}

void CSystem_UserInput::CheckJoysticks()
{
  // Wait for rebuilding...
  if(rebuild_joysticks)
  {
    if(gSystem_Time.GetTicks() - rebuild_joysticks_timeout < __CSYSTEM_USERINPUT_JOYSTICK_REBUILD_TIMEOUT)
      return;

    gSystem_Debug.console_warning_msg("Rebuilding joystick list...");
    RebuildJoysticks();
    gSystem_Debug.console_info_msg("Joystick rebuild done!");

    rebuild_joysticks = false;
  }

  // New joystick?
  if((unsigned int)SDL_NumJoysticks() > joysticks.size())
  {
    gSystem_Debug.console_info_msg("New joystick detected!");
    rebuild_joysticks = true;
    rebuild_joysticks_timeout = gSystem_Time.GetTicks();

    // Clear joysticks to avoid errors.
    joysticks.clear();

    return;
  }

  // Some other joystick is disconnected?
  for(vector<CJoystick>::iterator it = joysticks.begin(); it != joysticks.end(); ++it)
  {
    if(!(*it).CheckStatus())
    {
      rebuild_joysticks = true;
      rebuild_joysticks_timeout = gSystem_Time.GetTicks();

      // Clear joysticks to avoid errors.
      joysticks.clear();

      return;
    }
  }
}

bool CSystem_UserInput::RebuildJoysticks()
{
  for(vector<CJoystick>::iterator it = joysticks.begin(); it != joysticks.end(); ++it)
    (*it).Close();

  joysticks.clear();

  SDL_JoystickUpdate();
  if (int njoy = SDL_NumJoysticks() > 0)
  {
    joysticks.resize(njoy);
    for(vector<CJoystick>::iterator it = joysticks.begin(); it != joysticks.end(); ++it)
    {
      int index = it - joysticks.begin(); // Truco sencillo para sacar el índice de un iterador
      if(!(*it).Init(index))
      {
        gSystem_Debug.console_error_msg("Error loading joystick %d", index);
        joysticks.erase(it);
        it--;
      }
    }
  }

  return true;
}

/**_______Keyboard_______**/

Uint8 CSystem_UserInput::Keyboard(std::string keyname)
{
  SDL_Scancode key = SDL_GetScancodeFromName(keyname.c_str());
  return Keyboard(key);
}

Uint8 CSystem_UserInput::Keyboard(SDL_Scancode key)
{
  if(key == SDL_SCANCODE_UNKNOWN)
    return 0; // Simulamos que no se pulsa la tecla
  return keyboard[key];
}


/**________Mouse_________**/

bool CSystem_UserInput::CMouse::Init()
{
  x = y = 0;
  y_vel = x_vel = 0;
  wheel_x = wheel_y = 0;

  mouse1.state = mouse2.state = mouse3.state = key_unpressed;
  mouse1.button = SDL_BUTTON_LEFT; mouse1.button_name = "Mouse Left";
  mouse2.button = SDL_BUTTON_RIGHT; mouse2.button_name = "Mouse Right";
  mouse3.button = SDL_BUTTON_MIDDLE; mouse2.button_name = "Mouse Middle";
  moved = scrolled = false;

  return true;
}


void CSystem_UserInput::CMouse::OnLoop()
{
  SDL_GetMouseState(&x, &y);

  if(!moved)
    x_vel = y_vel = 0;
  if(!scrolled)
    wheel_x = wheel_y = 0;

  moved = scrolled = false;
}

void CSystem_UserInput::CMouse::OnEvent()
{
  if(event.type == SDL_MOUSEMOTION)
  {
    x_vel = event.motion.xrel;
    y_vel = event.motion.yrel;

    moved = true;
  }
  else if(event.type == SDL_MOUSEWHEEL)
  {
    wheel_x = event.wheel.x;
    wheel_y = event.wheel.y;

    scrolled = true;
  }
  // Para simplificar, usamos 2 estados (pulsado y no-pulsado)
  else if(event.type == SDL_MOUSEBUTTONDOWN)
  {
    if (event.button.button == mouse1.button and (mouse1.state == key_unpressed or mouse1.state == key_keyup))
    {
      mouse1.state = key_keydown;
      mouse1_key = true;
    }
    else if (event.button.button == mouse2.button and (mouse2.state == key_unpressed or mouse2.state == key_keyup))
    {
      mouse2.state = key_keydown;
      mouse2_key = true;
    }
    else if (event.button.button == mouse3.button and (mouse3.state == key_unpressed or mouse3.state == key_keyup))
    {
      mouse3.state = key_keydown;
      mouse3_key = true;
    }
  }
  else if (event.type == SDL_MOUSEBUTTONUP)
  {
    if (event.button.button == mouse1.button and (mouse1.state == key_pressed or mouse1.state == key_keydown))
    {
      mouse1.state = key_keyup;
      mouse1_key = true;
    }
    else if (event.button.button == mouse2.button and (mouse2.state == key_pressed or mouse2.state == key_keydown))
    {
      mouse2.state = key_keyup;
      mouse2_key = true;
    }
    else if (event.button.button == mouse3.button and (mouse3.state == key_pressed or mouse3.state == key_keydown))
    {
      mouse3.state = key_keyup;
      mouse3_key = true;
    }
  }
}

void CSystem_UserInput::CMouse::OnInput()
{
  if(!mouse1_key)
  {
    if(mouse1.state == key_keydown)
      mouse1.state = key_pressed;
    else if(mouse1.state == key_keyup)
      mouse1.state = key_unpressed;
  }
  if(!mouse2_key)
  {
    if(mouse2.state == key_keydown)
      mouse2.state = key_pressed;
    else if(mouse2.state == key_keyup)
      mouse2.state = key_unpressed;
  }
  if(!mouse3_key)
  {
    if(mouse3.state == key_keydown)
      mouse3.state = key_pressed;
    else if(mouse3.state == key_keyup)
      mouse3.state = key_unpressed;
  }

  mouse1_key = mouse2_key = mouse3_key = false;
}

void CSystem_UserInput::ShowMouse(bool show)
{
  SDL_ShowCursor(show);
}

void CSystem_UserInput::SetMousePos(int x, int y)
{
  // Screen center
  if(x == INT_MAX && y == INT_MAX)
  {
    gSystem_Render.GetWindowSize(&x, &y);
    x /= 2;
    y /= 2;
  }

  SDL_WarpMouseInWindow(gSystem_Render.GetWindow(), x, y);
}

void CSystem_UserInput::SetMouseTrap(bool mode)
{
  if(mode) SDL_SetRelativeMouseMode(SDL_TRUE);
  else     SDL_SetRelativeMouseMode(SDL_FALSE);
}


/**_______Joystick_______**/

bool CSystem_UserInput::CJoystick::Init(int index)
{
  joystick = SDL_JoystickOpen(index);
  if(!joystick)
    return false;

  joystick_name = SDL_JoystickNameForIndex(index);
  axes.resize(SDL_JoystickNumAxes(joystick));
  balls.resize(SDL_JoystickNumBalls(joystick));
  buttons.resize(SDL_JoystickNumButtons(joystick));
  povs.resize(SDL_JoystickNumHats(joystick));

  for(vector<CAxis>::iterator it = axes.begin(); it != axes.end(); ++it)
    (*it).value = 0.f;

  for(vector<CBall>::iterator it = balls.begin(); it != balls.end(); ++it)
   (*it).dx = (*it).dy = 0;

  for(vector<CJoyButton>::iterator it = buttons.begin(); it != buttons.end(); ++it)
  {
    uint button_index = it - buttons.begin();

    (*it).button = button_index;
    (*it).state = button_unpressed;

    stringstream ss;
    ss << "JOY" << index << " [" << joystick_name << "] Button" << button_index;
    (*it).button_name = ss.str();
  }

  for(vector<Uint8>::iterator it = povs.begin(); it != povs.end(); ++it)
    (*it) = pov_center;

  return true;
}

void CSystem_UserInput::CJoystick::Close()
{
  axes.clear();
  buttons.clear();
  balls.clear();
  povs.clear();

  SDL_JoystickUpdate();
  if(SDL_JoystickGetAttached(joystick))
    SDL_JoystickClose(joystick);
}

/*void CSystem_UserInput::CJoystick::OnEvent()
{

}*/

// ->NOTA Al parecer, la estructura de joysticks de SDL2 es un poco inestable, por lo que desconectar un mando en medio de la ejecución puede hacer que la aplicación crashee. Tal vez lo arreglen con el tiempo...
void CSystem_UserInput::CJoystick::OnInput()
{
  // Evitar problemas, o algo
  //SDL_JoystickUpdate();

  // Suponemos operaciones atómicas, por lo que solo comprobamos al principio.
  if(SDL_JoystickGetAttached(joystick) == SDL_FALSE)
    return;

  for(vector<CAxis>::iterator it = axes.begin(); it != axes.end(); ++it)
    if(SDL_JoystickGetAttached(joystick) == SDL_TRUE)
      (*it).value = SDL_JoystickGetAxis(joystick, (it - axes.begin()))/32768.f;

  for(vector<Uint8>::iterator it = povs.begin(); it != povs.end(); ++it)
    if(SDL_JoystickGetAttached(joystick) == SDL_TRUE)
      (*it) = SDL_JoystickGetHat(joystick, it - povs.begin());

  for(vector<CBall>::iterator it = balls.begin(); it != balls.end(); ++it)
    if(SDL_JoystickGetAttached(joystick) == SDL_TRUE and SDL_JoystickGetBall(joystick, it - balls.begin(), &(*it).dx, &(*it).dy) < 0)
      gSystem_Debug.console_error_msg("Error reading Ball%d from joystick %s: %s", it - balls.begin(), joystick_name.c_str(), SDL_GetError());

  for(vector<CJoyButton>::iterator it = buttons.begin(); it != buttons.end(); ++it)
    if(SDL_JoystickGetAttached(joystick) == SDL_TRUE)
      (*it).state = static_cast<button_t>((int)SDL_JoystickGetButton(joystick, it - buttons.begin())); // 1 -> pressed, 0 -> unpressed
}

/*void CSystem_UserInput::CJoystick::OnLoop()
{

}*/
