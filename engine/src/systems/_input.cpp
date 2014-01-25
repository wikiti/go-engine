#include "systems/_input.h"
#include "systems/_data.h"
#include "systems/_debug.h"

CSystem_UserInput gSystem_UserInput;
CSystem_UserInput& gUserInput = gSystem_UserInput;

//const Uint8* CSystem_UserInput::keyboard = SDL_GetKeyboardState(NULL);

bool CSystem_UserInput::Init()
{

  // Axis
  axis1.horizontal = axis2.horizontal = 0;
  axis1.vertical = axis2.vertical = 0;

  axis1.v_up.state = axis1.v_down.state = axis1.h_left.state = axis1.h_right.state = false;
  axis2.v_up.state = axis2.v_down.state = axis2.h_left.state = axis2.h_right.state = false;

  axis1.v_up.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS1_UP_KEY").c_str());
  axis1.v_down.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS1_DOWN_KEY").c_str());
  axis1.h_left.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS1_LEFT_KEY").c_str());
  axis1.h_right.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS1_RIGHT_KEY").c_str());

  axis2.v_up.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS2_UP_KEY").c_str());
  axis2.v_down.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS2_DOWN_KEY").c_str());
  axis2.h_left.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS2_LEFT_KEY").c_str());
  axis2.h_right.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_AXIS2_RIGHT_KEY").c_str());

    // Actions
  //fire1.state = fire2.state = false;
  action1.state = action2.state = action3.state = action4.state = false;
  run.state = crouch.state = jump.state = false;

  return true;
}

void CSystem_UserInput::Close()
{

}

