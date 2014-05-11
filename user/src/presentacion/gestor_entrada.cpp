/*
 * gestor_entrada.cpp
 *
 *  Created on: 25/04/2014
 *      Author: Dani
 */

#include "presentacion/gestor_entrada.h"
#include "presentacion/general.h"

vector3f rotate_to(0, 0, 0);
bool toggle_forward = false;
bool toggle_backward = false;

uint backward_button = 13;
uint forward_button = 10;

void Gestor_Entrada_Input(CGameObject* gameObject)
{
  vector<InputClasses::CJoystick> joys = gUserInput.GetJoysticks();

  if(joys.size() > 0)
  {
    if(!toggle_forward and joys[0].buttons.size() > forward_button and joys[0].buttons[forward_button].State() == Input::button_pressed)
    {
      diapositiva_actual++;
      toggle_forward = true;

      // Desactivar pre-anterior y activar posterior posterior
      if(diapositiva_actual - 2 >= 0)
      {
        diapositivas_funcs_close[diapositiva_actual - 2]();
      }
      if(diapositiva_actual + 1 < NUM_DIAPOSITIVAS)
      {
        diapositivas_funcs_init[diapositiva_actual + 1]();
      }
      if(diapositiva_actual == NUM_DIAPOSITIVAS)
        gEngine.Exit();

      // Rotar la cámara
      rotate_to.y -= 90;
      if(diapositiva_actual == NUM_DIAPOSITIVAS - 1)
      {
        gGameObjects["firework_manager"]->Enable();
        rotate_to.x -= 45;
      }

      rotate_to = gMath.NormalizeAngles(rotate_to);
    }
    else if(toggle_forward and joys[0].buttons.size() > forward_button and joys[0].buttons[forward_button].State() == Input::button_unpressed)
    {
      toggle_forward = false;
    }

    // Retroceder
    if(!toggle_backward and diapositiva_actual > 0 and joys[0].buttons.size() > backward_button and joys[0].buttons[backward_button].State() == Input::button_pressed)
    {
      diapositiva_actual--;

      toggle_backward = true;

      // Desactivar proposterior y activar anterior
      if(diapositiva_actual + 2 < NUM_DIAPOSITIVAS)
      {
        diapositivas_funcs_close[diapositiva_actual + 2]();
      }
      if(diapositiva_actual - 1 >= 0)
      {
        diapositivas_funcs_init[diapositiva_actual - 1]();
      }

      // Rotar la cámara
      rotate_to.y += 90;
      if(diapositiva_actual == NUM_DIAPOSITIVAS - 2)
      {
        gGameObjects["firework_manager"]->Disable(); // ¿?
        rotate_to.x += 45;
      }

      rotate_to = gMath.NormalizeAngles(rotate_to);
    }
    else if(toggle_backward and joys[0].buttons.size() > backward_button and joys[0].buttons[backward_button].State() == Input::button_unpressed)
    {
      toggle_backward = false;
    }
  }

}
