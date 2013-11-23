#ifndef DEBUG_H_
#define DEBUG_H_

#define __CSYSTEM_DEBUG_STORAGE_SAVEFILE "log.txt"
#define __CSYSTEM_DEBUG_STORAGE_ACCESS "a"
#define __CSYSTEM_DEBUG_CONSOLE_FONT "__CONSOLE_FONT"
#define __CSYSTEM_DEBUG_CONSOLE_FONT_FILE "data/engine/console_font.tga"

#define __CSYSTEM_DEBUG_KEYWORD_ALL "$ALL"

#define __CSYSTEM_DEBUG_CONSOLE_SIZE 200
#define __CSYSTEM_DEBUG_CONSOLE_LINES 11
#define __CSYSTEM_DEBUG_CONSOLE_LINESPACE 16
#define __CSYSTEM_DEBUG_CONSOLE_Y_OFFSET 30
#define __CSYSTEM_DEBUG_CONSOLE_X_OFFSET 10

#include <cstdarg>
#include <stdio.h>
#include <time.h>

#include "_globals.h"
#include "_object.h"
#include "systems/_system.h"

typedef struct string_console_t
{
  colorf_t line_color;
  string str;

  string_console_t(): str("")
  {
    line_color.r = line_color.g = line_color.b = line_color.a = 1.f;
  }

  string_console_t(string msg, GLfloat r = 1.f, GLfloat g = 1.f, GLfloat b = 1.f, GLfloat a = 1.f): str(msg)
  {
    line_color.r = r;
    line_color.g = g;
    line_color.b = b;
    line_color.a = a;
  }

} string_console_t;

namespace debug
{
  enum flags_t {error = SDL_MESSAGEBOX_ERROR, warning = SDL_MESSAGEBOX_WARNING, information = SDL_MESSAGEBOX_INFORMATION};
}

class CSystem_Debug: public CSystem
{
  private:
    friend class CSystem_Render;

  protected:
    // Debug
    bool opened;
    FILE* file;

    // Console
    vector<string_console_t> console_buffer;
    vector<string> command_buffer;

    uint current_line_buffered;
    uint current_last_command;

    typedef void (CSystem_Debug::*command_p)(string);
    map<string, command_p> console_commands;

    bool console;
    string input;
    void ParseInput();

    // Console Font
    GLuint base;
    void print(GLint x, GLint y, int set, const char* fmt, ...);

    // Puntero de consola
    uint console_pointer_pos;

  public:
    CSystem_Debug(): opened(false), file(NULL){}
    bool IsOpened(){return opened;}

    bool Init();
      bool InitConsoleFont();
      bool InitCommandMap();

    void Close();

    bool IsConsole()
    {
      return console;
    }

    // log.txt File
    void log(const char* fmt, ...);
    void error(const char* fmt, ...);
    void raw_log(const char* fmt, ...);

    // Message Boxes
    // Flags: debug::error for error, debug::warning for warnings, debug::information for info
    void msg_box(const char* title, const char* message, Uint32 flags = debug::error);
    void msg_box(Uint32 flags, const char* title, const char* fmt, ...);

    // Console
    void console_msg(const char* fmt, ...);
    void console_error_msg(const char* fmt, ...);
    void console_warning_msg(const char* fmt, ...);
    void console_custom_msg(GLfloat r, GLfloat g, GLfloat b, GLfloat a, const char* fmt, ...);

    void OnEvent();
    void OnRender();

  protected:
      // Console commands
    // General
    void Console_command__UNKNOWN_COMMAND(string arguments);
    void Console_command__HELP(string arguments);
    void Console_command__LOG(string arguments);

    void Console_command__SET_INT(string arguments);
    void Console_command__SET_FLOAT(string arguments);
    void Console_command__SET_STRING(string arguments);

    void Console_command__GET_INT(string arguments);
    void Console_command__GET_FLOAT(string arguments);
    void Console_command__GET_STRING(string arguments);

    void Console_command__REMOVE_INT(string arguments);
    void Console_command__REMOVE_FLOAT(string arguments);
    void Console_command__REMOVE_STRING(string arguments);
    void Console_command__REMOVE_USER_VARS(string arguments);

    void Console_command__QUIT(string arguments);
    void Console_command__CLEAR(string arguments);
    void Console_command__EXIT(string arguments);

    void Console_command__SECRET_PLZ(string arguments);

    void Console_command__SAVE_STATE(string arguments);
    void Console_command__LOAD_STATE(string arguments);

    // Game Objects
    void Console_command__GAME_OBJECT_SHOW_TREE(string arguments);
      void Console_command__AUX__GAME_OBJECT_SHOW_TREE_print_element(CGameObject* go, map<string, void*>& list, int level = 1);
    void Console_command__GAME_OBJECT_ENABLE(string arguments);
    void Console_command__GAME_OBJECT_COMPONENT_ENABLE(string arguments);
    //void Console_command__ENABLE_SYSTEM(string arguments);
    //

    // Render
    void Console_command__R_UPDATE_WINDOW(string arguments);
    void Console_command__R_RESIZE_WINDOW(string arguments);
    void Console_command__R_DRAW_TRANSFORM(string arguments);
    void Console_command__R_DRAW_GRID(string arguments);
    void Console_command__R_FPS(string arguments);
};

extern CSystem_Debug gSystem_Debug;
//extern CSystem_Debug& gDebug;



#endif /* DEBUG_H_ */
