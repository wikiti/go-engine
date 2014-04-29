#include "gameObjects.h"

bool SetGameObjects_Instance1()
{
  int test = 1200;
  Network::CBuffer buffer( (char*) (&test), sizeof(test) );
  std::cout << buffer.toInt() << std::endl;

  gNetwork.Send("pepito", "menganito");
  gNetwork.Send("fulanito", "menganito");
  gNetwork.Send("pepito", "moc");
  gNetwork.OnLoop();
  cout << gNetwork.Recv("pepito") << " -- " << gNetwork.Recv("fulanito") << endl;

  return Presentacion_Init();

  // Ignorado
  CGameObject* camara_main = gGameObjects["camara_main"];

  if(!camara_main)
  {
    camara_main = gGameObjects.Add("camara_main");
    camara_main->Transform()->position.y = -3.f;
    camara_main->Transform()->position.z = -10.f;
    camara_main->Camera()->far_clip = 500.f;
    camara_main->Preserve();
    camara_main->SetInputFunction(&Camara_main_movimiento);
    gRender.AddCamera(camara_main);
  }
  camara_main->Camera()->skybox_texture = "skybox1";


  CGameObject* next_instancer = gGameObjects["next_instancer"];
  if(!next_instancer)
  {
    next_instancer = gGameObjects.Add("next_instancer");
    next_instancer->Preserve();
    next_instancer->SetInputFunction(&Next_instancer_button);
  }

  SetGameObjects_Instance1_Fireworks();
  SetGameObjects_Instance1_Rainbow();
  SetGameObjects_Instance1_Render();
  //SetGameObjects_Instance1_Other_Particles();
  //SetGameObjects_Instance1_Shaders();
  //SetGameObjects_Instance1_RandomStuff();

  //gDebug.command("run script_scene1_setup", true);

  gEngine.SetNextInstance("level2");

  return true;
}

bool SetGameObjects_Instance2()
{
  CGameObject* camara_main = gGameObjects["camara_main"];
  camara_main->Camera()->skybox_texture = "skybox2";

  CGameObject* hada1 = gGameObjects.Add("hada1");

  hada1->Transform()->SetScale(0.1f, 0.1f, 0.1f);
  hada1->Transform()->Translate(0.f, 0.f, 10.f);
  hada1->Transform()->Rotate(0.f, 90.f, 0.f);

  hada1->MeshRender()->mesh_name = "mdl_hada1";
  hada1->MeshRender()->material_name = "texture_mdl_hada1";
  hada1->MeshRender()->color(1.0, 0.9f, 0.9f, 1.f);


  // Efecto sencillo de lluvia.
  CGameObject* lluvia = gGameObjects.Add("lluvia");
  lluvia->Transform()->Translate(0.f, 50.f, 0.f);
  lluvia->ParticleEmitter()->direction(0.2f, -1.f, 0.f);
  lluvia->ParticleEmitter()->angle_spread = 360.f;
  lluvia->ParticleEmitter()->max_particles = 6000;
  lluvia->ParticleEmitter()->particles_per_second = 500;
  lluvia->ParticleEmitter()->start_max_life_time = 20.f;
  lluvia->ParticleEmitter()->start_max_life_time = 10.f;
  lluvia->ParticleEmitter()->start_max_scale = 0.1f;
  lluvia->ParticleEmitter()->start_min_scale = 0.05f;
  lluvia->ParticleEmitter()->start_max_vel = lluvia->ParticleEmitter()->start_min_vel = 1.f;
  lluvia->ParticleEmitter()->start_max_base_radius = 50.f;
  lluvia->ParticleEmitter()->start_min_base_radius = 0.f;
  lluvia->ParticleEmitter()->material_name = "raindrop";
  lluvia->ParticleEmitter()->Start();

  gEngine.SetNextInstance("level1");

  return true;
}


