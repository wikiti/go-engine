#include "_globals.h"
#include "_systems.h"
#include "_components.h"
#include "engine/_engine.h"

int main(int argc, char* argv[])
{
  try
  {
    //THROW(general_exception, "Error de loquesea.");
    return gEngine.OnExecute();
  }
  catch(exception& e)
  {
    gSystem_Debug.error("Exception ocurred: %s", e.what());
  }

  return 0;
}

/*int main_save();
int main_load();

int main_(int argc, char* argv[])
{
  Systems_Init();

  gSystem_Debug.log("From main(): prueba de log con n %i", 2);
  gSystem_Debug.error("From main(): prueba de error con string %s", "padas");
  gSystem_Debug.raw_log("From main(): wololo %f", 42.f);
  gSystem_Debug.raw_log(" From main(): wololo %f\n", 3.f);

  gSystem_Debug.msg_box("File error", "Could not load tuputamadre");
  //main_load();

  gSystem_Resources.LoadResourceFile("test1.rc");
  gSystem_Resources.LoadResourceFile("test2.rc");

  Systems_Close();

  return 0;
}


int main_load()
{
  gSystem_GameObject_Manager.LoadGameObjects("save_go.dat");

  CGameObject* GO1 = gSystem_GameObject_Manager["GO1"];
  CGameObject* GO2 = gSystem_GameObject_Manager["GO2"];
  CGameObject* GO3 = gSystem_GameObject_Manager["GO3"];
  CGameObject* GO4 = gSystem_GameObject_Manager["GO4"];

  // Comprobar funcionalidad de los padres e hijos

  //GO4->AddChild(GO2); // falla y no hace nada, bien
  //GO4->AddChild(GO4); // falla y no hace nada, bien

  cout << "Padre de GO1: " << (GO1->GetParent() == NULL? "NULL": GO1->GetParent()->GetName()) << endl;
  cout << "Padre de GO2: " << (GO2->GetParent() == NULL? "NULL": GO2->GetParent()->GetName()) << endl;
  cout << "Padre de GO3: " << (GO3->GetParent() == NULL? "NULL": GO3->GetParent()->GetName()) << endl;
  cout << "Padre de GO4: " << (GO4->GetParent() == NULL? "NULL": GO4->GetParent()->GetName()) << endl;

  // Comprobar posiciones

  CComponent_Transform* datos_GO1 = GO1->GetComponent<CComponent_Transform>();
  CComponent_Transform* datos_GO2 = GO2->GetComponent<CComponent_Transform>();
  CComponent_Transform* datos_GO3 = GO3->GetComponent<CComponent_Transform>();
  CComponent_Transform* datos_GO4 = GO4->GetComponent<CComponent_Transform>();

  GO1->GetComponent<CComponent_Transform>()->position.x;

  cout << "POS GO1: " << datos_GO1->position.x << " " << datos_GO1->position.y << " " << datos_GO1->position.z << endl;
  cout << "POS GO2: " << datos_GO2->position.x << " " << datos_GO2->position.y << " " << datos_GO2->position.z << endl;
  cout << "POS GO3: " << datos_GO3->position.x << " " << datos_GO3->position.y << " " << datos_GO3->position.z << endl;
  cout << "POS GO4: " << datos_GO4->position.x << " " << datos_GO4->position.y << " " << datos_GO4->position.z << endl;
  cout << "ROT GO1: " << datos_GO1->angle.x << " " << datos_GO1->angle.y << " " << datos_GO1->angle.z << endl;
  cout << "ROT GO2: " << datos_GO2->angle.x << " " << datos_GO2->angle.y << " " << datos_GO2->angle.z << endl;
  cout << "ROT GO3: " << datos_GO3->angle.x << " " << datos_GO3->angle.y << " " << datos_GO3->angle.z << endl;
  cout << "ROT GO4: " << datos_GO4->angle.x << " " << datos_GO4->angle.y << " " << datos_GO4->angle.z << endl;

  // llamadas a funciones

  vector3f translate_data(0.5f, 0.32f, -5.f);
  vector3f rotate_data(-5.f, -3.9f, -5.12f);
  GO2->GetComponent<CComponent_Transform>()->Translate(translate_data);
  GO1->GetComponent<CComponent_Transform>()->Translate(translate_data);
  GO3->GetComponent<CComponent_Transform>()->Translate(translate_data);
  GO1->GetComponent<CComponent_Transform>()->Rotate(rotate_data);
  GO2->GetComponent<CComponent_Transform>()->Rotate(rotate_data);
//  GO1->SendMessage(GO2, "helloworld", NULL);
//  GO1->SendMessage(GO2, "translate", &translate_data);//&translate_data);
//  GO2->SendMessage(GO1, "translate", &translate_data);
//  GO2->SendMessage(GO3, "translate", &translate_data, NULL);
//  GO4->SendMessage("GO4", "helloworld", NULL);
//  GO1->CallFunc("rotate", &rotate_data);
  //GO2->CallFunc("rotate", &rotate_data);

  cout << "POS GO1: " << datos_GO1->position.x << " " << datos_GO1->position.y << " " << datos_GO1->position.z << endl;
  cout << "POS GO2: " << datos_GO2->position.x << " " << datos_GO2->position.y << " " << datos_GO2->position.z << endl;
  cout << "POS GO3: " << datos_GO3->position.x << " " << datos_GO3->position.y << " " << datos_GO3->position.z << endl;
  cout << "POS GO4: " << datos_GO4->position.x << " " << datos_GO4->position.y << " " << datos_GO4->position.z << endl;
  cout << "ROT GO1: " << datos_GO1->angle.x << " " << datos_GO1->angle.y << " " << datos_GO1->angle.z << endl;
  cout << "ROT GO2: " << datos_GO2->angle.x << " " << datos_GO2->angle.y << " " << datos_GO2->angle.z << endl;
  cout << "ROT GO3: " << datos_GO3->angle.x << " " << datos_GO3->angle.y << " " << datos_GO3->angle.z << endl;
  cout << "ROT GO4: " << datos_GO4->angle.x << " " << datos_GO4->angle.y << " " << datos_GO4->angle.z << endl;

  //gSystem_GameObject_Manager.RemoveGameObject(GO2);

  cout << "Padre de GO1: " << (GO1->GetParent() == NULL? "NULL": GO1->GetParent()->GetName()) << endl;
  cout << "Padre de GO2: " << (GO2->GetParent() == NULL? "NULL": GO2->GetParent()->GetName()) << endl;
  cout << "Padre de GO3: " << (GO3->GetParent() == NULL? "NULL": GO3->GetParent()->GetName()) << endl;
  cout << "Padre de GO4: " << (GO4->GetParent() == NULL? "NULL": GO4->GetParent()->GetName()) << endl;

  gSystem_GameObject_Manager.SaveGameObjects("save_go.dat");

  return 0;
}


int main_save()
{
  gSystem_GameObject_Manager.Init();

  cout << "ID GO1:" << gSystem_GameObject_Manager.AddGameObject("GO1") << endl;
  cout << "ID GO2:" << gSystem_GameObject_Manager.AddGameObject("GO2") << endl;
  cout << "ID GO3:" << gSystem_GameObject_Manager.AddGameObject("GO3") << endl;

  CGameObject* GO4 = new CGameObject("GO4");
  gSystem_GameObject_Manager.AddGameObject(GO4);
  cout << "ID GO4:" << gSystem_GameObject_Manager["GO4"]->GetID() << endl;
  gSystem_GameObject_Manager.RemoveGameObject("GO4");
  cout << "ID GO4:" << GO4->GetID() << endl;
  GO4->AddToSystem();
  cout << "ID GO4:" << GO4->GetID() << endl;
  GO4->RemoveFromSystem();
  cout << "ID GO4:" << GO4->GetID() << endl;
  cout << "ID GO4:" << gSystem_GameObject_Manager.AddGameObject(GO4) << endl;

  gSystem_GameObject_Manager.RebuildIndex();
  cout << "ID GO4:" << gSystem_GameObject_Manager["GO4"]->GetID() << endl;

  CGameObject* GO1 = gSystem_GameObject_Manager["GO1"];
  CGameObject* GO2 = gSystem_GameObject_Manager["GO2"];
  CGameObject* GO3 = gSystem_GameObject_Manager["GO3"];

  // Comprobar funcionalidad de los padres e hijos

  GO1->AddChild(GO2);
  GO1->AddChild(GO3);
  GO2->AddChild(GO4);
  //GO4->AddChild(GO2); // falla y no hace nada, bien
  //GO4->AddChild(GO4); // falla y no hace nada, bien

  cout << "Padre de GO1: " << (GO1->GetParent() == NULL? "NULL": GO1->GetParent()->GetName()) << endl;
  cout << "Padre de GO2: " << (GO2->GetParent() == NULL? "NULL": GO2->GetParent()->GetName()) << endl;
  cout << "Padre de GO3: " << (GO3->GetParent() == NULL? "NULL": GO3->GetParent()->GetName()) << endl;
  cout << "Padre de GO4: " << (GO4->GetParent() == NULL? "NULL": GO4->GetParent()->GetName()) << endl;

  // Comprobar posiciones

  CComponent_Transform* datos_GO1 = GO1->GetComponent<CComponent_Transform>();
  CComponent_Transform* datos_GO2 = GO2->GetComponent<CComponent_Transform>();
  CComponent_Transform* datos_GO3 = GO3->GetComponent<CComponent_Transform>();
  CComponent_Transform* datos_GO4 = GO4->GetComponent<CComponent_Transform>();

  cout << "POS GO1: " << datos_GO1->position.x << " " << datos_GO1->position.y << " " << datos_GO1->position.z << endl;
  cout << "POS GO2: " << datos_GO2->position.x << " " << datos_GO2->position.y << " " << datos_GO2->position.z << endl;
  cout << "POS GO3: " << datos_GO3->position.x << " " << datos_GO3->position.y << " " << datos_GO3->position.z << endl;
  cout << "POS GO4: " << datos_GO4->position.x << " " << datos_GO4->position.y << " " << datos_GO4->position.z << endl;
  cout << "ROT GO1: " << datos_GO1->angle.x << " " << datos_GO1->angle.y << " " << datos_GO1->angle.z << endl;
  cout << "ROT GO2: " << datos_GO2->angle.x << " " << datos_GO2->angle.y << " " << datos_GO2->angle.z << endl;
  cout << "ROT GO3: " << datos_GO3->angle.x << " " << datos_GO3->angle.y << " " << datos_GO3->angle.z << endl;
  cout << "ROT GO4: " << datos_GO4->angle.x << " " << datos_GO4->angle.y << " " << datos_GO4->angle.z << endl;

  // llamadas a funciones

  vector3f translate_data(0.5f, 0.32f, -5.f);
  vector3f rotate_data(-5.f, -3.9f, -5.12f);
  //GO1->SendMessage(GO2, "helloworld", NULL);
 // GO1->SendMessage(GO2, "translate", &translate_data);
  //GO2->SendMessage(GO1, "translate", &translate_data);
  //GO2->SendMessage(GO3, "translate", &translate_data, NULL);
  //GO4->SendMessage("GO4", "helloworld", NULL);
  //GO1->CallFunc("rotate", &rotate_data);
  //GO2->CallFunc("rotate", &rotate_data);
  GO2->GetComponent<CComponent_Transform>()->Translate(translate_data);
  GO1->GetComponent<CComponent_Transform>()->Translate(translate_data);
  GO3->GetComponent<CComponent_Transform>()->Translate(translate_data);
  GO1->GetComponent<CComponent_Transform>()->Rotate(rotate_data);
  GO2->GetComponent<CComponent_Transform>()->Rotate(rotate_data);

  cout << "POS GO1: " << datos_GO1->position.x << " " << datos_GO1->position.y << " " << datos_GO1->position.z << endl;
  cout << "POS GO2: " << datos_GO2->position.x << " " << datos_GO2->position.y << " " << datos_GO2->position.z << endl;
  cout << "POS GO3: " << datos_GO3->position.x << " " << datos_GO3->position.y << " " << datos_GO3->position.z << endl;
  cout << "POS GO4: " << datos_GO4->position.x << " " << datos_GO4->position.y << " " << datos_GO4->position.z << endl;
  cout << "ROT GO1: " << datos_GO1->angle.x << " " << datos_GO1->angle.y << " " << datos_GO1->angle.z << endl;
  cout << "ROT GO2: " << datos_GO2->angle.x << " " << datos_GO2->angle.y << " " << datos_GO2->angle.z << endl;
  cout << "ROT GO3: " << datos_GO3->angle.x << " " << datos_GO3->angle.y << " " << datos_GO3->angle.z << endl;
  cout << "ROT GO4: " << datos_GO4->angle.x << " " << datos_GO4->angle.y << " " << datos_GO4->angle.z << endl;

  //gSystem_GameObject_Manager.RemoveGameObject(GO2);

  cout << "Padre de GO1: " << (GO1->GetParent() == NULL? "NULL": GO1->GetParent()->GetName()) << endl;
  cout << "Padre de GO2: " << (GO2->GetParent() == NULL? "NULL": GO2->GetParent()->GetName()) << endl;
  cout << "Padre de GO3: " << (GO3->GetParent() == NULL? "NULL": GO3->GetParent()->GetName()) << endl;
  cout << "Padre de GO4: " << (GO4->GetParent() == NULL? "NULL": GO4->GetParent()->GetName()) << endl;

  cout << GO1 << " " << GO2 << " " << GO3 << " " << GO4 << endl;
  gSystem_GameObject_Manager.SaveGameObjects("save_go.dat");

  gSystem_GameObject_Manager.DeleteGameObject(GO1, true);
  GO1 = gSystem_GameObject_Manager["GO1"];
  GO2 = gSystem_GameObject_Manager["GO2"];
  GO3 = gSystem_GameObject_Manager["GO3"];
  GO4 = gSystem_GameObject_Manager["GO4"];
  cout << GO1 << " " << GO2 << " " << GO3 << " " << GO4 << endl;
  return 0;
}*/
