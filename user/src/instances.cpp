#include "instances.h"
#include "gameObjects.h"

#include "engine/_engine.h"

bool User_Engine_Loader()
{
  gEngine.SetIcon("data/engine/icon.tga");
  gEngine.SetTitle("GO-Engine Test");

  return true;
}

bool User_Instance_Loader()
{
  gEngine.AddInstance(&SetGameObjects_Instance1, "data/resources/test1.rc", "level1");
  gEngine.AddInstance(&SetGameObjects_Instance2, "data/resources/test2.rc", "level2");

  return true;
}
