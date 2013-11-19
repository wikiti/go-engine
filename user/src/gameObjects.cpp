#include "gameObjects.h"
#include "_components.h"
#include "_systems.h"
#include "engine/_engine.h"

bool SetGameObjects_Instance1()
{
  CGameObject* camara_main = new CGameObject("camara_main");
  gGameObjects.AddGameObject(camara_main);
  CGameObject* camara_second = gGameObjects.AddGameObject("camara_second");
  CGameObject* cubo_main = gGameObjects.AddGameObject("cubo_main");
  CGameObject* cubo_second = gGameObjects.AddGameObject("cubo_second");
  CGameObject* cubo_third = gGameObjects.AddGameObject("cubo_third");
  CGameObject* hada = gGameObjects.AddGameObject("hada");

  cubo_main->AddChild(cubo_second);
  cubo_second->AddChild(cubo_third);

  camara_main->transform()->position.y += 3.f;
  camara_second->transform()->position.y += 3.f;
  cubo_second->transform()->Translate(1.f, 0.f, 0.f);
  cubo_third->transform()->Translate(0.f, 1.f, 0.f);
  cubo_main->transform()->Translate(0.f, 0.f, 5.f);

  hada->transform()->SetScale(0.1f, 0.1f, 0.1f);
  hada->transform()->Translate(0.f, 0.f, 10.f);
  hada->transform()->Rotate(0.f, 90.f, 0.f);


  camara_second->camera()->SetTarget(cubo_third);
  camara_second->camera()->viewport.height = camara_second->camera()->viewport.width = 0.3f;
  camara_second->camera()->background_color(0, 0.5f, 0.75f, 1.f);

  cubo_main->SetKeyEventBehaviourFunction(&Cubo_main_movimiento);
  camara_main->SetKeyEventBehaviourFunction(&Camara_main_movimiento);
  camara_second->SetEventBehaviourFunction(&Camara_second_movimiento);
  cubo_second->SetKeyEventBehaviourFunction(&Cubo_second_movimiento);
  hada->SetBehaviourFunction(&Hada_movimiento);
  DefineTramsformByVar("GO_HADA");

  gRender.AddCamera(camara_main);
  gSystem_Render.AddCamera(camara_second);

  cubo_main->AddComponent<CComponent_Dummy1>();
  cubo_second->AddComponent<CComponent_Dummy1>();
  cubo_third->AddComponent<CComponent_Dummy1>();
  //hada->AddComponent<CComponent_Dummy2>();

  hada->meshRender()->mesh_name = "mdl_hada1";
  hada->meshRender()->material_name = "texture_mdl_hada1";

  camara_second->camera()->before_render = &Camara_second_before_render;
  camara_second->camera()->after_render = &Camara_second_after_render;

  return true;
}

void Camara_second_before_render(CGameObject* gameObject)
{
  gGameObjects["hada"]->meshRender()->material_name = "textura1";
}

void Camara_second_after_render(CGameObject* gameObject)
{
  gGameObjects["hada"]->meshRender()->material_name = "texture_mdl_hada1";
}

