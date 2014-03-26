#ifndef __CSYSTEM_DATA_STORAGE_
#define __CSYSTEM_DATA_STORAGE_

#define __CSYSTEM_DATA_STORAGE_SAVEFILE "data/cfg/vars.dat"
#define __CSYSTEM_DATA_STORAGE_CONFIG_SAVEFILE "data/cfg/user.cfg"

#define __CSYSTEM_DATA_STORAGE_NOSTRING ""
#define __CSYSTEM_DATA_STORAGE_NOINT INT_MAX
#define __CSYSTEM_DATA_STORAGE_NOFLOAT FLT_MAX

#define __NO_STRING  __CSYSTEM_DATA_STORAGE_NOSTRING
#define __NO_INT     __CSYSTEM_DATA_STORAGE_NOINT
#define __NO_FLOAT   __CSYSTEM_DATA_STORAGE_NOFLOAT

#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_WIDTH 1200
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_HEIGHT 800
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_WINDOW_MODE "windowed"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_MULTISAMPLING 1
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_MULTISAMPLING_VALUE 1

#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_SOUND_RADIUS 0
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_SOUND_RADIUS_Z 10.f
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_SOUND_RADIUS_X 10.f

#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM 1
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID 1
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_ROWS 20
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_ROWS_SCALE 1.f
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_COLS 20
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_COLS_SCALE 1.f

#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_SOUND_VOLUME 1.0
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_SOUND_MUSIC_VOLUME 1.0
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_SOUND_NUMBER_SOURCES 225
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_SOUND_NUMBER_SOURCES_ONESHOT 30

#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS1_UP_KEY "W"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS1_DOWN_KEY "S"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS1_LEFT_KEY "A"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS1_RIGHT_KEY "D"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS2_UP_KEY "Up"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS2_DOWN_KEY "Down"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS2_LEFT_KEY "Left"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS2_RIGHT_KEY "Right"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_ACTION1_KEY "N"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_ACTION2_KEY "M"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_ACTION3_KEY "J"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_ACTION4_KEY "K"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_RUN_KEY "Left Shift"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_CROUCH_KEY "Left Ctrl"
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_JUMP_KEY "Space"

#include "_globals.h"
#include "_system.h"

class CSystem_Data_Storage: public CSystem
{
  protected:
    friend class CSystem_Debug;

    std::map<std::string, std::string> strings;
    std::map<std::string, int> ints; //GLint
    std::map<std::string, float> floats; //GLfloat

    //bool enabled

  public:
    CSystem_Data_Storage(): CSystem() {};

    bool Init();
    void Close();

    bool Reset();

    // Cargar y guardar encriptados?
    bool Save(const char* file = __CSYSTEM_DATA_STORAGE_SAVEFILE);
    bool Load(const char* file = __CSYSTEM_DATA_STORAGE_SAVEFILE);

    // Modificar, instanciar gSystem_Config o algo
    void LoadConfig();
    void SaveConfig();

    void SetString(std::string name_id, std::string value);
    void SetInt(std::string name_id, int value);
    void SetFloat(std::string name_id, float value);

    std::string GetString(std::string name_id);
    int GetInt(std::string name_id);
    float GetFloat(std::string name_id);

    bool RemoveString(std::string name_id);
    bool RemoveInt(std::string name_id);
    bool RemoveFloat(std::string name_id);

    bool ExistsInt(std::string name_id);
    bool ExistsFloat(std::string name_id);
    bool ExistsString(std::string name_id);

    void RemoveAll();
    void RemoveUserVars();
};

// Crear 2 objetos: 1 para la configuración de la aplicación, y otra para guardar variables de usuario.
extern CSystem_Data_Storage gSystem_Data_Storage;
extern CSystem_Data_Storage& gData;

#endif /* __CSYSTEM_DATA_STORAGE_ */
