/**
 * @file
 * @brief Fichero que incluye la clase CSystem_Debug y sus estancias globales.
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#define __CSYSTEM_DEBUG_STORAGE_SAVEFILE "data/log.txt"
#define __CSYSTEM_DEBUG_STORAGE_ACCESS "a"
#define __CSYSTEM_DEBUG_CONSOLE_FONT "__CONSOLE_FONT"
#define __CSYSTEM_DEBUG_CONSOLE_FONT_FILE "data/engine/console_font.tga"

#define __CSYSTEM_DEBUG_KEYWORD_ALL "$ALL"

#define __CSYSTEM_DEBUG_CONSOLE_SIZE 200
#define __CSYSTEM_DEBUG_CONSOLE_CONSOLE_BUFFER_MAXLINES 1024
#define __CSYSTEM_DEBUG_CONSOLE_COMMAND_BUFFER_MAXLINES 1024
#define __CSYSTEM_DEBUG_CONSOLE_LINES 11
#define __CSYSTEM_DEBUG_CONSOLE_LINESPACE 16
#define __CSYSTEM_DEBUG_CONSOLE_Y_OFFSET 30
#define __CSYSTEM_DEBUG_CONSOLE_X_OFFSET 10

#include <cstdarg>
#include <stdio.h>
#include <time.h>

#include "_object.h"
#include "systems/_system.h"

/** @addtogroup Sistemas */
/*@{*/

/**
 * @brief Espacio de nombres para algunos elementos usados por el sistema CSystem_Debug.
 *
 */
namespace Debug
{
  /**
   * @brief Enum para los distintos tipos de mensajes de alerta.
   *
   * Un di�logo o message box es un *pop-up* que permite alertar al usuario de errores, advertencias o informaci�n.
   */
  enum flags_t {error = SDL_MESSAGEBOX_ERROR, warning = SDL_MESSAGEBOX_WARNING, information = SDL_MESSAGEBOX_INFORMATION};
  /**
   * @brief Estructura que represente una l�nea de texto dentro de la consola.
   *
   * Cada l�nea tiene asociada un texto (string) y un color (colorf_t). Se usa de manera interna en
   * el sistema CSystem_Debug.
   */
  typedef struct string_console_t
  {
    colorf_t line_color;
    std::string str;

    string_console_t(): str("")
    {
      line_color.r = line_color.g = line_color.b = line_color.a = 1.f;
    }

    string_console_t(std::string msg, GLfloat r = 1.f, GLfloat g = 1.f, GLfloat b = 1.f, GLfloat a = 1.f): str(msg)
    {
      line_color.r = r;
      line_color.g = g;
      line_color.b = b;
      line_color.a = a;
    }

  } string_console_t;
}

/**
 * @brief Sistema que representa el depurador del motor.
 *
 * Este sistema, uno de los m�s importante para el desarrollo, es el encargado de servir de utililidad
 * para depurar algunos aspectos del programa, tales como mostrar errores, advertencias, guardar informaci�n en
 * un log, etc.
 *
 * Actualmente, el depurador realiza las siguientes acciones:
 *
 * - Escribir informaci�n o errores en un *log* (fichero de texto, por defecto en __CSYSTEM_DEBUG_STORAGE_SAVEFILE).
 * - Abrir advertencias o pop-ups para informar o advertir al usuario sobre los aspectos deseados (errores, notificaciones...).
 * - Una consola o terminal de comandos completamente funcional.
 *   - Dicha consola se activar� mediante la tecla F10. <- gInput
 */
class CSystem_Debug: public CSystem
{
  friend class CInstance;
  friend bool Systems_Init();
  friend void Systems_Close();
  friend bool Systems_Reset();

  protected:
    // Debug
    bool opened;
    FILE* file;

    // Console
    std::vector<Debug::string_console_t> console_buffer;
    std::vector<std::string> command_buffer;

    int current_line_buffered;
    int current_last_command;

    typedef void (CSystem_Debug::*command_p)(std::string);
    std::map<std::string, command_p> console_commands;

    bool console;
    std::string input;
    void ParseInput();
    void ParseAppArguments();

    // Console Font
    // De momento no vamos a usar los VBOs, ya que es el debugger y no tiene mucha importancia :D
    /*GLuint m_ConsoleFontVAO;
    GLuint m_ConsoleFontVBOVertices;
    GLuint m_ConsoleFontVBOTexCoords;
    GLuint m_ConsoleFontVBOTypeInfo;*/
    GLuint base;
    void print(GLint x, GLint y, int set, const char* fmt, ...);

    // Puntero de consola
    uint console_pointer_pos;

