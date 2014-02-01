#include "gameObjects.h"

bool SetGameObjects_Instance1()
{
  CGameObject* camara_main = gGameObjects.AddGameObject("camara_main");
  camara_main->Camera()->skybox_texture = "skybox_example";
  //camara_main->Camera()->background_color(1.f, 1.f, 1.f);
  camara_main->Transform()->position.y += 3.f;
  camara_main->Transform()->position.z -= 10.f;

  camara_main->SetKeyEventFunction(&Camara_main_movimiento);
  //camara_main->SetKeyEventFunction(&Camara_mouse_movimiento);

  gRender.AddCamera(camara_main);

  SetGameObjects_Instance1_Fireworks();
  SetGameObjects_Instance1_Rainbow();
  SetGameObjects_Instance1_Mesh();
  //SetGameObjects_Instance1_Other_Particles();
  //SetGameObjects_Instance1_Shaders();
  //SetGameObjects_Instance1_RandomStuff();

  return true;
}


void Camara_main_movimiento(CGameObject* gameObject)
{
  float boost = 1.f;
  if (gUserInput.Keyboard("left shift"))
    boost = 3.f;

  gameObject->Transform()->LTranslate(gUserInput.axis1.horizontal * boost * -3.f * gTime.deltaTime_s(), 0.f, gUserInput.axis1.vertical * boost * 3.f * gTime.deltaTime_s());
  //gameObject->Transform()->LTranslate(gUserInput.axis1.horizontal * boost * -3.f * gTime.deltaTime_s(), 0.f, 0.f);

  if (gUserInput.Keyboard("E"))
  {
    gameObject->Transform()->Translate(0.f, boost * -3.f * gTime.deltaTime_s(), 0.f);
  }
  if (gUserInput.Keyboard("Q"))
  {
    gameObject->Transform()->Translate(0.f, boost * 3.f * gTime.deltaTime_s(), 0.f);
  }

  // Viewport
  if(gUserInput.axis2.vertical > 0)
  {
    gameObject->Camera()->viewport.height += 0.1f * gTime.deltaTime_s();
    if(gameObject->Camera()->viewport.height > 1.f)
      gameObject->Camera()->viewport.height = 1.f;
  }
  else if(gUserInput.axis2.vertical < 0)
  {
    gameObject->Camera()->viewport.height -= 0.1f * gTime.deltaTime_s();
    if(gameObject->Camera()->viewport.height < 0.f)
      gameObject->Camera()->viewport.height = 0.f;
  }
  if(gUserInput.axis2.horizontal < 0)
  {
    gameObject->Camera()->viewport.width -= 0.1f * gTime.deltaTime_s();
    if(gameObject->Camera()->viewport.width < 0.f)
      gameObject->Camera()->viewport.width = 0.f;
  }
  else if(gUserInput.axis2.horizontal > 0)
  {
    gameObject->Camera()->viewport.width += 0.1f * gTime.deltaTime_s();
    if(gameObject->Camera()->viewport.width > 1.f)
      gameObject->Camera()->viewport.width = 1.f;
  }

  if (gUserInput.Keyboard("T"))
  {
    gameObject->Transform()->LookAt(vector3f(0.f, 0.f, 0.f));
  }

  Camara_mouse_movimiento(gameObject);
  Camara_Joystick_movimiento(gameObject);
}

void Camara_mouse_movimiento(CGameObject* gameObject)
{
  gameObject->Transform()->LTranslate(0.f, 0.f, gUserInput.mouse.wheel_y * 20.f * gTime.deltaTime_s());

  if(gUserInput.mouse.mouse1() == GO_Input::key_keydown or gUserInput.mouse.mouse3() == GO_Input::key_keydown)
    gUserInput.SetMouseTrap(true);
  else if(gUserInput.mouse.mouse1() == GO_Input::key_keyup or gUserInput.mouse.mouse3() == GO_Input::key_keyup)
    gUserInput.SetMouseTrap(false);

  // Rotate
  if(gUserInput.mouse.mouse1() > 0)
  {
    gameObject->Transform()->LRotate(gUserInput.mouse.y_vel * 20.f * gTime.deltaTime_s(), 0, 0);
    gameObject->Transform()->Rotate(0, gUserInput.mouse.x_vel * -20.f * gTime.deltaTime_s(), 0);
  }
  // Pan
  else if(gUserInput.mouse.mouse3() > 0)
  {
    gameObject->Transform()->LTranslate(3.f * gUserInput.mouse.x_vel * gTime.deltaTime_s(), 3.f * gUserInput.mouse.y_vel * gTime.deltaTime_s(), 0.f);
  }
}

void Camara_Joystick_movimiento(CGameObject* gameObject)
{
  vector<GO_InputClasses::CJoystick> joys = gUserInput.GetJoysticks();

  if(joys.size() > 0)
  {
    float boost = 1.f;
    if(joys[0].buttons.size() > 11 and joys[0].buttons[10].State() == GO_Input::button_pressed)
      boost = 3.f;

    if(joys[0].axes.size() >= 2)
    {
      if(joys[0].buttons.size() > 12 and joys[0].buttons[11].State() == GO_Input::button_pressed)
        gameObject->Transform()->LTranslate(- joys[0].axes[0].value * boost * 3.f * gTime.deltaTime_s(), - joys[0].axes[1].value * boost * 3.f * gTime.deltaTime_s(), 0.f);
      else
        gameObject->Transform()->LTranslate(joys[0].axes[0].value * -3.f * boost * gTime.deltaTime_s(), 0.f, joys[0].axes[1].value * -3.f * boost * gTime.deltaTime_s());
    }
    if(joys[0].axes.size() >= 4)
    {
      gameObject->Transform()->LRotate(joys[0].axes[3].value * 60.f * gTime.deltaTime_s(), 0, 0);
      gameObject->Transform()->Rotate(0, joys[0].axes[2].value * -60.f * gTime.deltaTime_s(), 0);
    }
    if(joys[0].axes.size() >= 6)
    {
      gameObject->Transform()->LTranslate(0.f, 0.f, -(joys[0].axes[4].value + 1)/2 * boost * 3.f * gTime.deltaTime_s());
      gameObject->Transform()->LTranslate(0.f, 0.f,  (joys[0].axes[5].value + 1)/2 * boost * 3.f * gTime.deltaTime_s());
      //gDebug.console_msg("%f", joys[0].axes[4].value);
    }

  }
}
