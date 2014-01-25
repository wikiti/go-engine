#include "systems/_input.h"
#include "systems/_data.h"
#include "systems/_debug.h"

CSystem_UserInput gSystem_UserInput;
CSystem_UserInput& gUserInput = gSystem_UserInput;

//const Uint8* CSystem_UserInput::keyboard = SDL_GetKeyboardState(NULL);

using namespace GO_Keystates;

bool CSystem_UserInput::Init()
{
  // Axis
  axis1.horizontal = axis2.horizontal = 0;
  axis1.vertical = axis2.vertical = 0;

  axis1.v_up.state = axis1.v_down.state = axis1.h_left.state = axis1.h_right.state = unpressed;
  axis2.v_up.state = axis2.v_down.state = axis2.h_left.state = axis2.h_right.state = unpressed;

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
  action1.state = action2.state = action3.state = action4.state = unpressed;
  action1.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_ACTION1_KEY").c_str());
  action2.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_ACTION2_KEY").c_str());
  action3.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_ACTION3_KEY").c_str());
  action4.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_ACTION4_KEY").c_str());

  run.state = crouch.state = jump.state = unpressed;
  run.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_RUN_KEY").c_str());
  crouch.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_CROUCH_KEY").c_str());
  jump.key = SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_JUMP_KEY").c_str());

  return true;
}

void CSystem_UserInput::Close()
{

}

