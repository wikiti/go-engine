#include "systems/_manager.h"
#include "systems/_debug.h"


CSystem_GameObject_Manager gSystem_GameObject_Manager;
CSystem_GameObject_Manager& gGameObjects = gSystem_GameObject_Manager;

bool CSystem_GameObject_Manager::Init()
{
  CSystem::Init();
  last_ID = 0;

  return true;
}



void CSystem_GameObject_Manager::SaveGameObjects(string file)
{
  ofstream os;

  os.open(file.c_str(), ios_base::binary | ios_base::out);

  if(!os.good())
  {
    gSystem_Debug.console_error_msg("From CSystem_GameObject_Manager::SaveGameObjects: Could not open file: %s", file.c_str());
    return;
  }


  boost::archive::binary_oarchive oa(os);

  uint size = gameObjects.size();
  oa << size;

  for(map<string, CGameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
  {
    oa << it->second;
  }

  os.close();
}

void CSystem_GameObject_Manager::LoadGameObjects(string file)
{
  ifstream is;

  is.open(file.c_str(), ios_base::binary | ios_base::in);

  if(!is.good())
  {
    gSystem_Debug.console_error_msg("From CSystem_GameObject_Manager::LoadGameObjects: Could not open file: %s", file.c_str());
    return;
  }

  DeleteGameObjects();

  boost::archive::binary_iarchive ia(is);

  // esto deberia bastar...
  //ia >> gameObjects;
  /*for(map<string, CGameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
  {
    for(map<string, CComponent*>::iterator it2 = it->second->components.begin(); it2 != it->second->components.end(); it2++)
      it2->second->AddFuncs(it->second);
    it->second->functions["helloworld"] = (&CGameObject::CallFunc_HelloWorld);
  }*/

  uint size;
  ia >> size;
  for(uint i = 0; i < size && size; i++)
  {
    CGameObject* obj;
    ia >> obj;
    gameObjects.insert(pair<string, CGameObject*>(obj->GetName(), obj));
    // Esto sobra
    /*for(map<int, CComponent*>::iterator it = obj->components.begin(); it != obj->components.end(); it++)
      it->second->AddFuncs(obj);*/


    //gameObjects[obj->GetName()]->functions["helloworld"] = (&CGameObject::CallFunc_HelloWorld);
  }

  is.close();
}

void CSystem_GameObject_Manager::Close()
{
  DeleteGameObjects();

  CSystem::Close();
}

void CSystem_GameObject_Manager::InitGameObjects()
{
  for(map<string, CGameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
    it->second->Init();
}

void CSystem_GameObject_Manager::CloseGameObjects()
{
  for(map<string, CGameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
    it->second->Close();
}

void CSystem_GameObject_Manager::InitGameObject(string name)
{
  map<string, CGameObject*>::iterator it = gameObjects.find(name);
  if(it != gameObjects.end())
    it->second->Init();
  else
    gSystem_Debug.console_warning_msg("From CSystem_GameObject_Manager::InitGameObject: Could not find objet \"%s\"", name.c_str());
}

void CSystem_GameObject_Manager::CloseGameObject(string name)
{
  map<string, CGameObject*>::iterator it = gameObjects.find(name);
  if(it != gameObjects.end())
    it->second->Close();
  else
    gSystem_Debug.console_warning_msg("From CSystem_GameObject_Manager::CloseGameObject: Could not find objet \"%s\"", name.c_str());
}

CGameObject* CSystem_GameObject_Manager::AddGameObject(string nombre, gameObject_type type, bool init)
{
  map<string, CGameObject*>::iterator it = gameObjects.find(nombre);
  if(it == gameObjects.end())
  {
    CGameObject* go =new CGameObject(nombre);
    gameObjects.insert(pair<string, CGameObject*>(nombre, go));

    it = gameObjects.find(nombre);

    it->second->Register(last_ID);
    last_ID++;
    if(last_ID == INT_MAX)
    {
      if(!RebuildIndex())
        return NULL;//return -1;
    }

    if(init) it->second->Init();

    return it->second;//return it->second->GetID();
  }
  return NULL;//return -1;
}

CGameObject* CSystem_GameObject_Manager::AddGameObject(CGameObject* go, bool init)
{
  map<string, CGameObject*>::iterator it = gameObjects.find(go->GetName());
  if(it != gameObjects.end() || go == NULL) // Solo lo colocamos si no existe otro o si go no es NULL
    return NULL;//return -1;

  gameObjects.insert(pair<string, CGameObject*>(go->GetName(), go));
  it = gameObjects.find(go->GetName());

  it->second->Register(last_ID);
  last_ID++;
  if(last_ID == INT_MAX)
  {
    if(!RebuildIndex())
      return NULL;//return -1;
  }

  if(init) it->second->Init();

  return it->second;//return it->second->GetID();
}

bool CSystem_GameObject_Manager::DeleteGameObject(string nombre, bool remove_children)
{
  // Borrar solo si se encuentra
  map<string, CGameObject*>::iterator it = gameObjects.find(nombre);
  if(it != gameObjects.end())
  {
    int num_hijos = it->second->GetNumChildren();
    if(remove_children)
      for(int i = 0; i < num_hijos && num_hijos != 0; i++)
        DeleteGameObject(it->second->GetChild(num_hijos-1-i));
    else
      for(int i = 0; i < num_hijos && num_hijos != 0; i++)
        it->second->GetChild(num_hijos-1-i)->UnParent();

    delete it->second;
    gameObjects.erase(it);

    return true;
  }

  gSystem_Debug.console_warning_msg("From CSystem_GameObject_Manager::DeleteGameObject: Could not find objet \"%s\"", nombre.c_str());

  return false;
}

bool CSystem_GameObject_Manager::DeleteGameObject(CGameObject* go, bool remove_children)
{
  for(map<string, CGameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
  {
    if(it->second == go)
    {
      int num_hijos = it->second->GetNumChildren();
      if(remove_children)
        for(int i = 0; i < num_hijos && num_hijos != 0; i++)
          DeleteGameObject(it->second->GetChild(num_hijos-1-i), true);
      else
        for(int i = 0; i < num_hijos && num_hijos != 0; i++)
          it->second->GetChild(num_hijos-1-i)->UnParent();

      delete it->second;
      gameObjects.erase(it);

      return true;
    }
  }

  gSystem_Debug.console_warning_msg("From CSystem_GameObject_Manager::DeleteGameObject: Could not find objet \"%i\"", go);

  return false;
}

bool CSystem_GameObject_Manager::RemoveGameObject(string str)
{
  map<string, CGameObject*>::iterator it = gameObjects.find(str);
  if(it != gameObjects.end())
  {
    //¿Borrarlos? naa, muy hardcore
    int num_hijos = it->second->GetNumChildren();
    for(int i = 0; i < num_hijos && num_hijos != 0; i++)
      it->second->RemoveChild(it->second->GetChild(0)->GetName()); // Como se borran, accedemos siempre al primero hasta quedarnos sin ninguno

    it->second->Register(-1);
    it->second->UnParent();
    gameObjects.erase(it);

    return true;
  }

  gSystem_Debug.console_warning_msg("From CSystem_GameObject_Manager::RemoveGameObject: Could not find objet \"%s\"", str.c_str());

  return false;
}

bool CSystem_GameObject_Manager::RemoveGameObject(CGameObject* go)
{
  for(map<string, CGameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
  {
    if(it->second == go)
    {
      //¿Borrarlos? naa, muy hardcore
      int num_hijos = it->second->GetNumChildren();
      for(int i = 0; i < num_hijos && num_hijos != 0; i++)
        it->second->RemoveChild(it->second->GetChild(0)->GetName()); // Como se borran, accedemos siempre al primero hasta quedarnos sin ninguno

      it->second->Register(-1);
      it->second->UnParent();
      gameObjects.erase(it);
      return true;
    }
  }
  gSystem_Debug.console_warning_msg("From CSystem_GameObject_Manager::RemoveGameObject: Could not find objet \"%i\"", go);
  return false;
}

bool CSystem_GameObject_Manager::RebuildIndex()
{
  map<string, CGameObject*>::iterator it;
  last_ID = 0;
  for(it = gameObjects.begin(); it != gameObjects.end(); it++)
  {
    it->second->Register(last_ID);
    last_ID ++;
  }
  if(last_ID == INT_MAX) // Si está llena, no se pueden colocar más objetos
    return false;

  return true;
}

void CSystem_GameObject_Manager::DeleteGameObjects()
{
  for(map<string, CGameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
  {
    it->second->Close();
    delete it->second;
  }

  gameObjects.clear();
}

CGameObject* CSystem_GameObject_Manager::GetGameObject(string nombre)
{
  map<string, CGameObject*>::iterator it = gameObjects.find(nombre);
  if(it != gameObjects.end())
    return it->second;
//  else
//    gSystem_Debug.console_warning_msg("From CSystem_GameObject_Manager::GetGameObject: Could not find objet \"%s\"", nombre.c_str());

  return NULL;
}

CGameObject* CSystem_GameObject_Manager::operator[](string nombre)
{
  map<string, CGameObject*>::iterator it = gameObjects.find(nombre);
  if(it != gameObjects.end())
    return it->second;
//  else
//    gSystem_Debug.console_warning_msg("From CSystem_GameObject_Manager::operator[]: Could not find objet \"%s\"", nombre.c_str());

  return NULL;
}

void CSystem_GameObject_Manager::DisableGameObject(string name)
{
  map<string, CGameObject*>::iterator it = gameObjects.find(name);
  if(it != gameObjects.end())
    it->second->Disable();
//  else
//    gSystem_Debug.console_warning_msg("From CSystem_GameObject_Manager::DisableGameObject: Could not find objet \"%s\"", name.c_str());
}

void CSystem_GameObject_Manager::EnableGameObject(string name)
{
  map<string, CGameObject*>::iterator it = gameObjects.find(name);
  if(it != gameObjects.end())
    it->second->Enable();
  else
    gSystem_Debug.console_warning_msg("From CSystem_GameObject_Manager::EnableGameObject: Could not find objet \"%s\"", name.c_str());
}

void CSystem_GameObject_Manager::SetGameObject(string name, bool state)
{
  map<string, CGameObject*>::iterator it = gameObjects.find(name);
  if(it != gameObjects.end())
    it->second->SetState(state);
  else
    gSystem_Debug.console_warning_msg("From CSystem_GameObject_Manager::SetGameObject: Could not find objet \"%s\"", name.c_str());
}

void CSystem_GameObject_Manager::OnEvent()
{
  for(map<string, CGameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
    it->second->OnEvent();
}

void CSystem_GameObject_Manager::OnKeyEvent()
{
  for(map<string, CGameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
    it->second->OnKeyEvent();
}

void CSystem_GameObject_Manager::OnLoop()
{
  for(map<string, CGameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
    it->second->OnLoop();
}

// Esto se hará desde el Render
void CSystem_GameObject_Manager::OnRender()
{
  for(map<string, CGameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++)
    it->second->OnRender();
}


