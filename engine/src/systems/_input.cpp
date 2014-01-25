#include "systems/_input.h"

CSystem_UserInput gSystem_UserInput;
CSystem_UserInput& gUserInput = gSystem_UserInput;

const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

bool CSystem_UserInput::Init()
{
    // Axis
  axis1.horizontal = axis2.horizontal = 0;
  axis1.vertical = axis2.vertical = 0;


    // Actions
  fire1.state = fire2.state = false;
  action1.state = action2.state = action3.state = false;
  run.state = crouch.state = jump.state = false;

  return true;
}

void CSystem_UserInput::Close()
{

}

void CSystem_UserInput::OnEvent()
{

}

Uint8 CSystem_UserInput::Keyboard(string keyname)
{
  SDL_Scancode key = SDL_GetScancodeFromName(keyname.c_str());
  return Keyboard(key);
}

Uint8 CSystem_UserInput::Keyboard(SDL_Scancode key)
{
  if(key == SDL_SCANCODE_UNKNOWN)
    return -1;
  return keyboard[key];
}