void Cubo_main_movimiento(CGameObject* gameObject)
{
  if(gKeyboardState[SDL_SCANCODE_LCTRL])
  {
    if(gKeyboardState[SDL_SCANCODE_W])
    {
      gameObject->transform()->Rotate(20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_S])
    {
      gameObject->transform()->Rotate(-20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_D])
    {
      gameObject->transform()->Rotate(0.f, 20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_A])
    {
      gameObject->transform()->Rotate(0.f, -20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_Q])
    {
      gameObject->transform()->Rotate(0.f, 0.f, -20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_E])
    {
      gameObject->transform()->Rotate(0.f, 0.f, 20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_R])
    {
      gameObject->transform()->SetAngle(0, 0, 0);
    }
    if(gKeyboardState[SDL_SCANCODE_1])
    {
      gameObject->transform()->SetAngle(45, 0, 0);
    }
    if(gKeyboardState[SDL_SCANCODE_2])
    {
      gameObject->transform()->SetAngle(0, 45, 0);
    }
    if(gKeyboardState[SDL_SCANCODE_3])
    {
      gameObject->transform()->SetAngle(0, 0, 45);
    }
  }
  else if(gKeyboardState[SDL_SCANCODE_LALT])
  {
    if(gKeyboardState[SDL_SCANCODE_W])
    {
      gameObject->transform()->LRotate(20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_S])
    {
      gameObject->transform()->LRotate(-20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_D])
    {
      gameObject->transform()->LRotate(0.f, 20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_A])
    {
      gameObject->transform()->LRotate(0.f, -20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_Q])
    {
      gameObject->transform()->LRotate(0.f, 0.f, -20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_E])
    {
      gameObject->transform()->LRotate(0.f, 0.f, 20.f * gTime.deltaTime_s());
    }
  }
  else if(gKeyboardState[SDL_SCANCODE_LSHIFT])
  {
    if(gKeyboardState[SDL_SCANCODE_W])
    {
      gameObject->transform()->LTranslate(0.f, 0.f, 5.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_S])
    {
      gameObject->transform()->LTranslate(0.f, 0.f, -5.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_D])
    {
      gameObject->transform()->LTranslate(5.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_A])
    {
      gameObject->transform()->LTranslate(-5.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_Q])
    {
      gameObject->transform()->LTranslate(0.f, 5.f * gTime.deltaTime_s(), 0.f);
    }
    if(gKeyboardState[SDL_SCANCODE_E])
    {
      gameObject->transform()->LTranslate(0.f, -5.f * gTime.deltaTime_s(), 0.f);
    }
  }
  else
  {
    if(gKeyboardState[SDL_SCANCODE_W])
    {
      gameObject->transform()->Translate(0.f, 0.f, 5.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_S])
    {
      gameObject->transform()->Translate(0.f, 0.f, -5.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_D])
    {
      gameObject->transform()->Translate(5.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_A])
    {
      gameObject->transform()->Translate(-5.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_Q])
    {
      gameObject->transform()->Translate(0.f, 5.f * gTime.deltaTime_s(), 0.f);
    }
    if(gKeyboardState[SDL_SCANCODE_E])
    {
      gameObject->transform()->Translate(0.f, -5.f * gTime.deltaTime_s(), 0.f);
    }
  }
}

