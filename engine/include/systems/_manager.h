#ifndef __CSYSTEM_GAMEOBJECT_MANAGER
#define __CSYSTEM_GAMEOBJECT_MANAGER

#define __CSYSTEM_DATA_STORAGE_DEFAULT_SAVEFILE "temp_go.tmp"

#include "_object.h"
#include "_system.h"

class CSystem_GameObject_Manager: public CSystem
{
  protected:
    friend class CSystem_Render;
    friend class CSystem_Debug;

    map<string, CGameObject*> gameObjects;
    //map<string, function_t> gameObjects_functions;
    int last_ID;

  public:
    CSystem_GameObject_Manager(): CSystem() {};

    bool Init();
    void Close();
    bool Reset();

    //void LoadGameObjects(string file = __CSYSTEM_DATA_STORAGE_DEFAULT_SAVEFILE);
    //void SaveGameObjects(string file = __CSYSTEM_DATA_STORAGE_DEFAULT_SAVEFILE);

    void InitGameObjects();
    void CloseGameObjects();

    void InitGameObject(string name);
    void CloseGameObject(string name);

    CGameObject* AddGameObject(string name, gameObject_type type = gameObject_empty, bool init = true); // cambiar con un enum
    CGameObject* AddGameObject(CGameObject* go, bool init = true);
    bool DeleteGameObject(string name, bool remove_children = true);
    void DeleteGameObjects();
    void DeleteGameObjects_NonPreserved();
    bool RemoveGameObject(string name);
      bool RebuildIndex(); // <- Mecanismo insano, pero puesto por si acaso (quien coño llega a 4 mil millones de objetos?)

    vector<CGameObject*> SearchGameObjects(string prefix);

    bool RenameGameObject(string name, string new_name);
    bool RenameGameObject(CGameObject* go, string new_name);

    CGameObject* GetGameObject(string name);
    CGameObject* operator[](string name);

    void DisableGameObject(string name);
    void EnableGameObject(string name);
    void SetGameObject(string name, bool state = true);

    void EnableGameObjects();
    void DisableGameObjects();
    void SetGameObjects(bool state = true);

    void OnEvent();
    void OnInput();
    void OnLoop();
};

extern CSystem_GameObject_Manager gSystem_GameObject_Manager;
// Facilidad de uso para el usuario
extern CSystem_GameObject_Manager& gGameObjects;


//#define REGISTER_GAMEOBJECT_FUNCTION(X, Y) gSystem_GameObject_Manager.InsertGameObjectFunction(X, Y)
//#define GET_GAMEOBJECT_FUNCTION(X) gSystem_GameObject_Manager.GetGameObjectFunction(X)

#endif /* __CSYSTEM_GAMEOBJECT_MANAGER */
