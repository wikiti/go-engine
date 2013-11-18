#include "systems/_debug.h"
#include "systems/_resource.h"
#include "systems/_render.h"
#include "systems/_data.h"
#include "engine/_engine.h"

CSystem_Debug gSystem_Debug;

void CSystem_Debug::ParseInput()
{
  stringstream ss(input);

  string comando, arguments;
  ss >> comando;
  getline(ss, arguments);
  if(arguments.size() > 2) arguments = arguments.substr(1);

  map<string, command_p>::iterator it = console_commands.find(comando);
  console_msg("> %s %s", comando.c_str(), arguments.c_str());
  if(it != console_commands.end())
  {
    (this->*(it->second))(arguments);
  }
  else
    Console_command__UNKNOWN_COMMAND(arguments);
}

bool CSystem_Debug::Init()
{
  CSystem::Init();

  file = fopen (__CSYSTEM_DEBUG_STORAGE_SAVEFILE, __CSYSTEM_DEBUG_STORAGE_ACCESS);//file = fopen ("log.txt","w");
  if(!file)
    return false;

  if(!InitCommandMap())
    return false;

  console_msg("Use \"help\" or \"?\" to list all comands.");
  current_line_buffered = current_last_command = console_pointer_pos = 0;
  console = false;


  input = "";

  opened = true;

  return true;
}

bool CSystem_Debug::InitConsoleFont()
{
  if(!gSystem_Resources.LoadResource(__CSYSTEM_DEBUG_CONSOLE_FONT, __CSYSTEM_DEBUG_CONSOLE_FONT_FILE, resources::texture, "mipmap"))
  {
    return false;
  }

  base = glGenLists(256);
  glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture(__CSYSTEM_DEBUG_CONSOLE_FONT)->GetID());

  for (int loop1=0; loop1<256; loop1++)                        // Loop Through All 256 Lists
  {
    float cx=float(loop1%16)/16.0f;                     // X Position Of Current Character
    float cy=float(loop1/16)/16.0f;                     // Y Position Of Current Character

    glNewList(base+loop1,GL_COMPILE);
      glBegin(GL_QUADS);
        glTexCoord2f(cx,1.0f-cy-0.001f); glVertex2d(0,16);
        glTexCoord2f(cx+0.0625f, 1.0f-cy-0.001f); glVertex2i(16,16);
        glTexCoord2f(cx+0.0625f, 1.0f-cy-0.0625f); glVertex2i(16,0);
        glTexCoord2f(cx,1.0f-cy-0.0625f); glVertex2i(0,0);
      glEnd();
      glTranslated(10,0,0);
    glEndList();
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  //gSystem_Textures.DeleteTexture(nombre);
  return true;
}

bool CSystem_Debug::InitCommandMap()
{
    // General
  console_commands.insert(pair<string, command_p>("help", &CSystem_Debug::Console_command__HELP));
  console_commands.insert(pair<string, command_p>("?", &CSystem_Debug::Console_command__HELP));
  console_commands.insert(pair<string, command_p>("log", &CSystem_Debug::Console_command__LOG));

  console_commands.insert(pair<string, command_p>("get_int", &CSystem_Debug::Console_command__GET_INT));
  console_commands.insert(pair<string, command_p>("get_float", &CSystem_Debug::Console_command__GET_FLOAT));
  console_commands.insert(pair<string, command_p>("get_string", &CSystem_Debug::Console_command__GET_STRING));

  console_commands.insert(pair<string, command_p>("set_int", &CSystem_Debug::Console_command__SET_INT));
  console_commands.insert(pair<string, command_p>("set_float", &CSystem_Debug::Console_command__SET_FLOAT));
  console_commands.insert(pair<string, command_p>("set_string", &CSystem_Debug::Console_command__SET_STRING));

  console_commands.insert(pair<string, command_p>("remove_int", &CSystem_Debug::Console_command__REMOVE_INT));
  console_commands.insert(pair<string, command_p>("remove_float", &CSystem_Debug::Console_command__REMOVE_FLOAT));
  console_commands.insert(pair<string, command_p>("remove_string", &CSystem_Debug::Console_command__REMOVE_STRING));
  console_commands.insert(pair<string, command_p>("remove_user_vars", &CSystem_Debug::Console_command__REMOVE_USER_VARS));

  console_commands.insert(pair<string, command_p>("exit", &CSystem_Debug::Console_command__EXIT));
  console_commands.insert(pair<string, command_p>("quit", &CSystem_Debug::Console_command__QUIT));
  console_commands.insert(pair<string, command_p>("clear", &CSystem_Debug::Console_command__CLEAR));

  console_commands.insert(pair<string, command_p>("secret_plz", &CSystem_Debug::Console_command__SECRET_PLZ));

    // Useless
  console_commands.insert(pair<string, command_p>("save_state", &CSystem_Debug::Console_command__SAVE_STATE));
  console_commands.insert(pair<string, command_p>("load_state", &CSystem_Debug::Console_command__LOAD_STATE));

    // Render
  console_commands.insert(pair<string, command_p>("r_update_window", &CSystem_Debug::Console_command__R_UPDATE_WINDOW));
  console_commands.insert(pair<string, command_p>("r_resize_window", &CSystem_Debug::Console_command__R_RESIZE_WINDOW));
  console_commands.insert(pair<string, command_p>("r_draw_transform", &CSystem_Debug::Console_command__R_DRAW_TRANSFORM));
  console_commands.insert(pair<string, command_p>("r_draw_grid", &CSystem_Debug::Console_command__R_DRAW_GRID));
  console_commands.insert(pair<string, command_p>("r_fps", &CSystem_Debug::Console_command__R_FPS));

  return true;
}