void CSystem_UserInput::OnKeyEvent()
{
  // ---- Para los ejes, usamos máquinas de estado para alternar entre los estados unpressed, keydown, pressed y keyup.
  // -- Axis1
  // - Up
  if((axis1.v_up.state == keyup or axis1.v_up.state == unpressed) and keyboard[axis1.v_up.key])
  {
    axis1.vertical += 1.0;
    axis1.v_up.state = keydown;
  }
  else if((axis1.v_up.state == keydown or axis1.v_up.state == pressed) and !keyboard[axis1.v_up.key])
  {
    axis1.vertical -= 1.0;
    axis1.v_up.state = keyup;
  }
  else if(axis1.v_up.state == keydown and keyboard[axis1.v_up.key])
    axis1.v_up.state = pressed;
  else if(axis1.v_up.state == keyup and !keyboard[axis1.v_up.key])
    axis1.v_up.state = unpressed;

  // - Down
  if((axis1.v_down.state == keyup or axis1.v_down.state == unpressed) and keyboard[axis1.v_down.key])
  {
    axis1.vertical -= 1.0;
    axis1.v_down.state = keydown;
  }
  else if((axis1.v_down.state == keydown or axis1.v_down.state == pressed) and !keyboard[axis1.v_down.key])
  {
    axis1.vertical += 1.0;
    axis1.v_down.state = keyup;
  }
  else if(axis1.v_down.state == keydown and keyboard[axis1.v_down.key])
    axis1.v_down.state = pressed;
  else if(axis1.v_down.state == keyup and !keyboard[axis1.v_down.key])
    axis1.v_down.state = unpressed;

  // - Left
  if((axis1.h_left.state == keyup or axis1.h_left.state == unpressed) and keyboard[axis1.h_left.key])
  {
    axis1.horizontal -= 1.0;
    axis1.h_left.state = keydown;
  }
  else if((axis1.h_left.state == keydown or axis1.h_left.state == pressed) and !keyboard[axis1.h_left.key])
  {
    axis1.horizontal += 1.0;
    axis1.h_left.state = keyup;
  }
  else if(axis1.h_left.state == keydown and keyboard[axis1.h_left.key])
    axis1.h_left.state = pressed;
  else if(axis1.h_left.state == keyup and !keyboard[axis1.h_left.key])
    axis1.h_left.state = unpressed;

  // - Right
  if((axis1.h_right.state == keyup or axis1.h_right.state == unpressed) and keyboard[axis1.h_right.key])
  {
    axis1.horizontal += 1.0;
    axis1.h_right.state = keydown;
  }
  else if((axis1.h_right.state == keydown or axis1.h_right.state == pressed) and !keyboard[axis1.h_right.key])
  {
    axis1.horizontal -= 1.0;
    axis1.h_right.state = keyup;
  }
  else if(axis1.h_right.state == keydown and keyboard[axis1.h_right.key])
    axis1.h_right.state = pressed;
  else if(axis1.h_right.state == keyup and !keyboard[axis1.h_right.key])
    axis1.h_right.state = unpressed;


  // -- Axis2
  // - Up
  if((axis2.v_up.state == keyup or axis2.v_up.state == unpressed) and keyboard[axis2.v_up.key])
  {
    axis2.vertical += 1.0;
    axis2.v_up.state = keydown;
  }
  else if((axis2.v_up.state == keydown or axis2.v_up.state == pressed) and !keyboard[axis2.v_up.key])
  {
    axis2.vertical -= 1.0;
    axis2.v_up.state = keyup;
  }
  else if(axis2.v_up.state == keydown and keyboard[axis2.v_up.key])
    axis2.v_up.state = pressed;
  else if(axis2.v_up.state == keyup and !keyboard[axis2.v_up.key])
    axis2.v_up.state = unpressed;

  // - Down
  if((axis2.v_down.state == keyup or axis2.v_down.state == unpressed) and keyboard[axis2.v_down.key])
  {
    axis2.vertical -= 1.0;
    axis2.v_down.state = keydown;
  }
  else if((axis2.v_down.state == keydown or axis2.v_down.state == pressed) and !keyboard[axis2.v_down.key])
  {
    axis2.vertical += 1.0;
    axis2.v_down.state = keyup;
  }
  else if(axis2.v_down.state == keydown and keyboard[axis2.v_down.key])
    axis2.v_down.state = pressed;
  else if(axis2.v_down.state == keyup and !keyboard[axis2.v_down.key])
    axis2.v_down.state = unpressed;

  // - Left
  if((axis2.h_left.state == keyup or axis2.h_left.state == unpressed) and keyboard[axis2.h_left.key])
  {
    axis2.horizontal -= 1.0;
    axis2.h_left.state = keydown;
  }
  else if((axis2.h_left.state == keydown or axis2.h_left.state == pressed) and !keyboard[axis2.h_left.key])
  {
    axis2.horizontal += 1.0;
    axis2.h_left.state = keyup;
  }
  else if(axis2.h_left.state == keydown and keyboard[axis2.h_left.key])
    axis2.h_left.state = pressed;
  else if(axis2.h_left.state == keyup and !keyboard[axis2.h_left.key])
    axis2.h_left.state = unpressed;

  // - Right
  if((axis2.h_right.state == keyup or axis2.h_right.state == unpressed) and keyboard[axis2.h_right.key])
  {
    axis2.horizontal += 1.0;
    axis2.h_right.state = keydown;
  }
  else if((axis2.h_right.state == keydown or axis2.h_right.state == pressed) and !keyboard[axis2.h_right.key])
  {
    axis2.horizontal -= 1.0;
    axis2.h_right.state = keyup;
  }
  else if(axis2.h_right.state == keydown and keyboard[axis2.h_right.key])
    axis2.h_right.state = pressed;
  else if(axis2.h_right.state == keyup and !keyboard[axis2.h_right.key])
    axis2.h_right.state = unpressed;


  // -- Actions
  // - Action1
  if((action1.state == keyup or action1.state == unpressed) and keyboard[action1.key])
    action1.state = keydown;
  else if((action1.state == keydown or action1.state == pressed) and !keyboard[action1.key])
    action1.state = keyup;
  else if(action1.state == keydown and keyboard[action1.key])
    action1.state = pressed;
  else if(action1.state == keyup and !keyboard[action1.key])
    action1.state = unpressed;

  // - Action2
  if((action2.state == keyup or action2.state == unpressed) and keyboard[action2.key])
    action2.state = keydown;
  else if((action2.state == keydown or action2.state == pressed) and !keyboard[action2.key])
    action2.state = keyup;
  else if(action2.state == keydown and keyboard[action2.key])
    action2.state = pressed;
  else if(action2.state == keyup and !keyboard[action2.key])
    action2.state = unpressed;

  // - Action3
  if((action3.state == keyup or action3.state == unpressed) and keyboard[action3.key])
    action3.state = keydown;
  else if((action3.state == keydown or action3.state == pressed) and !keyboard[action3.key])
    action3.state = keyup;
  else if(action3.state == keydown and keyboard[action3.key])
    action3.state = pressed;
  else if(action3.state == keyup and !keyboard[action3.key])
    action3.state = unpressed;

  // - Action4
  if((action4.state == keyup or action4.state == unpressed) and keyboard[action4.key])
    action4.state = keydown;
  else if((action4.state == keydown or action4.state == pressed) and !keyboard[action4.key])
    action4.state = keyup;
  else if(action4.state == keydown and keyboard[action4.key])
    action4.state = pressed;
  else if(action4.state == keyup and !keyboard[action4.key])
    action4.state = unpressed;

  // - Run
  if((run.state == keyup or run.state == unpressed) and keyboard[run.key])
    run.state = keydown;
  else if((run.state == keydown or run.state == pressed) and !keyboard[run.key])
    run.state = keyup;
  else if(run.state == keydown and keyboard[run.key])
    run.state = pressed;
  else if(run.state == keyup and !keyboard[run.key])
    run.state = unpressed;

  // - Crouch
  if((crouch.state == keyup or crouch.state == unpressed) and keyboard[crouch.key])
    crouch.state = keydown;
  else if((crouch.state == keydown or crouch.state == pressed) and !keyboard[crouch.key])
    crouch.state = keyup;
  else if(crouch.state == keydown and keyboard[crouch.key])
    crouch.state = pressed;
  else if(crouch.state == keyup and !keyboard[crouch.key])
    crouch.state = unpressed;

  // - Jump
  if((jump.state == keyup or jump.state == unpressed) and keyboard[jump.key])
    jump.state = keydown;
  else if((jump.state == keydown or jump.state == pressed) and !keyboard[jump.key])
    jump.state = keyup;
  else if(jump.state == keydown and keyboard[jump.key])
    jump.state = pressed;
  else if(jump.state == keyup and !keyboard[jump.key])
    jump.state = unpressed;

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