void Camara_main_movimiento(CGameObject* gameObject)
{
  if(gKeyboardState[SDL_SCANCODE_LCTRL])
  {
    if(gKeyboardState[SDL_SCANCODE_UP])
    {
      gameObject->transform()->LRotate(-20.f * gTime.deltaTime_s(), 0, 0);
    }
    if(gKeyboardState[SDL_SCANCODE_DOWN])
    {
      gameObject->transform()->LRotate(20.f * gTime.deltaTime_s(), 0, 0);
    }
    if(gKeyboardState[SDL_SCANCODE_LEFT])
    {
      gameObject->transform()->LRotate(0, +20.f * gTime.deltaTime_s(), 0);
    }
    if(gKeyboardState[SDL_SCANCODE_RIGHT])
    {
      gameObject->transform()->LRotate(0, -20.f * gTime.deltaTime_s(), 0);
    }
    if(gKeyboardState[SDL_SCANCODE_PAGEDOWN])
    {
      gameObject->transform()->LRotate(0, 0, 20.f * gTime.deltaTime_s());
    }
    if(gKeyboardState[SDL_SCANCODE_PAGEUP])
    {
      gameObject->transform()->LRotate(0, 0, -20.f * gTime.deltaTime_s());
    }
  }
  else
  {
    if(gKeyboardState[SDL_SCANCODE_UP])
    {
      gameObject->transform()->LTranslate(0.f, 0.f, 3.f * gTime.deltaTime_s());
      //gameObject->transform()->position.z += 1.f * gTime.deltaTime_s();
    }
    if(gKeyboardState[SDL_SCANCODE_DOWN])
    {
      gameObject->transform()->LTranslate(0.f, 0.f, -3.f * gTime.deltaTime_s());
      //gameObject->transform()->position.z -= 1.f * gTime.deltaTime_s();
    }
    if(gKeyboardState[SDL_SCANCODE_LEFT])
    {
      gameObject->transform()->LTranslate(3.f * gTime.deltaTime_s(), 0.f, 0.f);
      //gameObject->transform()->position.x -= 1.f * gTime.deltaTime_s();
    }
    if(gKeyboardState[SDL_SCANCODE_RIGHT])
    {
      gameObject->transform()->LTranslate(-3.f * gTime.deltaTime_s(), 0.f, 0.f);
      //gameObject->transform()->position.x += 1.f * gTime.deltaTime_s();
    }
    if(gKeyboardState[SDL_SCANCODE_PAGEDOWN])
    {
      gameObject->transform()->LTranslate(0.f, -3.f * gTime.deltaTime_s(), 0.f);
      //gameObject->transform()->position.y -= 1.f * gTime.deltaTime_s();
    }
    if(gKeyboardState[SDL_SCANCODE_PAGEUP])
    {
      gameObject->transform()->LTranslate(0.f, 3.f * gTime.deltaTime_s(), 0.f);
      //gameObject->transform()->position.y += 1.f * gTime.deltaTime_s();
    }
  }

  // Viewport
  if(gKeyboardState[SDL_SCANCODE_I])
  {
    gameObject->camera()->viewport.height += 0.1f * gTime.deltaTime_s();
    if(gameObject->camera()->viewport.height > 1.f)
      gameObject->camera()->viewport.height = 1.f;
  }
  if(gKeyboardState[SDL_SCANCODE_K])
  {
    gameObject->camera()->viewport.height -= 0.1f * gTime.deltaTime_s();
    if(gameObject->camera()->viewport.height < 0.f)
      gameObject->camera()->viewport.height = 0.f;
  }
  if(gKeyboardState[SDL_SCANCODE_J])
  {
    gameObject->camera()->viewport.width -= 0.1f * gTime.deltaTime_s();
    if(gameObject->camera()->viewport.width < 0.f)
      gameObject->camera()->viewport.width = 0.f;
  }
  if(gKeyboardState[SDL_SCANCODE_L])
  {
    gameObject->camera()->viewport.width += 0.1f * gTime.deltaTime_s();
    if(gameObject->camera()->viewport.width > 1.f)
      gameObject->camera()->viewport.width = 1.f;
  }
}

void Cubo_second_movimiento(CGameObject* gameObject)
{
  static float scale = 1.f;
  // Viewport
  if(gKeyboardState[SDL_SCANCODE_Y])
  {
    scale += 1.f * gTime.deltaTime_s();
    gameObject->transform()->SetScale(1.f, scale, 1.f);
  }
  if(gKeyboardState[SDL_SCANCODE_H])
  {
    scale -= 1.f * gTime.deltaTime_s();
    gameObject->transform()->SetScale(1.f, scale, 1.f);
  }
}

void Cubo_second_cout(CGameObject* gameObject)
{
  cout << "Cubo_second GPOS: " << gameObject->transform()->Position() << endl;
  cout << "Cubo_second LPOS: " << gameObject->transform()->LPosition() << endl;
  cout << "----------------------------------------------------" << endl;
}

void Camara_second_movimiento(CGameObject* gameObject)
{
  static bool target = false;
  if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
  {
    if(!target)
      gameObject->camera()->SetTarget(gGameObjects["cubo_second"]);
    else
      gameObject->camera()->UnSetTarget();

    target = !target;
  }
}

void Hada_movimiento(CGameObject* gameObject)
{
  SetTransformByVar(gameObject, "GO_HADA");
}