void CSystem_Debug::Close()
{
  CSystem::Close();

  fclose(file);
  opened = false;

  gSystem_Resources.ClearResource(__CSYSTEM_DEBUG_CONSOLE_FONT);
  glDeleteLists(base,256);
  base = 0;
}

void CSystem_Debug::log(const char* fmt, ...)
{
  if(!file)
    return;

  va_list ap;

  time_t rawtime;
  struct tm * timeinfo;
  char buffer [80];

  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"[%d/%m/%Y %H:%M] ",timeinfo);

  fprintf(file, buffer);
  va_start(ap, fmt);
    vfprintf(file, fmt, ap);
  va_end(ap);
  fprintf(file, "\n");
}

void CSystem_Debug::error(const char* fmt, ...)
{
  if(!file)
    return;

  va_list ap;

  time_t rawtime;
  struct tm * timeinfo;
  char buffer [256];

  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80,"[%d/%m/%Y %H:%M] **ERROR** ",timeinfo);

  fprintf(file, buffer);
  va_start(ap, fmt);
    vfprintf(file, fmt, ap);
  va_end(ap);
  fprintf(file, "\n");
}

void CSystem_Debug::raw_log(const char* fmt, ...)
{
  if(!file)
    return;

  va_list ap;
  va_start(ap, fmt);
    vfprintf(file, fmt, ap);
  va_end(ap);
}

void CSystem_Debug::msg_box(const char* title, const char* message, Uint32 flags)
{
  SDL_ShowSimpleMessageBox(flags, title, message, gSystem_Render.window);
}

void CSystem_Debug::msg_box(Uint32 flags, const char* title, const char* fmt, ...)
{
  if(!fmt)
    return;

  char texto[1024];

  va_list ap;
  va_start(ap, fmt);
    vsprintf(texto, fmt, ap);
  va_end(ap);

  SDL_ShowSimpleMessageBox(flags, title, texto, gSystem_Render.window);
}

