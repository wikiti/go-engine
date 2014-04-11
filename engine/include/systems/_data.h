/**
 * @file
 * @brief Fichero que incluye la clase CSystem_Data_Storage y sus estancias globales.
 */

#ifndef __CSYSTEM_DATA_STORAGE_
#define __CSYSTEM_DATA_STORAGE_

/** Ficheros donde se guardarán las variables. */
#define __CSYSTEM_DATA_STORAGE_SAVEFILE "data/cfg/vars.dat"
/** Ficheros donde se guardarán las variables de usuario. @no_use */
#define __CSYSTEM_DATA_STORAGE_CONFIG_SAVEFILE "data/cfg/user.cfg"

/** Valor de variable tipo string nula. */
#define __CSYSTEM_DATA_STORAGE_NOSTRING ""
/** Valor de variable tipo entera nula. */
#define __CSYSTEM_DATA_STORAGE_NOINT INT_MAX
/** Valor de variable tipo flotante nula. */
#define __CSYSTEM_DATA_STORAGE_NOFLOAT FLT_MAX

/** Valor de variable tipo string nula. */
#define __NO_STRING  __CSYSTEM_DATA_STORAGE_NOSTRING
/** Valor de variable tipo string nula. */
#define __NO_INT     __CSYSTEM_DATA_STORAGE_NOINT
/** Valor de variable tipo string nula. */
#define __NO_FLOAT   __CSYSTEM_DATA_STORAGE_NOFLOAT

/** Valor por defecto de la variable "__RENDER_RESOLUTION_WIDTH", siendo el ancho de la ventana. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_WIDTH 1200
/** Valor por defecto de la variable "__RENDER_RESOLUTION_HEIGHT", siendo el alto de la ventana. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_HEIGHT 800
/** Valor por defecto de la variable "__RENDER_RESOLUTION_WINDOW_MODE", siendo el modo la ventana. (ventana, pantalla completa y ventana completa). */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_WINDOW_MODE "windowed"
/** Valor por defecto de la variable "__RENDER_RESOLUTION_MULTISAMPLING", para activar o desactivar el multimuestreo (antialising). */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_MULTISAMPLING 1
/** Valor por defecto de la variable "__RENDER_RESOLUTION_MULTISAMPLING_VALUE", para determinar el número de buffers a usar con el multimuestreo. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_MULTISAMPLING_VALUE 1

/** Valor por defecto de la variable "__RENDER_SOUND_RADIUS", para activar o desactivar el dibujado del sonido. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_SOUND_RADIUS 0
/** Valor por defecto de la variable "__RENDER_SOUND_RADIUS_Z", para  saber las "Z" secciones que tienen las esferas de sonido. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_SOUND_RADIUS_Z 10.f
/** Valor por defecto de la variable "__RENDER_SOUND_RADIUS_X", para  saber las "X" secciones que tienen las esferas de sonido. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_SOUND_RADIUS_X 10.f

/** Valor por defecto de la variable "__RENDER_TRANSFORM", para activar o desactivar el dibujado de los componentes de transformación. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM 1
/** Valor por defecto de la variable "__RENDER_TRANSFORM_GRID", para activar o desactivar el dibujado de la rejilla. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID 1
/** Valor por defecto de la variable "__RENDER_TRANSFORM_GRID_ROWS", para definir el número de filas en la rejilla. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_ROWS 20
/** Valor por defecto de la variable "__RENDER_TRANSFORM_GRID_ROWS_SCALE", para definir la escala de las filas en la rejilla. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_ROWS_SCALE 1.f
/** Valor por defecto de la variable "__RENDER_TRANSFORM_GRID_COLS", para definir el número de columnas en la rejilla. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_COLS 20
/** Valor por defecto de la variable "__RENDER_TRANSFORM_GRID_COLS_SCALE", para definir la escala de las columnas en la rejilla. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_COLS_SCALE 1.f

/** Valor por defecto de la variable "__SOUND_VOLUME", para definir el volumen del juego. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_SOUND_VOLUME 1.0
/** Valor por defecto de la variable "__SOUND_MUSIC_VOLUME", para definir el volumen de la música del juego. */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_SOUND_MUSIC_VOLUME 1.0
/** Valor por defecto de la variable "__SOUND_VOLUME_NUMBER_SOURCES", recursos usados para los componentes CComponent_Audio_Source con Bind(). */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_SOUND_NUMBER_SOURCES 225
/** Valor por defecto de la variable "__SOUND_VOLUME_NUMBER_SOURCES_ONESHIT", recursos usados para los componentes CComponent_Audio_Source con oneshots ( CComponent_Audio_Source::PlayOneShot() ). */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_SOUND_NUMBER_SOURCES_ONESHOT 30

