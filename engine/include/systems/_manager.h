#ifndef __CSYSTEM_GAMEOBJECT_MANAGER
#define __CSYSTEM_GAMEOBJECT_MANAGER

#define __CSYSTEM_DATA_STORAGE_DEFAULT_SAVEFILE "temp_go.tmp"

#include "_object.h"
#include "_system.h"

// ->PORHACER Cambiar los nombres de los métodos de CSystem_GameObject_Manager por algo más sencillo (AddGameObject -> Add).
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

    CGameObject* Add(std::string name, gameObject_type type = gameObject_empty, bool init = true); // cambiar con un enum
    CGameObject* Add(CGameObject* go, bool init = true);
    bool Delete(std::string name, bool delete_children = true);
    void DeleteAll();
    void DeleteAll_NonPreserved();
    bool Remove(std::string name, bool remove_children = true); // ->PORHACER Hay que probar la función CSystem_GameObject_Manager::Remove().
      bool RebuildIndex(); // <- Mecanismo insano, pero puesto por si acaso (quien coño llega a 4 mil millones de objetos?)

    std::vector<CGameObject*> Search(std::string prefix);

    bool Rename(std::string name, std::string new_name);
    bool Rename(CGameObject* go, std::string new_name);

    CGameObject* Get(std::string name);
    CGameObject* operator[](std::string name);

    void DisableGameObject(std::string name, bool recursive = true);
    void EnableGameObject(std::string name, bool recursive = true);
    void SetGameObjectState(std::string name, bool state = true, bool recursive = true);

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