void CSystem_Debug::OnEvent()
{
  if(console)
  {
    if(event.type == SDL_KEYDOWN)
    {
      if(event.key.keysym.sym == SDLK_F10)
      {
        console = false;
        SDL_StopTextInput();
      }
      if(event.key.keysym.sym == SDLK_ESCAPE)
      {
        console_pointer_pos = 0;
        if(input == "")
        {
          console = false;
          SDL_StopTextInput();
        }
        else
          input = "";
      }
      else if(event.key.keysym.sym == SDLK_RETURN)
      {
        if(input != "")
        {
          command_buffer.push_back(input);
          current_last_command = command_buffer.size();
          ParseInput();
          input = "";
          console_pointer_pos = 0;
        }
      }
      else if(event.key.keysym.sym == SDLK_DELETE && input.length())
      {
        if(console_pointer_pos < input.length())
        {
           input.erase(console_pointer_pos, 1);
           //if(console_pointer_pos == input.length() && console_pointer_pos > 0)
             //console_pointer_pos--;
        }
      }
      else if(event.key.keysym.sym == SDLK_BACKSPACE && input.length())
      {
        if(console_pointer_pos > 0)
        {
          input.erase(console_pointer_pos-1, 1);
          console_pointer_pos--;
        }
      }
      else if(event.key.keysym.sym == SDLK_UP)
      {
        if(command_buffer.size() && current_last_command > 0)
        {
          current_last_command--;
          input = command_buffer[current_last_command];
          console_pointer_pos = input.size();
        }
      }
      else if(event.key.keysym.sym == SDLK_DOWN)
      {
        if(command_buffer.size() && current_last_command < command_buffer.size()-1)
        {
          current_last_command++;
          input = command_buffer[current_last_command];
          console_pointer_pos = input.size();
        }
      }
      else if(event.key.keysym.sym == SDLK_LEFT)
      {
        if(console_pointer_pos > 0)
          console_pointer_pos--;
      }
      else if(event.key.keysym.sym == SDLK_RIGHT)
      {
          if(console_pointer_pos < input.length())
            console_pointer_pos++;
      }
      else if(event.key.keysym.sym == SDLK_PAGEUP)
      {
        if(console_buffer.size() && current_line_buffered < console_buffer.size()-1)
        {
          current_line_buffered++;
        }
      }
      else if(event.key.keysym.sym == SDLK_PAGEDOWN)
      {
        if(console_buffer.size() && current_line_buffered > 0)
        {
          current_line_buffered--;
        }
      }
    }
    else if(event.type == SDL_MOUSEBUTTONDOWN)
    {
      if( event.button.button == SDL_BUTTON_RIGHT )
      {
        //input += SDL_GetClipboardText();
        input.insert(console_pointer_pos, SDL_GetClipboardText());
        console_pointer_pos += strlen(SDL_GetClipboardText());
      }
    }
    else if(event.type == SDL_TEXTINPUT)
    {
      input.insert(console_pointer_pos, event.text.text);
      console_pointer_pos++;
    }
  }
  else
  {
    if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F10)
    {
      console = true;
      SDL_StartTextInput();
    }
  }
}

void CSystem_Debug::OnRender()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH"), 0, __CSYSTEM_DEBUG_CONSOLE_SIZE, -1, 1);

  glViewport(0, gSystem_Data_Storage.GetInt("__RESOLUTION_HEIGHT") - __CSYSTEM_DEBUG_CONSOLE_SIZE, gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH"), __CSYSTEM_DEBUG_CONSOLE_SIZE);
  glScissor(0, gSystem_Data_Storage.GetInt("__RESOLUTION_HEIGHT") - __CSYSTEM_DEBUG_CONSOLE_SIZE, gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH"), __CSYSTEM_DEBUG_CONSOLE_SIZE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  glEnable(GL_BLEND);

  glColor3f(1.f, 1.f, 1.f);

  // Show input
  print(__CSYSTEM_DEBUG_CONSOLE_X_OFFSET, 10, 0, ("> " + input).c_str());

  // Show console buffer
  //int line = 0;
  for(uint i = current_line_buffered, line = 0; i < __CSYSTEM_DEBUG_CONSOLE_LINES + current_line_buffered && i < console_buffer.size(); i++, line++)
  {
    string_console_t& s = console_buffer[i];
    glColor4f(s.line_color.r, s.line_color.g, s.line_color.b, s.line_color.a);
    print(__CSYSTEM_DEBUG_CONSOLE_X_OFFSET, line*__CSYSTEM_DEBUG_CONSOLE_LINESPACE + __CSYSTEM_DEBUG_CONSOLE_Y_OFFSET, 0, s.str.c_str());
  }

  // Draw console pointer
  glColor4f(0.75f, 0.75f, 0.75f, 1.f);
  print(__CSYSTEM_DEBUG_CONSOLE_X_OFFSET*3 + console_pointer_pos*10, 6, 0, "_");

  glDisable(GL_BLEND);
}

void CSystem_Debug::print(GLint x, GLint y, int set, const char* fmt, ...)
{
  char text[1024];
  va_list ap;

  if(fmt == NULL)
    return;

  va_start(ap, fmt);
    vsprintf(text, fmt, ap);
  va_end(ap);

  if(set > 1 || set < 0)
    set = 1;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glTranslated(x, y, 0);
  glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture(__CSYSTEM_DEBUG_CONSOLE_FONT)->GetID());
  glListBase(base-32 + (128)*set);

  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);

  glPopMatrix();
}

