#ifndef __CSYSTEM_DATA_STORAGE_
#define __CSYSTEM_DATA_STORAGE_

#define __CSYSTEM_DATA_STORAGE_SAVEFILE "vars.dat"
#define __CSYSTEM_DATA_STORAGE_CONFIG_SAVEFILE "user.cfg"

#define __CSYSTEM_DATA_STORAGE_NOSTRING ""
#define __CSYSTEM_DATA_STORAGE_NOINT INT_MAX
#define __CSYSTEM_DATA_STORAGE_NOFLOAT FLT_MAX

#define __NO_STRING  __CSYSTEM_DATA_STORAGE_NOSTRING
#define __NO_INT     __CSYSTEM_DATA_STORAGE_NOINT
#define __NO_FLOAT   __CSYSTEM_DATA_STORAGE_NOFLOAT

#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RESOLUTION_WIDTH 1600
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RESOLUTION_HEIGHT 600

#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_SOUND_RADIUS 1
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_SOUND_RADIUS_Z 10.f
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_SOUND_RADIUS_X 10.f

#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM 1
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID 1
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_ROWS 75
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_ROWS_SIZE 0.2f
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_COLS 75
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_COLS_SIZE 0.2f


#include "_globals.h"
#include "_system.h"

class CSystem_Data_Storage: public CSystem
{
  protected:
    friend class CSystem_Debug;

    map<string, string> strings;
    map<string, int> ints; //GLint
    map<string, float> floats; //GLfloat

    //bool enabled

  public:
    CSystem_Data_Storage() {};

    bool Init();
    void Close();

    // Cargar y guardar encriptados?
    bool Save(const char* file = __CSYSTEM_DATA_STORAGE_SAVEFILE);
    bool Load(const char* file = __CSYSTEM_DATA_STORAGE_SAVEFILE);

    // Modificar, instanciar gSystem_Config o algo
    void LoadConfig();
    void SaveConfig();

    void SetString(string name_id, string value);
    void SetInt(string name_id, int value);
    void SetFloat(string name_id, float value);

    string GetString(string name_id);
    int GetInt(string name_id);
    float GetFloat(string name_id);

    bool RemoveString(string name_id);
    bool RemoveInt(string name_id);
    bool RemoveFloat(string name_id);

    bool ExistsInt(string name_id);
    bool ExistsFloat(string name_id);
    bool ExistsString(string name_id);

    void RemoveAll();
    void RemoveUserVars();
};

// Crear 2 objetos: 1 para la configuración de la aplicación, y otra para guardar variables de usuario.
extern CSystem_Data_Storage gSystem_Data_Storage;


#endif /* __CSYSTEM_DATA_STORAGE_ */