    bool Init();
      bool InitConsoleFont();
      bool InitCommandMap();

    void Close();
    bool Reset();

    void OnEvent();
    void OnRender();

  public:
    CSystem_Debug(): CSystem(), opened(false), file(NULL){}
    bool IsOpened(){return opened;}

    bool IsConsole()
    {
      return console;
    }

    // Commands
    void command(const std::string& command, bool log = false);

    // log.txt File
    void log(const char* fmt, ...);
    void error(const char* fmt, ...);
    void raw_log(const char* fmt, ...);

    // Message Boxes
    // Flags: debug::error for error, debug::warning for warnings, debug::information for info
    void msg_box(Uint32 flags, const char* title, const char* message);
    void msg_boxf(Uint32 flags, const char* title, const char* fmt, ...);

    // Console
    void console_msg(const char* fmt, ...);
    void console_error_msg(const char* fmt, ...);
    void console_warning_msg(const char* fmt, ...);
    /**
     * @brief TEST
     *
     * Texto de prueba para <span style="background-color: black; color: #57b8d9;">mostrar un texto azul</span>.
     *
     * @param fmt
     */
    void console_info_msg(const char* fmt, ...);
    void console_custom_msg(GLfloat r, GLfloat g, GLfloat b, GLfloat a, const char* fmt, ...);

  protected:
      // Console commands
    // Test

    // General
    void Console_command__UNKNOWN_COMMAND(std::string arguments);
    void Console_command__HELP(std::string arguments);
    void Console_command__LOG(std::string arguments);

    void Console_command__SET_INT(std::string arguments);
    void Console_command__SET_FLOAT(std::string arguments);
    void Console_command__SET_STRING(std::string arguments);

    void Console_command__GET_INT(std::string arguments);
    void Console_command__GET_FLOAT(std::string arguments);
    void Console_command__GET_STRING(std::string arguments);
    void Console_command__GET_VAR(std::string arguments);

    void Console_command__REMOVE_INT(std::string arguments);
    void Console_command__REMOVE_FLOAT(std::string arguments);
    void Console_command__REMOVE_STRING(std::string arguments);
    void Console_command__REMOVE_USER_VARS(std::string arguments);

    void Console_command__EXIT(std::string arguments);
    void Console_command__CLEAR(std::string arguments);
    void Console_command__ABORT(std::string arguments);
    void Console_command__RUN(std::string arguments);

    void Console_command__SECRET_PLZ(std::string arguments);

    //void Console_command__SAVE_STATE(std::string arguments);
    //void Console_command__LOAD_STATE(std::string arguments);

    // Systems
    void Console_command__SYSTEM_TIME_SETSCALE(std::string arguments);
    void Console_command__SYSTEM_USERINPUT_SHOW_JOYSTICKS(std::string arguments);

    // Game Objects
    void Console_command__GO_SHOW_TREE(std::string arguments);
      void Console_command__AUX__GO_SHOW_TREE_print_element(CGameObject* go, std::map<std::string, void*>& list, int level = 1);
    void Console_command__GO_ENABLE(std::string arguments);
    void Console_command__GO_COMPONENT_ENABLE(std::string arguments);
    void Console_command__GO_COMPONENT_SET(std::string arguments);
    void Console_command__GO_COMPONENT_GET(std::string arguments);
    void Console_command__GO_SEARCH(std::string arguments);
    //void Console_command__ENABLE_SYSTEM(std::string arguments);

    // Sound
    void Console_command__SND_VOLUME(std::string arguments);
    void Console_command__SND_MUSIC_VOLUME(std::string arguments);
    void Console_command__SND_CHECKSOURCES(std::string arguments);

    // Render
    void Console_command__R_UPDATE_WINDOW(std::string arguments);
    void Console_command__R_RESIZE_WINDOW(std::string arguments);
    void Console_command__R_DRAW_TRANSFORM(std::string arguments);
    void Console_command__R_DRAW_GRID(std::string arguments);
    void Console_command__R_FPS(std::string arguments);
    void Console_command__R_DRAW_SOUND(std::string arguments);
    void Console_command__R_GLINFO(std::string arguments);
};

/**
 * @brief Objeto global del sistema "Debug".
 *
 * Se debe usar como �nica estancia de dicho sistema. Para mayor comodidad, use el objeto gDebug,
 * que es una referencia a este objeto.
 */
extern CSystem_Debug gSystem_Debug;
/**
 * @brief Objeto global del sistema "Debug".
 *
 * Se debe usar como �nica estancia de dicho sistema.
 */
extern CSystem_Debug& gDebug;

/*@}*/

#endif /* DEBUG_H_ */