void CSystem_Debug::console_msg(const char* fmt, ...)
{
  if(!fmt)
    return;

  char text[1024];

  va_list ap;
  va_start(ap, fmt);
    vsprintf(text, fmt, ap);
  va_end(ap);

  string_console_t t(text, 1.f, 1.f, 1.f, 1.f);
  console_buffer.insert(console_buffer.begin(), t);
}

void CSystem_Debug::console_error_msg(const char* fmt, ...)
{
  if(!fmt)
    return;

  char text[1024];

  va_list ap;
  va_start(ap, fmt);
    vsprintf(text, fmt, ap);
  va_end(ap);

  string_console_t t(text, 1.f, 0.f, 0.f, 1.f);
  console_buffer.insert(console_buffer.begin(), t);
}

void CSystem_Debug::console_warning_msg(const char* fmt, ...)
{
  if(!fmt)
    return;

  char text[1024];

  va_list ap;
  va_start(ap, fmt);
    vsprintf(text, fmt, ap);
  va_end(ap);

  string_console_t t(text, 1.f, 0.75f, 0.f, 1.f);
  console_buffer.insert(console_buffer.begin(), t);
}

void CSystem_Debug::console_custom_msg(GLfloat r, GLfloat g, GLfloat b, GLfloat a, const char* fmt, ...)
{
  if(!fmt)
    return;

  char text[1024];

  va_list ap;
  va_start(ap, fmt);
    vsprintf(text, fmt, ap);
  va_end(ap);

  string_console_t t(text, r, g, b, a);
  console_buffer.insert(console_buffer.begin(), t);
}


/** Console commands **/

void CSystem_Debug::Console_command__UNKNOWN_COMMAND(string arguments)
{
  console_error_msg("Unkown command: %s", input.c_str());
}

void CSystem_Debug::Console_command__HELP(string arguments)
{
  if(arguments == "")
  {
    console_msg("Use \"help <category>\". Categories are:");
    console_warning_msg("general, render");
  }
  else if(arguments == "general")
  {
    console_msg("General commands:");
    console_msg("  bla1");
    console_msg("  bla2");
  }
  else if(arguments == "render")
  {
    console_msg("Render commands:");
    console_msg("  bla1");
    console_msg("  bla2");
  }
  // ...
  else
  {
    console_error_msg("Unknown help category: %s", arguments.c_str());
  }
}

void CSystem_Debug::Console_command__LOG(string arguments)
{
  if(!console_buffer.size())
    return;

  console_msg("Wrote console buffer to file \"%s\"", __CSYSTEM_DEBUG_STORAGE_SAVEFILE);

  raw_log("\n---------------------------------- CONSOLE BEGIN ----------------------------------\n\n");

  //    vector<string_console_t> console_buffer;
  for(vector<string_console_t>::reverse_iterator it = console_buffer.rbegin(); it != console_buffer.rend(); it++)
  {
    raw_log("%s\n", it->str.c_str());
  }

  raw_log("\n----------------------------------- CONSOLE END -----------------------------------\n\n");
}

void CSystem_Debug::Console_command__SET_INT(string arguments)
{
  // ¿Restricciones de variables del sistema? hmmm

  stringstream ss(arguments);
  string val_name, val;
  ss >> val_name >> val;

  if(val_name != "")
  {
    int value = atoi(val.c_str());

    console_msg("%s = %i", val_name.c_str(), value);
    gSystem_Data_Storage.SetInt(val_name, value);
  }
  else
    console_warning_msg("Format is: set_int <identifier> [value]");
}

void CSystem_Debug::Console_command__SET_FLOAT(string arguments)
{
  stringstream ss(arguments);
  string val_name, val;
  ss >> val_name >> val;

  if(val_name != "")
  {
    float value = atof(val.c_str());

    console_msg("%s = %f", val_name.c_str(), value);
    gSystem_Data_Storage.SetFloat(val_name, value);
  }
  else
    console_warning_msg("Format is: set_float <identifier> [value]");
}

void CSystem_Debug::Console_command__SET_STRING(string arguments)
{
  stringstream ss(arguments);
  string val_name, val;
  ss >> val_name;

  if(val_name != "")
  {
    getline(ss, val);
    if(val.size() > 2) val = val.substr(1);

    console_msg("%s = %s", val_name.c_str(), val.c_str());
    gSystem_Data_Storage.SetString(val_name, val);
  }
  else
    console_warning_msg("Format is: set_string <identifier> [value]");
}

