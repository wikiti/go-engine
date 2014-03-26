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

    std::map<std::string, CGameObject*> gameObjects;
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

    void InitGameObject(std::string name);
    void CloseGameObject(std::string name);

    CGameObject* AddGameObject(std::string name, gameObject_type type = gameObject_empty, bool init = true); // cambiar con un enum
    CGameObject* AddGameObject(CGameObject* go, bool init = true);
    bool DeleteGameObject(std::string name, bool remove_children = true);
    void DeleteGameObjects();
    void DeleteGameObjects_NonPreserved();
    bool RemoveGameObject(std::string name);
      bool RebuildIndex(); // <- Mecanismo insano, pero puesto por si acaso (quien coño llega a 4 mil millones de objetos?)

    std::vector<CGameObject*> SearchGameObjects(std::string prefix);

    bool RenameGameObject(std::string name, std::string new_name);
    bool RenameGameObject(CGameObject* go, std::string new_name);

    CGameObject* GetGameObject(std::string name);
    CGameObject* operator[](std::string name);

    void DisableGameObject(std::string name);
    void EnableGameObject(std::string name);
    void SetGameObject(std::string name, bool state = true);

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