void Camara_main_movimiento(CGameObject* gameObject)
{
  // lerp
  static float initial_fixed_rotation_time = 0.f;
  static bool train_move = false;

  if(initial_fixed_rotation_time != 0.f)
  {
    // Interpolate Y axis to angle "0"
    vector3f initialAngles = gameObject->Transform()->LRotation();
    vector3f finalAngle(0.f, 0.f, 0.f);

    float current_alpha = (gTime.GetTicks_s() - initial_fixed_rotation_time)/50.f; // 50 -> Arbitrary value
    gameObject->Transform()->SetAngle(gMath.lerpAngles(initialAngles, finalAngle, current_alpha));

    float epsilon = 1.f;
    if((gMath.abs(finalAngle.x - initialAngles.x) <= epsilon or gMath.abs(finalAngle.x - initialAngles.x) >= 360 - epsilon)
        and (gMath.abs(finalAngle.y - initialAngles.y) <= epsilon or gMath.abs(finalAngle.y - initialAngles.y) >= 360 - epsilon)
        and (gMath.abs(finalAngle.z - initialAngles.z) <= epsilon or gMath.abs(finalAngle.z - initialAngles.z) >= 360 - epsilon))
    {
      initial_fixed_rotation_time = 0.f;
      return;
    }

    return;
  }

  if(train_move)
  {
    vector3f initialPos = gameObject->Transform()->Position();
    vector3f finalPos(0.f, 0.f, 0.f);

    gameObject->Transform()->SetPosition(gMath.lerp(initialPos, finalPos, 0.05f));

    if(initialPos == finalPos)
      train_move = false;

    return;
  }

  if (gUserInput.Keyboard("T"))
  {
    initial_fixed_rotation_time = gTime.GetTicks_s();
  }

  if (gUserInput.Keyboard("Y"))
  {
    train_move = true;
  }


  float boost = 1.f;
  if (gUserInput.Keyboard("left shift"))
    boost = 3.f;

  gameObject->Transform()->LTranslate(gUserInput.axis1.horizontal * boost * -3.f * gTime.deltaTime_s(), 0.f, gUserInput.axis1.vertical * boost * 3.f * gTime.deltaTime_s());

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

  /*if (gUserInput.Keyboard("T"))
  {
    gameObject->Transform()->LookAt(vector3f(0.f, 0.f, 0.f));
  }*/

  Camara_mouse_movimiento(gameObject);
  Camara_Joystick_movimiento(gameObject);
}

void Camara_mouse_movimiento(CGameObject* gameObject)
{
  gameObject->Transform()->LTranslate(0.f, 0.f, gUserInput.mouse.wheel_y * 20.f * gTime.deltaTime_s());

  if(gUserInput.mouse.mouse1() == Input::key_keydown or gUserInput.mouse.mouse2() == Input::key_keydown  or gUserInput.mouse.mouse3() == Input::key_keydown)
    gUserInput.SetMouseTrap(true);
  else if(gUserInput.mouse.mouse1() == Input::key_keyup or gUserInput.mouse.mouse2() == Input::key_keyup or gUserInput.mouse.mouse3() == Input::key_keyup)
    gUserInput.SetMouseTrap(false);

  // Rotate
  if(gUserInput.mouse.mouse1() > 0)
  {
    vector3f rotation = gameObject->Transform()->LRotation();
    float x = gUserInput.mouse.y_vel * 20.f * gTime.deltaTime_s();
    if(rotation.x + x < 90 or rotation.x + x > 270)
      gameObject->Transform()->LRotate(x, 0, 0);

    gameObject->Transform()->Rotate(0, gUserInput.mouse.x_vel * -20.f * gTime.deltaTime_s(), 0);
  }
  // Screen Rotation
  else if(gUserInput.mouse.mouse2() > 0)
  {
    gameObject->Transform()->LRotate(0, 0, gUserInput.mouse.x_vel * 10.f * gTime.deltaTime_s());
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
    if(joys[0].buttons.size() > 8 and joys[0].buttons[8].State() == Input::button_pressed)
      boost = 3.f;

    // Translation
    if(joys[0].axes.size() >= 2)
    {
      if(joys[0].buttons.size() > 11 and joys[0].buttons[11].State() == Input::button_pressed)
        gameObject->Transform()->LTranslate(- joys[0].axes[0].value * boost * 3.f * gTime.deltaTime_s(), - joys[0].axes[1].value * boost * 3.f * gTime.deltaTime_s(), 0.f);
      else
        gameObject->Transform()->LTranslate(joys[0].axes[0].value * -3.f * boost * gTime.deltaTime_s(), 0.f, joys[0].axes[1].value * -3.f * boost * gTime.deltaTime_s());
    }
    // Rotation
    if(joys[0].axes.size() >= 4)
    {
      vector3f rotation = gameObject->Transform()->LRotation();
      float x = joys[0].axes[3].value * 60.f * gTime.deltaTime_s();
      if(rotation.x + x < 90 or rotation.x + x > 270)
        gameObject->Transform()->LRotate(x, 0, 0);

      gameObject->Transform()->Rotate(0, joys[0].axes[2].value * -60.f * gTime.deltaTime_s(), 0);
    }
    // More Translation
    if(joys[0].axes.size() >= 6)
    {
      gameObject->Transform()->LTranslate(0.f, 0.f, -(joys[0].axes[4].value + 1)/2 * boost * 3.f * gTime.deltaTime_s());
      gameObject->Transform()->LTranslate(0.f, 0.f,  (joys[0].axes[5].value + 1)/2 * boost * 3.f * gTime.deltaTime_s());
    }
  }
}


void Next_instancer_button(CGameObject* gameObject)
{
  if(gUserInput.Keyboard("K"))
    gEngine.EndInstance();
}