void CSystem_Debug::Console_command__GET_INT(string arguments)
{
  stringstream ss(arguments);
  string val_name;
  ss >> val_name;

  if(val_name != "")
  {
    int val = gSystem_Data_Storage.GetInt(val_name);

    if(val == __NO_INT)
      console_error_msg("No int saved in \"%s\"", val_name.c_str());
    else
      console_msg("%s = %i", val_name.c_str(), val);
  }
  else
  {
    //console_warning_msg("Format is: get_int <identifier>");
    for(map<string, int>::iterator it = gSystem_Data_Storage.ints.begin(); it != gSystem_Data_Storage.ints.end(); it++)
      console_msg("%s = %i", it->first.c_str(), it->second);
  }
}

void CSystem_Debug::Console_command__GET_FLOAT(string arguments)
{
  stringstream ss(arguments);
  string val_name;
  ss >> val_name;

  if(val_name != "")
  {
    float val = gSystem_Data_Storage.GetFloat(val_name);

    if(val == __NO_FLOAT)
      console_error_msg("No float saved in \"%s\"", val_name.c_str());
    else
      console_msg("%s = %f", val_name.c_str(), val);
  }
  else
  {
    //console_warning_msg("Format is: get_float <identifier>");
    for(map<string, float>::iterator it = gSystem_Data_Storage.floats.begin(); it != gSystem_Data_Storage.floats.end(); it++)
      console_msg("%s = %f", it->first.c_str(), it->second);
  }
}

void CSystem_Debug::Console_command__GET_STRING(string arguments)
{
  stringstream ss(arguments);
  string val_name;
  ss >> val_name;

  if(val_name != "")
  {
    string val = gSystem_Data_Storage.GetString(val_name);

    if(val == __NO_STRING)
      console_error_msg("No string saved in \"%s\"", val_name.c_str());
    else
      console_msg("%s = \"%s\"", val_name.c_str(), val.c_str());
  }
  else
  {
    //console_warning_msg("Format is: get_string <identifier>");
    for(map<string, string>::iterator it = gSystem_Data_Storage.strings.begin(); it != gSystem_Data_Storage.strings.end(); it++)
      console_msg("%s = \"%s\"", it->first.c_str(), it->second.c_str());
  }
}

void CSystem_Debug::Console_command__REMOVE_INT(string arguments)
{
  stringstream ss(arguments);
  string val_name;
  ss >> val_name;

  if(val_name != "")
  {
    if(val_name[0] == '_' and val_name[1] == '_')
    {
      console_error_msg("Can't remove system vars (starting with \"__\") \"%s\"", val_name.c_str());
      return;
    }

    if(!gSystem_Data_Storage.RemoveInt(val_name))
    {
      console_error_msg("No int saved in \"%s\"", val_name.c_str());
    }
    else
      console_warning_msg("Deleted int variable \"%s\"", val_name.c_str());
  }
  else
    console_warning_msg("Format is: remove_int <identifier>");
}

void CSystem_Debug::Console_command__REMOVE_FLOAT(string arguments)
{
  stringstream ss(arguments);
  string val_name;
  ss >> val_name;

  if(val_name != "")
  {
    if(val_name[0] == '_' and val_name[1] == '_')
    {
      console_error_msg("Can't remove system vars (starting with \"__\") \"%s\"", val_name.c_str());
      return;
    }

    if(!gSystem_Data_Storage.RemoveFloat(val_name))
    {
      console_error_msg("No float saved in \"%s\"", val_name.c_str());
    }
    else
      console_warning_msg("Deleted float variable \"%s\"", val_name.c_str());
  }
  else
    console_warning_msg("Format is: remove_float <identifier>");
}

void CSystem_Debug::Console_command__REMOVE_STRING(string arguments)
{
  stringstream ss(arguments);
  string val_name;
  ss >> val_name;

  if(val_name != "")
  {
    if(val_name[0] == '_' and val_name[1] == '_')
    {
      console_error_msg("Can't remove system vars (starting with \"__\") \"%s\"", val_name.c_str());
      return;
    }

    if(!gSystem_Data_Storage.RemoveString(val_name))
    {
      console_error_msg("No string saved in \"%s\"", val_name.c_str());
    }
    else
      console_warning_msg("Deleted string variable \"%s\"", val_name.c_str());
  }
  else
    console_warning_msg("Format is: remove_string <identifier>");
}

void CSystem_Debug::Console_command__REMOVE_USER_VARS(string arguments)
{
  console_warning_msg("Deleted all user variables");
  gSystem_Data_Storage.RemoveUserVars();
}