/** Valor por defecto de la variable "__INPUT_AXIS1_UP_KEY", para definir la tecla por defecto "arriba" del eje 1 (wads). */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS1_UP_KEY "W"
/** Valor por defecto de la variable "__INPUT_AXIS1_DOWN_KEY", para definir la tecla por defecto "abajo" del eje 1 (wads). */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS1_DOWN_KEY "S"
/** Valor por defecto de la variable "__INPUT_AXIS1_LEFT_KEY", para definir la tecla por defecto "izquierda" del eje 1 (wads). */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS1_LEFT_KEY "A"
/** Valor por defecto de la variable "__INPUT_AXIS1_RIGHT_KEY", para definir la tecla por defecto "derecha" del eje 1 (wads). */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS1_RIGHT_KEY "D"
/** Valor por defecto de la variable "__INPUT_AXIS2_UP_KEY", para definir la tecla por defecto "arriba" del eje 2 (flechas). */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS2_UP_KEY "Up"
/** Valor por defecto de la variable "__INPUT_AXIS2_DOWN_KEY", para definir la tecla por defecto "abajo" del eje 2 (flechas). */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS2_DOWN_KEY "Down"
/** Valor por defecto de la variable "__INPUT_AXIS2_LEFT_KEY", para definir la tecla por defecto "izquierda" del eje 2 (flechas). */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS2_LEFT_KEY "Left"
/** Valor por defecto de la variable "__INPUT_AXIS2_RIGHT_KEY", para definir la tecla por defecto "derecha" del eje 2 (flechas). */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS2_RIGHT_KEY "Right"
/** Valor por defecto de la variable "__INPUT_ACTION1_KEY", para definir la tecla por defecto de "acción1". */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_ACTION1_KEY "N"
/** Valor por defecto de la variable "__INPUT_ACTION2_KEY", para definir la tecla por defecto de "acción2". */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_ACTION2_KEY "M"
/** Valor por defecto de la variable "__INPUT_ACTION3_KEY", para definir la tecla por defecto de "acción3". */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_ACTION3_KEY "J"
/** Valor por defecto de la variable "__INPUT_ACTION4_KEY", para definir la tecla por defecto de "acción4". */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_ACTION4_KEY "K"
/** Valor por defecto de la variable "__INPUT_RUN_KEY", para definir la tecla por defecto de "correr". */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_RUN_KEY "Left Shift"
/** Valor por defecto de la variable "__INPUT_CROUCH_KEY", para definir la tecla por defecto de "agacharse". */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_CROUCH_KEY "Left Ctrl"
/** Valor por defecto de la variable "__INPUT_JUMP_KEY", para definir la tecla por defecto de "saltar". */
#define __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_JUMP_KEY "Space"

#include "_globals.h"
#include "_system.h"

/** @addtogroup Sistemas */
/*@{*/

class CSystem_Data_Storage: public CSystem
{
  protected:
    friend class CSystem_Debug;

    std::map<std::string, std::string> strings;
    std::map<std::string, int> ints; //GLint
    std::map<std::string, float> floats; //GLfloat

    //bool enabled

    bool Init();
    void Close();

    bool Reset();

  public:
    CSystem_Data_Storage(): CSystem() {};

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

// ->NOTA Para el gestor de datos, se deberían crear 2 objetos: 1 para la configuración de la aplicación, y otra para guardar variables de usuario.
/**
 * @brief Objeto global del sistema "Data Storage".
 *
 * Se debe usar como única estancia de dicho sistema. Para mayor comodidad, use el objeto gData.
 */
extern CSystem_Data_Storage gSystem_Data_Storage;
/**
 * @brief Objeto global del sistema "Data Storage".
 *
 * Se debe usar como única estancia de dicho sistema.
 */
extern CSystem_Data_Storage& gData;

/*@}*/

#endif /* __CSYSTEM_DATA_STORAGE_ */
