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
   * Un diálogo o message box es un *pop-up* que permite alertar al usuario de errores, advertencias o información.
   */
  enum flags_t {error = SDL_MESSAGEBOX_ERROR, warning = SDL_MESSAGEBOX_WARNING, information = SDL_MESSAGEBOX_INFORMATION};
  /**
   * @brief Estructura que represente una línea de texto dentro de la consola.
   *
   * Cada línea tiene asociada un texto (string) y un color (colorf_t). Se usa de manera interna en
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
 * Este sistema, uno de los más importante para el desarrollo, es el encargado de servir de utililidad
 * para depurar algunos aspectos del programa, tales como mostrar errores, advertencias, guardar información en
 * un log, etc.
 *
 * Actualmente, el depurador realiza las siguientes acciones:
 *
 * - Escribir información o errores en un *log* (fichero de texto, por defecto en __CSYSTEM_DEBUG_STORAGE_SAVEFILE).
 * - Abrir advertencias o pop-ups para informar o advertir al usuario sobre los aspectos deseados (errores, notificaciones...).
 * - Una consola o terminal de comandos completamente funcional.
 *   - Dicha consola se activará mediante la tecla F10 por defecto, o con la tecla almacenada en la variable del sistema CSystem_Data_Storage "__INPUT_CONSOLE_KEY".
 *
 * @warning En caso de que la consola esté abierta, se ignorará la entrada del usuario hasta que se cierre. Esto significa que, sólo se
 * @warning procesará entrada para este sistema, y no para el resto de objetos.
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
    /**
     * @brief Constructor principal.
     *
     * Inicia el sistema.
     */
    CSystem_Debug(): CSystem(), opened(false), file(NULL){}
    /**
     * @brief Comprobar si el fichero de log está abierto.
     *
     * @deprecated
     * @no_use
     *
     * @return true si el fichero de log está abierto, false en caso contrario.
     */
    bool IsOpened(){return opened;}

    /**
     * @brief Comprobar si la consola está desplegada.
     *
     * @return En caso de que la consoa esté abierta, devuelve true. En caso contrario, devuelve false.
     */
    bool IsConsole()
    {
      return console;
    }

    // Commands
    /**
     * @brief Ejecutar un comando de la terminal.
     *
     * Ejecuta un comando de la consola, con los correspondientes argumentos. Por ejemplo, hacer
     *
     @code
     gDebug.command("go_show_tree");
     @endcode
     *
     * Es idéntico a abrir una terminal y escribir
     *
     @code
     > go_show_tree
     @endcode
     *
     * La lista completa de comandos es la siguiente:
     *
     * ->PorHacer Escribir lista de comandos en la documentación.
     *
     * @warning Puede que la lista de comandos anterior no este actualizada. Ejecute el comando "help" para
     * @warning ver la lista de comandos, en sus respectivas categorías.
     *
     * @param command Comando a ejecutar, con el la instruccion y los argumentos.
     * @param log Flag para saber si se debe escribir los comandos ejecutados en la terminal.
     */
    void command(const std::string& command, bool log = false);

    // log.txt File
    /**
     * @brief Escribir un mensaje en una línea del fichero de logs.
     *
     * Escribe una línea con formato al fichero de logs, que puede resultar útil para depurar algunos aspectos.
     *
     * Por ejemplo, usando:
     @code
     gDebug.log("Texto de prueba: %s -> %d", "nombre", 50);
     @endcode
     *
     * Genera la línea en el fichero de logs siguiente:
     @code
     [12/04/2014 09:59] Texto de prueba: nombre -> 50
     @endcode
     *
     * Si no quiere una marca de tiempo, use raw().
     *
     * @param fmt Formato de salida. Idéntico a usar "printf" de *C*.
     */
    void log(const char* fmt, ...);
    /**
     * @brief Escribir un mensaje de error en una línea del fichero de logs.
     *
     * Escribe una línea con formato al fichero de logs, que puede resultar útil para depurar algunos aspectos con respecto
     * a errores.
     *
     * Por ejemplo, usando:
     @code
     gDebug.error("No quedan huevos!!");
     @endcode
     *
     * Genera la línea en el fichero de logs siguiente:
     @code
     [12/04/2014 09:59] **ERROR** No quedan huevos!!
     @endcode
     *
     * Si no quiere una marca de tiempo, use raw().
     *
     * @param fmt Formato de salida. Idéntico a usar "printf" de *C*.
     */
    void error(const char* fmt, ...);
    /**
     * @brief Escribir un mensaje sin marca de tiempo y sin saltos de línea en el fichero de logs.
     *
     * Escribe una línea con formato al fichero de logs, que puede resultar útil para depurar algunos aspectos. No introduce
     * un retorno de carro al final de la línea, ni tampoco añade marca de tiempo.
     *
     * Por ejemplo, usando:
     @code
     gDebug.raw("Esto es una ");
     gDebug.raw("lineas\n");
     gDebug.raw("Esto es otra linea\n");
     @endcode
     *
     * Genera la línea en el fichero de logs siguiente:
     @code
     Esto es una linea
     Esto es otra linea
     @endcode
     *
     * @param fmt Formato de salida. Idéntico a usar "printf" de *C*.
     */
    void raw_log(const char* fmt, ...);

    // Message Boxes
    // Flags: debug::error for error, debug::warning for warnings, debug::information for info
    /**
     * @brief Mensaje estilo pop-up
     *
     * Genera una nueva ventana o diágolo con el mensaje a mostrar al usuario.
     *
     * @param flags Tipo de mensaje (error, información, warning). @see Debug::flags_t
     * @param title Título del diálogo.
     * @param fmt Cuerpo, texto o mensaje del diálogo. Como si se usase "printf" de *C*.
     */
    void msg_box(Uint32 flags, const char* title, const char* fmt, ...);

    // Console
    /**
     * @brief Escribir una línea en consola.
     *
     * Escribe una línea en consola con un formato determinado. Por ejemplo, usar
     @code
     gDebug.cosole_msg("Texto de prueba numero %d", 40);
     @endcode
     *
     * Generará por consola el mensaje:
     *
     * <span style="font-weight: bold; background-color: black; color: white; display: block; padding: 5px;">Texto de consola</span>
     *
     * @param fmt Cuerpo, texto o mensaje del diálogo. Como si se usase "printf" de *C*.
     */
    void console_msg(const char* fmt, ...);
    /**
     * @brief Escribir una línea de error (roja) en consola.
     *
     * Escribe una línea de error en consola con un formato determinado. Por ejemplo, usar
     @code
     gDebug.cosole_msg("Error! No quedan huevos!!");
     @endcode
     *
     * Generará por consola el mensaje:
     *
     * <span style="font-weight: bold; background-color: black; color: red; display: block; padding: 5px;">Error! No quedan huevos!!</span>
     *
     * @param fmt Cuerpo, texto o mensaje del diálogo. Como si se usase "printf" de *C*.
     */
    void console_error_msg(const char* fmt, ...);
    /**
     * @brief Escribir una línea de advertencia (naranja) en consola.
     *
     * Escribe una línea de advertencia en consola con un formato determinado. Por ejemplo, usar
     @code
     gDebug.cosole_msg("Advertencia! te quedan 2 huevos.");
     @endcode
     *
     * Generará por consola el mensaje:
     *
     * <span style="font-weight: bold; background-color: black; color: #ffbf00; display: block; padding: 5px;">Advertencia! te quedan 2 huevos.</span>
     *
     * @param fmt Cuerpo, texto o mensaje del diálogo. Como si se usase "printf" de *C*.
     */
    void console_warning_msg(const char* fmt, ...);
    /**
     * @brief Escribir una línea de información (azul) en consola.
     *
     * Escribe una línea de información en consola con un formato determinado. Por ejemplo, usar
     @code
     gDebug.cosole_msg("FYI: Esto es un mensaje de informacion.");
     @endcode
     *
     * Generará por consola el mensaje:
     *
     * <span style="font-weight: bold; background-color: black; color: #57b8d9; display: block; padding: 5px;">FYI: Esto es un mensaje de informacion.</span>
     *
     * @param fmt Cuerpo, texto o mensaje del diálogo. Como si se usase "printf" de *C*.
     */
    void console_info_msg(const char* fmt, ...);
    /**
     * @brief Escribir una línea con un color personalizado en consola.
     *
     * Escribe una línea con un color personalizado en consola con un formato determinado. Por ejemplo, usar
     *
     @code
     gDebug.console_custom_msg(0.6f, 0.f, 0.6f, 1.f, "Violeta");
     gDebug.console_custom_msg(0, 0, 0.6f, 1.f, "Azul oscuro");
     gDebug.console_custom_msg(0, 0, 1.f, 1.f, "Azul");
     gDebug.console_custom_msg(0.f, 1.f, 0.f, 1.f, "Verde");
     gDebug.console_custom_msg(1.f, 1.f, 0.f, 1.f, "Amarillo");
     gDebug.console_custom_msg(1.f, 0.5f, 0.f, 1.f, "Naranja");
     gDebug.console_custom_msg(1.f, 0.f, 0.f, 1.f, "Rojo");
     @endcode
     *
     * Generará por consola los mensajes:
     *
     * <span style="font-weight: bold; background-color: black; color: #990099; display: block; padding: 5px;">Violeta</span>
     * <span style="font-weight: bold; background-color: black; color: #000099; display: block; padding: 5px;">Azul oscuro</span>
     * <span style="font-weight: bold; background-color: black; color: #0000FF; display: block; padding: 5px;">Azul</span>
     * <span style="font-weight: bold; background-color: black; color: #00FF00; display: block; padding: 5px;">Verde</span>
     * <span style="font-weight: bold; background-color: black; color: #FFFF00; display: block; padding: 5px;">Amarillo</span>
     * <span style="font-weight: bold; background-color: black; color: #FF8000; display: block; padding: 5px;">Naranja</span>
     * <span style="font-weight: bold; background-color: black; color: #FF0000; display: block; padding: 5px;">Rojo</span>
     *
     * @param r Componente roja del color.
     * @param g Componente verde del color.
     * @param b Componente azul del color.
     * @param a Componente alfa (trasparencia) del color.
     * @param fmt Cuerpo, texto o mensaje del diálogo. Como si se usase "printf" de *C*.
     */
    void console_custom_msg(GLfloat r, GLfloat g, GLfloat b, GLfloat a, const char* fmt, ...);

    /**
     * @brief Escribir una línea con un color personalizado en consola.
     *
     * Escribe una línea con un color personalizado en consola con un formato determinado. Por ejemplo, usar
     *
     @code
     gDebug.console_custom_msg(0.6f, 0.f, 0.6f, 1.f, "Violeta");
     gDebug.console_custom_msg(0, 0, 0.6f, 1.f, "Azul oscuro");
     gDebug.console_custom_msg(0, 0, 1.f, 1.f, "Azul");
     gDebug.console_custom_msg(0.f, 1.f, 0.f, 1.f, "Verde");
     gDebug.console_custom_msg(1.f, 1.f, 0.f, 1.f, "Amarillo");
     gDebug.console_custom_msg(1.f, 0.5f, 0.f, 1.f, "Naranja");
     gDebug.console_custom_msg(1.f, 0.f, 0.f, 1.f, "Rojo");
     @endcode
     *
     * Generará por consola los mensajes:
     *
     * <span style="font-weight: bold; background-color: black; color: #990099; display: block; padding: 5px;">Violeta</span>
     * <span style="font-weight: bold; background-color: black; color: #000099; display: block; padding: 5px;">Azul oscuro</span>
     * <span style="font-weight: bold; background-color: black; color: #0000FF; display: block; padding: 5px;">Azul</span>
     * <span style="font-weight: bold; background-color: black; color: #00FF00; display: block; padding: 5px;">Verde</span>
     * <span style="font-weight: bold; background-color: black; color: #FFFF00; display: block; padding: 5px;">Amarillo</span>
     * <span style="font-weight: bold; background-color: black; color: #FF8000; display: block; padding: 5px;">Naranja</span>
     * <span style="font-weight: bold; background-color: black; color: #FF0000; display: block; padding: 5px;">Rojo</span>
     *
     * @param color Color del mensaje.
     * @param fmt Cuerpo, texto o mensaje del diálogo. Como si se usase "printf" de *C*.
     */
    void console_custom_msg(colorf_t color, const char* fmt, ...);

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
 * Se debe usar como única estancia de dicho sistema. Para mayor comodidad, use el objeto gDebug,
 * que es una referencia a este objeto.
 */
extern CSystem_Debug gSystem_Debug;
/**
 * @brief Objeto global del sistema "Debug".
 *
 * Se debe usar como única estancia de dicho sistema.
 */
extern CSystem_Debug& gDebug;

/*@}*/

#endif /* DEBUG_H_ */