void CSystem_Debug::Console_command__CLEAR(string arguments)
{
  // No se limpia el buffer de comandos (por comodidad, básicamente); solo el de resultados.
  console_buffer.clear();
  current_line_buffered = 0;
}

void CSystem_Debug::Console_command__EXIT(string arguments)
{
  gEngine.Exit();
}

void CSystem_Debug::Console_command__QUIT(string arguments)
{
  gSystem_Debug.log("Quit: \"%s\"", arguments.c_str());
  gEngine.Quit();
}

void CSystem_Debug::Console_command__SAVE_STATE(string arguments)
{
  gSystem_GameObject_Manager.SaveGameObjects(arguments);
}

void CSystem_Debug::Console_command__LOAD_STATE(string arguments)
{
  gSystem_GameObject_Manager.LoadGameObjects(arguments);
}

void CSystem_Debug::Console_command__SECRET_PLZ(string arguments)
{
  console_custom_msg(0.7f, 0.9f, 0.1f, 1.f, "      ________                 ___________              .__");
  console_custom_msg(0.7f, 0.9f, 0.1f, 1.f, "     /  _____/  ____           \\_   _____/ ____    ____ |__| ____   ____");
  console_custom_msg(0.7f, 0.9f, 0.1f, 1.f, "    /   \\  ___ /  _ \\   ______  |    __)_ /    \\  / ___\\|  |/    \\_/ __ \\");
  console_custom_msg(0.7f, 0.9f, 0.1f, 1.f, "    \\    \\_\\  (  <_> ) /_____/  |        \\   |  \\/ /_/  >  |   |  \\  ___/");
  console_custom_msg(0.7f, 0.9f, 0.1f, 1.f, "     \\______  /\\____/          /_______  /___|  /\\___  /|__|___|  /\\___  >");
  console_custom_msg(0.7f, 0.9f, 0.1f, 1.f, "            \\/                         \\/     \\//_____/         \\/     \\/");
}

void CSystem_Debug::Console_command__R_UPDATE_WINDOW(string arguments)
{
  if(gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH") <= 0 or gSystem_Data_Storage.GetInt("__RESOLUTION_HEIGHT") <= 0 )
    console_error_msg("Invalid values of __RESOLUTION_WIDTH (%i) or __RESOLUTION_HEIGHT (%i)", gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH"), gSystem_Data_Storage.GetInt("__RESOLUTION_HEIGHT"));
  else
  {
    gSystem_Render.ResizeWindow(gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH"), gSystem_Data_Storage.GetInt("__RESOLUTION_HEIGHT"));
    console_msg("Set window to resolution %i x %i", gSystem_Data_Storage.GetInt("__RESOLUTION_WIDTH"), gSystem_Data_Storage.GetInt("__RESOLUTION_HEIGHT"));
  }
}

void CSystem_Debug::Console_command__R_RESIZE_WINDOW(string arguments)
{
  stringstream ss(arguments);
  int w = 0,
      h = 0;

  ss >> w >> h;
  if(w <= 0 or h <= 0)
  {
    console_error_msg("Invalid values of width (%i) or height (%i)", w, h);
    console_warning_msg("Format is: r_resize_window <width> <height>");
  }
  else
  {
    gSystem_Data_Storage.SetInt("__RESOLUTION_WIDTH", w);
    gSystem_Data_Storage.SetInt("__RESOLUTION_HEIGHT", h);
    Console_command__R_UPDATE_WINDOW(arguments);
  }
}

void CSystem_Debug::Console_command__R_DRAW_TRANSFORM(string arguments)
{
  stringstream ss(arguments);
  int val = -1;
  ss >> val;

  if(val < 0 or val > 1)
    console_warning_msg("Format is: r_draw_transform <0 | 1>");
  else
  {
    gSystem_Data_Storage.SetInt("__RENDER_TRANSFORM", val);
  }
}

void CSystem_Debug::Console_command__R_DRAW_GRID(string arguments)
{
  stringstream ss(arguments);
  int val = -1;
  ss >> val;

  if(val < 0 or val > 1)
    console_warning_msg("Format is: r_draw_transform_grid <0 | 1>");
  else
  {
    gSystem_Data_Storage.SetInt("__RENDER_TRANSFORM_GRID", val);
  }
}

void CSystem_Debug::Console_command__R_FPS(string arguments)
{
  console_msg("FPS: %f", gEngine.fps());
}