void CSystem_UserInput::OnEvent()
{
    // Axis1
  // Up
  if(keyboard[axis1.v_up.key] and !axis1.v_up.state)
  {
    axis1.vertical += 1.0;
    axis1.v_up.state = true;
  }
  else if(!keyboard[axis1.v_up.key] and axis1.v_up.state)
  {
    axis1.vertical -= 1.0;
    axis1.v_up.state = false;
  }
  // Down
  if(keyboard[axis1.v_down.key] and !axis1.v_down.state)
  {
    axis1.vertical -= 1.0;
    axis1.v_down.state = true;
  }
  else if(!keyboard[axis1.v_down.key] and axis1.v_down.state)
  {
    axis1.vertical += 1.0;
    axis1.v_down.state = false;
  }
  // Left
  if(keyboard[axis1.h_left.key] and !axis1.h_left.state)
  {
    axis1.horizontal -= 1.0;
    axis1.h_left.state = true;
  }
  else if(!keyboard[axis1.h_left.key] and axis1.h_left.state)
  {
    axis1.horizontal += 1.0;
    axis1.h_left.state = false;
  }
  // Right
  if(keyboard[axis1.h_right.key] and !axis1.h_right.state)
  {
    axis1.horizontal += 1.0;
    axis1.h_right.state = true;
  }
  else if(!keyboard[axis1.h_right.key] and axis1.h_right.state)
  {
    axis1.horizontal -= 1.0;
    axis1.h_right.state = false;
  }
  /*if(event.type == SDL_KEYDOWN)
  {
    if(event.key.keysym.scancode == axis1.v_up.key)
    {
      axis1.vertical += 1.0;
      axis1.v_up.state = true;
    }
    else if(event.key.keysym.scancode == axis1.v_down.key)
    {
      axis1.vertical -= 1.0;
      axis1.v_down.state = true;
    }
    else if(event.key.keysym.scancode == axis1.h_left.key)
    {
      axis1.horizontal -= 1.0;
      axis1.h_left.state = true;
    }
    else if(event.key.keysym.scancode == axis1.h_right.key)
    {
      axis1.horizontal += 1.0;
      axis1.h_right.state = true;
    }
  }
  else if(event.type == SDL_KEYUP)
  {
    if(event.key.keysym.scancode == axis1.v_up.key)
    {
      axis1.vertical -= 1.0;
      axis1.v_up.state = false;
    }
    else if(event.key.keysym.scancode == axis1.v_down.key)
    {
      axis1.vertical += 1.0;
      axis1.v_down.state = false;
    }
    else if(event.key.keysym.scancode == axis1.h_left.key)
    {
      axis1.horizontal += 1.0;
      axis1.h_left.state = false;
    }
    else if(event.key.keysym.scancode == axis1.h_right.key)
    {
      axis1.horizontal -= 1.0;
      axis1.h_right.state = false;
    }
  }*/

    // Axis2
  // Up
  if (keyboard[axis2.v_up.key] and !axis2.v_up.state)
  {
    axis2.vertical += 1.0;
    axis2.v_up.state = true;
  }
  else if (!keyboard[axis2.v_up.key] and axis2.v_up.state)
  {
    axis2.vertical -= 1.0;
    axis2.v_up.state = false;
  }
  // Down
  if (keyboard[axis2.v_down.key] and !axis2.v_down.state)
  {
    axis2.vertical -= 1.0;
    axis2.v_down.state = true;
  }
  else if (!keyboard[axis2.v_down.key] and axis2.v_down.state)
  {
    axis2.vertical += 1.0;
    axis2.v_down.state = false;
  }
  // Left
  if (keyboard[axis2.h_left.key] and !axis2.h_left.state)
  {
    axis2.horizontal -= 1.0;
    axis2.h_left.state = true;
  }
  else if (!keyboard[axis2.h_left.key] and axis2.h_left.state)
  {
    axis2.horizontal += 1.0;
    axis2.h_left.state = false;
  }
  // Right
  if (keyboard[axis2.h_right.key] and !axis2.h_right.state)
  {
    axis2.horizontal += 1.0;
    axis2.h_right.state = true;
  }
  else if (!keyboard[axis2.h_right.key] and axis2.h_right.state)
  {
    axis2.horizontal -= 1.0;
    axis2.h_right.state = false;
  }

  // Actions
  if (event.type == SDL_KEYDOWN)
  {
    if (event.key.keysym.scancode == action1.key)
    {
      action1.state = true;
    }
    if (event.key.keysym.scancode == action2.key)
    {
      action2.state = true;
    }
    if (event.key.keysym.scancode == action3.key)
    {
      action3.state = true;
    }
    if (event.key.keysym.scancode == action4.key)
    {
      action4.state = true;
    }
    if (event.key.keysym.scancode == run.key)
    {
      run.state = true;
    }
    if (event.key.keysym.scancode == jump.key)
    {
      jump.state = true;
    }
    if (event.key.keysym.scancode == crouch.key)
    {
      crouch.state = true;
    }
  }
  else if (event.type == SDL_KEYUP)
  {
    if (event.key.keysym.scancode == action1.key)
    {
      action1.state = false;
    }
    if (event.key.keysym.scancode == action2.key)
    {
      action2.state = false;
    }
    if (event.key.keysym.scancode == action3.key)
    {
      action3.state = false;
    }
    if (event.key.keysym.scancode == action4.key)
    {
      action4.state = false;
    }
    if (event.key.keysym.scancode == run.key)
    {
      run.state = false;
    }
    if (event.key.keysym.scancode == jump.key)
    {
      jump.state = false;
    }
    if (event.key.keysym.scancode == crouch.key)
    {
      crouch.state = false;
    }
  }
}

void CSystem_UserInput::OnLoop()
{
  /*if(axis1.v_up.state)
  {
    gSystem_Debug.console_msg("Pulsando la tecla %s", gSystem_Data_Storage.GetString("__INPUT_AXIS1_UP_KEY").c_str());
  }
  if(axis1.v_down.state)
  {
    gSystem_Debug.console_msg("Pulsando la tecla %s", gSystem_Data_Storage.GetString("__INPUT_AXIS1_DOWN_KEY").c_str());
  }*/
}

Uint8 CSystem_UserInput::Keyboard(string keyname)
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
