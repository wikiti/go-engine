#include "systems/_debug.h"
#include "systems/_resource.h"
#include "systems/_manager.h"
#include "systems/_render.h"
#include "systems/_data.h"
#include "systems/_other.h"
#include "systems/_mixer.h"
#include "systems/_shader.h"
#include "systems/_input.h"

#include "engine/_engine.h"

using namespace std;

CSystem_Debug gSystem_Debug;
CSystem_Debug& gDebug = gSystem_Debug;

using namespace Debug;

void CSystem_Debug::ParseInput()
{
  stringstream ss(input);

  string command, arguments;
  ss >> command;

  getline(ss, arguments);
  if(arguments.size() > 1) arguments = arguments.substr(1);

  command = Utils::string_to_lower(command);
  map<string, command_p>::iterator it = console_commands.find(command);
  console_msg("> %s %s", command.c_str(), arguments.c_str());

  if(command[0] == '#') // ignore comments
    return;

  if(it != console_commands.end())
  {
    (this->*(it->second))(arguments);
  }
  else
    Console_command__UNKNOWN_COMMAND(command);
}

void CSystem_Debug::ParseAppArguments()
{
  vector<string>& args = gEngine.Arguments();
  for(vector<string>::iterator it = args.begin(); it != args.end(); ++it)
  {
    // Command case
    if((*it)[0] == '-')
    {
      input = (*it);
      input = input.substr(1);
      if(input.size())
      {
        //replace(input.begin(), input.end(), '#', ' ');

        ParseInput();
        input = "";
      }
    }
    // Other cases here
  }
  console_msg("-----------------------------------------------");
}

bool CSystem_Debug::Init()
{
  if(enabled) return true;

  //file = fopen (__CSYSTEM_DEBUG_STORAGE_SAVEFILE, __CSYSTEM_DEBUG_STORAGE_ACCESS);//file = fopen ("log.txt","w");
  //if(!file)
  //  return false;

  if(!InitCommandMap())
    return false;

  //Console_command__RUN("auto");
  command("run auto");

  console_msg("Use \"help\" or \"?\" to list all commands.");
  current_line_buffered = current_last_command = console_pointer_pos = 0;
  console = false;

  input = "";

  opened = true;

  CSystem::Init();
  return true;
}

bool CSystem_Debug::Reset()
{
  /*if(file)
    fclose(file);

  file = fopen (__CSYSTEM_DEBUG_STORAGE_SAVEFILE, __CSYSTEM_DEBUG_STORAGE_ACCESS);//file = fopen ("log.txt","w");
  if(!file)
    return false;*/

  return true;
}

bool CSystem_Debug::InitConsoleFont()
{
  if(!gSystem_Resources.LoadResource(__CSYSTEM_DEBUG_CONSOLE_FONT, __CSYSTEM_DEBUG_CONSOLE_FONT_FILE, resources::texture, "mipmap"))
  {
    error("From Debug: Could not file file \"%s\"", __CSYSTEM_DEBUG_CONSOLE_FONT_FILE);
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

  return true;

  // Posible problema: vec2 a vec3
  /*const GLfloat consoleFont_vertices[][2]
  {
    { 0.f, 16.f}, {16.f, 16.0f},
    {16.f,  0.f}, { 0.f,  0.0f}
  };

  const GLfloat consoleFont_texCoords[][2]
  {
    // problema, tal vez estén mal puestos
    {    0.f,  0.999f}, {0.0625f,  0.999f},
    {0.0625f, 0.9375f}, {   0.0f, 0.9375f}
  };

  glGenVertexArrays(1, &m_ConsoleFontVAO);
  if(!m_ConsoleFontVAO)
  {
    gSystem_Debug.error("From Debug: Could not generate ConsoleFont VAO.");
    return false;
  }

  glGenBuffers( 1, &m_ConsoleFontVBOVertices );
  glGenBuffers( 1, &m_ConsoleFontVBOTexCoords );
  glGenBuffers( 1, &m_ConsoleFontVBOTypeInfo ); // Este se actualiza automáticamente cada vez que se llama a print().

  if(!m_ConsoleFontVBOVertices or !m_ConsoleFontVBOTexCoords or !m_ConsoleFontVBOTypeInfo)
  {
    gSystem_Debug.error("From Debug: Could not generate ConsoleFont VBO.");
    return false;
  }

  glBindVertexArray(m_ConsoleFontVAO);

  glBindBuffer( GL_ARRAY_BUFFER, m_ConsoleFontVBOVertices );
  glBufferData( GL_ARRAY_BUFFER, 4*2*sizeof(GLfloat), consoleFont_vertices, GL_STATIC_DRAW );
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer( GL_ARRAY_BUFFER, m_ConsoleFontVBOTexCoords );
  glBufferData( GL_ARRAY_BUFFER, 4*2*sizeof(GLfloat), consoleFont_texCoords, GL_STATIC_DRAW );
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);*/
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
  console_commands.insert(pair<string, command_p>("get_var", &CSystem_Debug::Console_command__GET_VAR));

  console_commands.insert(pair<string, command_p>("set_int", &CSystem_Debug::Console_command__SET_INT));
  console_commands.insert(pair<string, command_p>("set_float", &CSystem_Debug::Console_command__SET_FLOAT));
  console_commands.insert(pair<string, command_p>("set_string", &CSystem_Debug::Console_command__SET_STRING));

  console_commands.insert(pair<string, command_p>("remove_int", &CSystem_Debug::Console_command__REMOVE_INT));
  console_commands.insert(pair<string, command_p>("remove_float", &CSystem_Debug::Console_command__REMOVE_FLOAT));
  console_commands.insert(pair<string, command_p>("remove_string", &CSystem_Debug::Console_command__REMOVE_STRING));
  console_commands.insert(pair<string, command_p>("remove_user_vars", &CSystem_Debug::Console_command__REMOVE_USER_VARS));

  console_commands.insert(pair<string, command_p>("abort", &CSystem_Debug::Console_command__ABORT));
  console_commands.insert(pair<string, command_p>("exit", &CSystem_Debug::Console_command__EXIT));
  console_commands.insert(pair<string, command_p>("clear", &CSystem_Debug::Console_command__CLEAR));

  console_commands.insert(pair<string, command_p>("run", &CSystem_Debug::Console_command__RUN));

  console_commands.insert(pair<string, command_p>("secret_plz", &CSystem_Debug::Console_command__SECRET_PLZ));

    // Useless
  //console_commands.insert(pair<string, command_p>("save_state", &CSystem_Debug::Console_command__SAVE_STATE));
  //console_commands.insert(pair<string, command_p>("load_state", &CSystem_Debug::Console_command__LOAD_STATE));

    // Systems
  console_commands.insert(pair<string, command_p>("system_time_setscale", &CSystem_Debug::Console_command__SYSTEM_TIME_SETSCALE));
  console_commands.insert(pair<string, command_p>("system_userinput_show_joysticks", &CSystem_Debug::Console_command__SYSTEM_USERINPUT_SHOW_JOYSTICKS));

    // Game objects
  console_commands.insert(pair<string, command_p>("go_show_tree", &CSystem_Debug::Console_command__GO_SHOW_TREE));
  console_commands.insert(pair<string, command_p>("go_enable", &CSystem_Debug::Console_command__GO_ENABLE));
  console_commands.insert(pair<string, command_p>("go_component_enable", &CSystem_Debug::Console_command__GO_COMPONENT_ENABLE));
  console_commands.insert(pair<string, command_p>("go_component_set", &CSystem_Debug::Console_command__GO_COMPONENT_SET));
  console_commands.insert(pair<string, command_p>("go_component_get", &CSystem_Debug::Console_command__GO_COMPONENT_GET));
  console_commands.insert(pair<string, command_p>("go_search", &CSystem_Debug::Console_command__GO_SEARCH));

    // Sound
  console_commands.insert(pair<string, command_p>("snd_volume", &CSystem_Debug::Console_command__SND_VOLUME));
  console_commands.insert(pair<string, command_p>("snd_music_volume", &CSystem_Debug::Console_command__SND_MUSIC_VOLUME));
  console_commands.insert(pair<string, command_p>("snd_checksources", &CSystem_Debug::Console_command__SND_CHECKSOURCES));

    // Render
  console_commands.insert(pair<string, command_p>("r_update_window", &CSystem_Debug::Console_command__R_UPDATE_WINDOW));
  console_commands.insert(pair<string, command_p>("r_resize_window", &CSystem_Debug::Console_command__R_RESIZE_WINDOW));
  console_commands.insert(pair<string, command_p>("r_draw_transform", &CSystem_Debug::Console_command__R_DRAW_TRANSFORM));
  console_commands.insert(pair<string, command_p>("r_draw_grid", &CSystem_Debug::Console_command__R_DRAW_GRID));
  console_commands.insert(pair<string, command_p>("r_fps", &CSystem_Debug::Console_command__R_FPS));
  console_commands.insert(pair<string, command_p>("r_draw_sound", &CSystem_Debug::Console_command__R_DRAW_SOUND));
  console_commands.insert(pair<string, command_p>("r_glinfo", &CSystem_Debug::Console_command__R_GLINFO));

  return true;
}

void CSystem_Debug::Close()
{
  if(!enabled) return;
  CSystem::Close();

  //fclose(file);
  opened = false;

  gSystem_Resources.ClearResource(__CSYSTEM_DEBUG_CONSOLE_FONT);
  glDeleteLists(base,256);
  base = 0;

  /*glDeleteVertexArrays(1, &m_ConsoleFontVAO);
  glDeleteBuffers( 1, &m_ConsoleFontVBOVertices );
  glDeleteBuffers( 1, &m_ConsoleFontVBOTexCoords );
  glDeletenBuffers( 1, &m_ConsoleFontVBOTypeInfo );*/
}

void CSystem_Debug::command(const string& command, bool log)
{
  if(command != "")
  {
    if(log)
    {
      command_buffer.push_back(command);
      if(command_buffer.size() > __CSYSTEM_DEBUG_CONSOLE_COMMAND_BUFFER_MAXLINES) command_buffer.erase(command_buffer.begin()); // Check command_buffer size and clean if necesary
      current_last_command = command_buffer.size();
      console_pointer_pos = current_line_buffered = 0;
    }

    input = command;
    ParseInput();
    input = "";
  }
}

void CSystem_Debug::log(const char* fmt, ...)
{
  file = fopen (__CSYSTEM_DEBUG_STORAGE_SAVEFILE, __CSYSTEM_DEBUG_STORAGE_ACCESS);
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

  fclose(file);
}

void CSystem_Debug::error(const char* fmt, ...)
{
  file = fopen (__CSYSTEM_DEBUG_STORAGE_SAVEFILE, __CSYSTEM_DEBUG_STORAGE_ACCESS);
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

  fclose(file);
}

void CSystem_Debug::raw_log(const char* fmt, ...)
{
  file = fopen (__CSYSTEM_DEBUG_STORAGE_SAVEFILE, __CSYSTEM_DEBUG_STORAGE_ACCESS);
  if(!file)
    return;

  va_list ap;
  va_start(ap, fmt);
    vfprintf(file, fmt, ap);
  va_end(ap);

  fclose(file);
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
      if(event.key.keysym.scancode == SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_CONSOLE_KEY").c_str()))
      {
        console = false;
        SDL_StopTextInput();
      }
      else if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
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
      else if(event.key.keysym.scancode == SDL_SCANCODE_RETURN)
      {
        if(input != "")
        {
          command_buffer.push_back(input);
          if(command_buffer.size() > __CSYSTEM_DEBUG_CONSOLE_COMMAND_BUFFER_MAXLINES) command_buffer.erase(command_buffer.begin()); // Check command_buffer size and clean if necesary

          current_last_command = command_buffer.size();
          ParseInput();
          input = "";
          console_pointer_pos = current_line_buffered = 0;
        }
      }
      else if(event.key.keysym.scancode == SDL_SCANCODE_DELETE && input.length())
      {
        if(console_pointer_pos < input.length())
        {
           input.erase(console_pointer_pos, 1);
           //if(console_pointer_pos == input.length() && console_pointer_pos > 0)
             //console_pointer_pos--;
        }
      }
      else if(event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE && input.length())
      {
        if(console_pointer_pos > 0)
        {
          input.erase(console_pointer_pos-1, 1);
          console_pointer_pos--;
        }
      }
      else if(event.key.keysym.scancode == SDL_SCANCODE_UP)
      {
        if(command_buffer.size() && current_last_command > 0)
        {
          current_last_command--;
          input = command_buffer[current_last_command];
          console_pointer_pos = input.size();
        }
      }
      else if(event.key.keysym.scancode == SDL_SCANCODE_DOWN)
      {
        if(command_buffer.size() && (uint)current_last_command < command_buffer.size()-1)
        {
          current_last_command++;
          input = command_buffer[current_last_command];
          console_pointer_pos = input.size();
        }
      }
      else if(event.key.keysym.scancode == SDL_SCANCODE_LEFT)
      {
        if(console_pointer_pos > 0)
          console_pointer_pos--;
      }
      else if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
      {
        if(console_pointer_pos < input.length())
          console_pointer_pos++;
      }
      else if(event.key.keysym.scancode == SDL_SCANCODE_PAGEUP)
      {
        if(console_buffer.size() )
        {
          current_line_buffered += __CSYSTEM_DEBUG_CONSOLE_LINES;
          if((uint)current_line_buffered > console_buffer.size()-1)
            current_line_buffered = console_buffer.size()-1;
        }
      }
      else if(event.key.keysym.scancode == SDL_SCANCODE_PAGEDOWN)
      {
        if(console_buffer.size() )
        {
          current_line_buffered -= __CSYSTEM_DEBUG_CONSOLE_LINES;
          if(current_line_buffered < 0)
            current_line_buffered = 0;
        }
      }
      else if(event.key.keysym.scancode == SDL_SCANCODE_END)
      {
        current_line_buffered = 0;
      }
      else if(event.key.keysym.scancode == SDL_SCANCODE_HOME)
      {
        if(console_buffer.size())
          current_line_buffered = console_buffer.size()-1;
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
      else if( event.button.button == SDL_BUTTON_MIDDLE )
      {
        if(input != "") SDL_SetClipboardText(input.c_str());
      }
    }
    else if(event.type == SDL_MOUSEWHEEL)
    {
      if(event.wheel.y > 0 && console_buffer.size() && (uint)current_line_buffered < console_buffer.size()-1)
        current_line_buffered++;
      else if(event.wheel.y < 0 && console_buffer.size() && current_line_buffered > 0)
        current_line_buffered--;
    }
    else if(event.type == SDL_TEXTINPUT)
    {
      input.insert(console_pointer_pos, event.text.text);
      console_pointer_pos++;
    }
  }
  else
  {
    if(event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_GetScancodeFromName(gSystem_Data_Storage.GetString("__INPUT_CONSOLE_KEY").c_str()))
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
  glOrtho(0, gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH"), 0, __CSYSTEM_DEBUG_CONSOLE_SIZE, -1, 1);

  glViewport(0, gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_HEIGHT") - __CSYSTEM_DEBUG_CONSOLE_SIZE, gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH"), __CSYSTEM_DEBUG_CONSOLE_SIZE);
  glScissor(0, gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_HEIGHT") - __CSYSTEM_DEBUG_CONSOLE_SIZE, gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH"), __CSYSTEM_DEBUG_CONSOLE_SIZE);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  glEnable(GL_BLEND);

  glColor3f(1.f, 1.f, 1.f);

  // Show input
  print(__CSYSTEM_DEBUG_CONSOLE_X_OFFSET, 10, 0, ("> " + input).c_str());

  // Show console buffer
  //int line = 0;
  for(uint i = current_line_buffered, line = 0; i < __CSYSTEM_DEBUG_CONSOLE_LINES + (uint)current_line_buffered && i < console_buffer.size(); i++, line++)
  {
    string_console_t& s = console_buffer[i];
    glColor4f(s.line_color.r, s.line_color.g, s.line_color.b, s.line_color.a);
    print(__CSYSTEM_DEBUG_CONSOLE_X_OFFSET, line*__CSYSTEM_DEBUG_CONSOLE_LINESPACE + __CSYSTEM_DEBUG_CONSOLE_Y_OFFSET, 0, s.str.c_str());
  }

  // Draw console pointer
  glColor4f(0.75f, 0.75f, 0.75f, 1.f);
  print(__CSYSTEM_DEBUG_CONSOLE_X_OFFSET*3 + console_pointer_pos*10, 6, 0, "_");

  //glDisable(GL_BLEND);
  glPopAttrib();
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

  gSystem_Shader_Manager.UseShader();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glTranslated(x, y, 0);
  glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture(__CSYSTEM_DEBUG_CONSOLE_FONT)->GetID());
  glListBase(base-32 + (128)*set);

  /*const char* SHADER_UNIF_PROJMAT              = "ProjMatrix";
  const char* SHADER_UNIF_MODELMAT             = "ModelViewMatrix";
  const char* SHADER_UNIF_TEXTUREMAP           = "TextureMap";

  CShader* simpleShader = gShader.GetShader("example1");
  //glUseProgram(simpleShader->GetProgram());

  GLfloat modelMatrix[16] = {0};
  glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
  //glUniformMatrix4fv(simpleShader->GetUniformIndex(SHADER_UNIF_MODELMAT), 1, GL_FALSE, &modelMatrix[0]);

  GLfloat projMatrix[16] = {0};
  glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
  //glUniformMatrix4fv(simpleShader->GetUniformIndex(SHADER_UNIF_PROJMAT), 1, GL_FALSE, &projMatrix[0]);

  //glUniform1i(simpleShader->GetUniformIndex(SHADER_UNIF_TEXTUREMAP), 0);*/

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
  if(console_buffer.size() > __CSYSTEM_DEBUG_CONSOLE_CONSOLE_BUFFER_MAXLINES) console_buffer.pop_back();
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
  if(console_buffer.size() > __CSYSTEM_DEBUG_CONSOLE_CONSOLE_BUFFER_MAXLINES) console_buffer.pop_back();
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
  if(console_buffer.size() > __CSYSTEM_DEBUG_CONSOLE_CONSOLE_BUFFER_MAXLINES) console_buffer.pop_back();
}

void CSystem_Debug::console_info_msg(const char* fmt, ...)
{
  if(!fmt)
    return;

  char text[1024];

  va_list ap;
  va_start(ap, fmt);
    vsprintf(text, fmt, ap);
  va_end(ap);

  string_console_t t(text, 0.34f, 0.72f, 0.85f, 1.f);
  console_buffer.insert(console_buffer.begin(), t);
  if(console_buffer.size() > __CSYSTEM_DEBUG_CONSOLE_CONSOLE_BUFFER_MAXLINES) console_buffer.pop_back();
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
  if(console_buffer.size() > __CSYSTEM_DEBUG_CONSOLE_CONSOLE_BUFFER_MAXLINES) console_buffer.pop_back();
}

void CSystem_Debug::console_custom_msg(colorf_t color, const char* fmt, ...)
{
  if(!fmt)
    return;

  char text[1024];

  va_list ap;
  va_start(ap, fmt);
    vsprintf(text, fmt, ap);
  va_end(ap);

  string_console_t t(text, color.r, color.g, color.b, color.a);
  console_buffer.insert(console_buffer.begin(), t);
  if(console_buffer.size() > __CSYSTEM_DEBUG_CONSOLE_CONSOLE_BUFFER_MAXLINES) console_buffer.pop_back();
}

/** Console commands **/

void CSystem_Debug::Console_command__UNKNOWN_COMMAND(string arguments)
{
  console_error_msg("Unkown command: \"%s\"", arguments.c_str());
}

void CSystem_Debug::Console_command__HELP(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: help <category>");
    return;
  }

  if(arguments == "")
  {
    console_msg("Use \"help <category>\". Categories are:");
    console_warning_msg("general, systems, game_objects, sound, render");
    console_warning_msg("Also, you can user <command> ?");
  }
  else if(arguments == "general")
  {
    console_msg("General commands:");
    console_msg("-----------------------------------------------------------------------");
    console_msg("help:                           Display help.");
    console_msg("logs:                           Saves console content to %s file.", __CSYSTEM_DEBUG_STORAGE_SAVEFILE);

    console_msg("set_int:                        Sets a variable to a int.");
    console_msg("set_float:                      Sets a variable to a float.");
    console_msg("set_string:                     Sets a variable to a string.");

    console_msg("get_int:                        Gets a variable of a int.");
    console_msg("get_float:                      Gets a variable of a float.");
    console_msg("get_string:                     Gets a variable of a string.");
    console_msg("get_var:                        Gets a variable of any type.");

    console_msg("remove_int:                     Remove a int variable.");
    console_msg("remove_float:                   Remove a float variable.");
    console_msg("remove_string:                  Remove a string variable.");
    console_msg("remove_user_vars:               Remove all user vars.");

    console_msg("exit:                           Quits the program and save current opened files.");
    console_msg("abort:                          Aborts the program execution.");
    console_msg("clear:                          Clears the console content.");
    console_msg("run:                            Runs a console script file (batch file). Fil must be in data/resources/console_scripts.");
  }
  else if(arguments == "systems")
  {
    console_msg("Game Objects commands:");
    console_msg("-----------------------------------------------------------------------");
    console_msg("system_time_setscale:                Set current time scale.");
    console_msg("system_userinput_show_joysticks:     Show current joysticks connected to the system.");


  }
  else if(arguments == "game_objects")
  {
    console_msg("Game Objects commands:");
    console_msg("-----------------------------------------------------------------------");

    console_msg("go_component_enable:      Enables or disables a game object's component.");
    console_msg("go_component_set:         Changes properties of a game object's component.");
    console_msg("go_enable:                Enables or disables a game object and/or its children.");
    console_msg("go_show_tree:             Displays current game object's tree stored in the manager.");
    console_msg("go_search:                Searches game objects by prefix.");

  }
  else if(arguments == "sound")
  {
    console_msg("Sound commands:");
    console_msg("-----------------------------------------------------------------------");
    console_msg("snd_volume:                Set volume.");
    console_msg("snd_music_volume:          Set music volume.");
    console_msg("snd_checksources:          Shows source vectors status.");
    // Sound
  }
  else if(arguments == "render")
  {
    console_msg("Render commands:");
    console_msg("-----------------------------------------------------------------------");

    console_msg("r_draw_transform:               Draws transform component for each game object (X, Y, Z Local axis).");
    console_msg("r_draw_grid:                    Draws a world grid.");
    console_msg("r_draw_sound:                   Draw sound radius (max and min) for each audio source.");
    console_msg("r_fps:                          Gets current frames per second.");
    console_msg("r_update_window:                Updates window's modified properties and applys them to the window.");
    console_msg("r_resize_window:                Resizes the window.");
    console_msg("r_glinfo:                       Gets information about current opengl driver.");
  }
  // ...
  else
  {
    console_error_msg("Unknown help category: %s", arguments.c_str());
  }
}

void CSystem_Debug::Console_command__LOG(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: log");
    return;
  }

  if(!console_buffer.size())
    return;

  console_msg("Wrote console buffer to file \"%s\"", __CSYSTEM_DEBUG_STORAGE_SAVEFILE);

  raw_log("\n---------------------------------- CONSOLE BEGIN ----------------------------------\n\n");

  //    vector<string_console_t> console_buffer;
  for(vector<string_console_t>::reverse_iterator it = console_buffer.rbegin(); it != console_buffer.rend(); ++it)
  {
    raw_log("%s\n", it->str.c_str());
  }

  raw_log("\n----------------------------------- CONSOLE END -----------------------------------\n\n");
}

void CSystem_Debug::Console_command__SET_INT(string arguments)
{
  // ¿Restricciones de variables del sistema? hmmm
  if(arguments == "?")
  {
    console_warning_msg("Format is: set_int <identifier> [value]");
    return;
  }

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
  if(arguments == "?")
  {
    console_warning_msg("Format is: set_float <identifier> [value]");
    return;
  }


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
  if(arguments == "?")
  {
    console_warning_msg("Format is: set_string <identifier> [value]");
    return;
  }

  stringstream ss(arguments);
  string val_name, val;
  ss >> val_name;

  //if(val_name != "")
  //{
  getline(ss, val);
  if(val.size() > 2) val = val.substr(1);

  console_msg("%s = \"%s\"", val_name.c_str(), val.c_str());
  gSystem_Data_Storage.SetString(val_name, val);
  //}
  //else
  //  console_warning_msg("Format is: set_string <identifier> [value]");
}

void CSystem_Debug::Console_command__GET_INT(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: get_int [identifier]");
    return;
  }

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
    for(map<string, int>::iterator it = gSystem_Data_Storage.ints.begin(); it != gSystem_Data_Storage.ints.end(); ++it)
      console_msg("%s = %i", it->first.c_str(), it->second);
  }
}

void CSystem_Debug::Console_command__GET_FLOAT(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: get_float [identifier]");
    return;
  }

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
    for(map<string, float>::iterator it = gSystem_Data_Storage.floats.begin(); it != gSystem_Data_Storage.floats.end(); ++it)
      console_msg("%s = %f", it->first.c_str(), it->second);
  }
}

void CSystem_Debug::Console_command__GET_STRING(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: get_string [identifier]");
    return;
  }

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
    for(map<string, string>::iterator it = gSystem_Data_Storage.strings.begin(); it != gSystem_Data_Storage.strings.end(); ++it)
      console_msg("%s = \"%s\"", it->first.c_str(), it->second.c_str());
  }
}

void CSystem_Debug::Console_command__GET_VAR(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: get_vars [identifier]");
    return;
  }

  gSystem_Debug.console_custom_msg(0.15f, 0.7f, 1.f, 1.f, "  Searching ints:");
  Console_command__GET_INT(arguments);    // Search int
  gSystem_Debug.console_custom_msg(0.15f, 0.7f, 1.f, 1.f, "  Searching floats:");
  Console_command__GET_FLOAT(arguments);  // Search float
  gSystem_Debug.console_custom_msg(0.15f, 0.7f, 1.f, 1.f, "  Searching strings:");
  Console_command__GET_STRING(arguments); // Search string
}

void CSystem_Debug::Console_command__REMOVE_INT(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: remove_int <identifier>");
    return;
  }

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
  if(arguments == "?")
  {
    console_warning_msg("Format is: remove_float <identifier>");
    return;
  }

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
  if(arguments == "?")
  {
    console_warning_msg("Format is: remove_string <identifier>");
    return;
  }

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
  if(arguments == "?")
  {
    console_warning_msg("Format is: remove_user_vars");
    return;
  }

  console_warning_msg("Deleted all user variables");
  gSystem_Data_Storage.RemoveUserVars();
}

void CSystem_Debug::Console_command__CLEAR(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: clear");
    return;
  }

  // No se limpia el buffer de comandos (por comodidad, básicamente); solo el de resultados.
  console_buffer.clear();
  current_line_buffered = 0;
}

void CSystem_Debug::Console_command__ABORT(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: abort");
    return;
  }

  gEngine.Abort();
}

void CSystem_Debug::Console_command__EXIT(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: exit [reason message]");
    return;
  }

  if(arguments != "")
    gSystem_Debug.log("Exit: \"%s\"", arguments.c_str());
  gEngine.Exit();
}

void CSystem_Debug::Console_command__RUN(string arguments)
{
  // Parse txt (or binary) file
  if(arguments == "?")
  {
    console_warning_msg("Format is: run <file>");
    return;
  }

  stringstream ss(arguments);
  string file;
  ss >> file;

  file = "data\\resources\\console_scripts\\" + file;

  ifstream is(file.c_str());

  if(!is || !is.good())
  {
    console_error_msg("Cannot open file \"%s\"", file.c_str());
    return;
  }

  vector<string> parameters;
  string arg = file;

  while(arg != "" and parameters.size() < 10)
  {
    parameters.push_back(arg);
    arg = "";
    ss >> arg;
  }

  console_msg("Running script \"%s\"", file.c_str());
  console_msg("---------------------------------");

  string line;
  while(getline(is, line, '\n'))
  {
    //gSystem_Debug.log("Linea: %s", line.c_str());
    if(line[0] == '#' || line[0] == '\0') // Ignore comments and blank lines
      continue;

    // Replace parameters
    size_t ampersand_pos = 0;
    while((ampersand_pos = line.find('&', ampersand_pos)) != string::npos)
    {
      if(ampersand_pos < line.size() - 1)
      {
        ampersand_pos++;
        int value = (int)(line[ampersand_pos] - '0');

        line.erase(ampersand_pos - 1, 2);
        if(value >= 0 and value < (int)parameters.size())
          line.insert(ampersand_pos - 1, parameters[value]);
      }
    }

    // Parse command
    //command_buffer.push_back(line);
    //if(command_buffer.size() > __CSYSTEM_DEBUG_CONSOLE_COMMAND_BUFFER_MAXLINES) command_buffer.erase(command_buffer.begin()); // Check command_buffer size and clean if necesary

    current_last_command = command_buffer.size();
    input = line;
    ParseInput();
    input = "";
    console_pointer_pos = 0;
  }

  console_msg("---------------------------------");

  is.close();
}

/*void CSystem_Debug::Console_command__SAVE_STATE(string arguments)
{
  gSystem_GameObject_Manager.SaveGameObjects(arguments);
}

void CSystem_Debug::Console_command__LOAD_STATE(string arguments)
{
  gSystem_GameObject_Manager.LoadGameObjects(arguments);
}*/

void CSystem_Debug::Console_command__SECRET_PLZ(string arguments)
{
  static colorf_t color = {0.7f, 0.9f, 0.1f, 1.f};
  if(arguments == "?")
  {
    color.r = gSystem_Math.random();
    color.g = gSystem_Math.random();
    color.b = gSystem_Math.random();
  }

  console_custom_msg(color.r, color.g, color.b, color.a, "      ________                 ___________              .__");
  console_custom_msg(color.r, color.g, color.b, color.a, "     /  _____/  ____           \\_   _____/ ____    ____ |__| ____   ____");
  console_custom_msg(color.r, color.g, color.b, color.a, "    /   \\  ___ /  _ \\   ______  |    __)_ /    \\  / ___\\|  |/    \\_/ __ \\");
  console_custom_msg(color.r, color.g, color.b, color.a, "    \\    \\_\\  (  <_> ) /_____/  |        \\   |  \\/ /_/  >  |   |  \\  ___/");
  console_custom_msg(color.r, color.g, color.b, color.a, "     \\______  /\\____/          /_______  /___|  /\\___  /|__|___|  /\\___  >");
  console_custom_msg(color.r, color.g, color.b, color.a, "            \\/                         \\/     \\//_____/         \\/     \\/");
}

// Systems
void CSystem_Debug::Console_command__SYSTEM_TIME_SETSCALE(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: system_time_setscale <value>");
    return;
  }

  stringstream ss(arguments);
  GLfloat val;
  ss >> val;

  if(val >= 0)
  {
    gSystem_Time.SetTimeScale(val);
    console_warning_msg("Time scale set to %f", val);
  }
  else
    console_warning_msg("Value must be greather than 0");
}

void CSystem_Debug::Console_command__SYSTEM_USERINPUT_SHOW_JOYSTICKS(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: system_userinput_show_joysticks");
    return;
  }

  gSystem_Debug.console_custom_msg(0.15f, 0.7f, 1.f, 1.f, "Joysticks connected");
  gSystem_Debug.console_custom_msg(0.15f, 0.7f, 1.f, 1.f, "-------------------");
  gSystem_Debug.console_custom_msg(0.75f, 0.75f, 0.75f, 1.f, " <id>- <Name> - <Num axes>/<Num balls>/<Num buttons>/<Num povs>");

  vector<GO_InputClasses::CJoystick> joys = gUserInput.GetJoysticks();
  for(vector<GO_InputClasses::CJoystick>::iterator it = joys.begin(); it != joys.end(); ++it)
  {
    gSystem_Debug.console_msg(" %d- %s - %d/%d/%d/%d", it - joys.begin(), (*it).GetName().c_str(), (*it).axes.size(), (*it).balls.size(), (*it).buttons.size(), (*it).povs.size());
  }

}

// Game Objects
void CSystem_Debug::Console_command__AUX__GO_SHOW_TREE_print_element(CGameObject* go, map<string, void*>& list, int level)
{
  if(list.find(go->GetName()) != list.end()) // Ya dibujado :(
    return;

  string c = "";
  c.resize(level*2, ' ');

  // Se mostrarán hijos que no pertenezcan al manager de padres que sí pertenezca, recursivamente.

  if(go->GetName()[0] != '_' && go->GetName()[1] != '_')
  {
    char visible = '*';
    if(gSystem_GameObject_Manager[go->GetName()] != NULL)
      visible = ' ';

    if(go->IsEnabled())
      gSystem_Debug.console_msg("%s- %s", c.c_str(), go->GetName().c_str());
    else
      gSystem_Debug.console_custom_msg(0.5f, 0.5f, 0.5f, 1.f, "%s- %s%c", c.c_str(), go->GetName().c_str(), visible);
  }

  list.insert(pair<string, void*>(go->GetName(), NULL));

  /*uint num_child = go->GetNumChildren();
  for(uint i = 0; i < num_child && num_child != 0; i++)
    print_element(go->GetChild(i), list, level+1);*/
  for(map<string, CGameObject*>::iterator it = go->children.begin(); it != go->children.end(); ++it)
    Console_command__AUX__GO_SHOW_TREE_print_element(it->second, list, level+1);
}

void CSystem_Debug::Console_command__GO_SHOW_TREE(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: go_show_tree [game object]");
    return;
  }

  stringstream ss(arguments);
  string go_name;
  ss >> go_name;

  map<string, void*> game_objects;

  if(go_name == "")
  {
    gSystem_Debug.console_custom_msg(0.15f, 0.7f, 1.f, 1.f, "Hierarchy of current game objects");
    gSystem_Debug.console_custom_msg(0.15f, 0.7f, 1.f, 1.f, "---------------------------------");

    for(map<string, CGameObject*>::iterator it = gSystem_GameObject_Manager.gameObjects.begin(); it != gSystem_GameObject_Manager.gameObjects.end(); ++it)
      if(game_objects.find(it->first) == game_objects.end() && it->second->GetParent() == NULL)
        Console_command__AUX__GO_SHOW_TREE_print_element(it->second, game_objects);
  }
  else
  {
    CGameObject* go = gSystem_GameObject_Manager[go_name];

    if(!go)
    {
      console_error_msg("Could not find game object named \"%s\"", go_name.c_str());
      return;
    }

    gSystem_Debug.console_custom_msg(0.15f, 0.7f, 1.f, 1.f, "Hierarchy of \"%s\":", go_name.c_str());
    gSystem_Debug.console_custom_msg(0.15f, 0.7f, 1.f, 1.f, "-----------------------------");
    Console_command__AUX__GO_SHOW_TREE_print_element(go, game_objects);
  }
}


void CSystem_Debug::Console_command__GO_ENABLE(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: go_enable <game_object_name> < 1 | enable | 0 | disable> [r[ecursive]]");
    return;
  }

  stringstream ss(arguments);
  string obj_name;
  ss >> obj_name;

  if(obj_name == __CSYSTEM_DEBUG_KEYWORD_ALL)
  {
    string value;
    ss >> value;

    if(value == "1" or value == "enable")
    {
      gSystem_GameObject_Manager.EnableGameObjects();
      console_msg("All game objects enabled");
    }
    else if(value == "0" or value == "disable")
    {
      gSystem_GameObject_Manager.DisableGameObjects();
      console_msg("All game objects disabled");
    }
    else
      console_warning_msg("Format is: go_enable <game_object_name> < 1 | enable | 0 | disable> [r[ecursive]]");

    return;
  }

  if(obj_name == "")
  {
    console_warning_msg("Format is: go_enable <game_object_name> < 1 | enable | 0 | disable> [r[ecursive]]");
    return;
  }

  if(!gSystem_GameObject_Manager[obj_name])
  {
    console_error_msg("Could not find game object named \"%s\"", obj_name.c_str());
    return;
  }

  string value, recursive, recursive_str = "";
  bool recursive_b = false;
  ss >> value >> recursive;

  if(recursive == "r" || recursive == "recursive")
  {
    recursive_str = " and all its children";
    recursive_b = true;
  }
  else if(recursive == "")
    recursive_b = false;
  else
    console_warning_msg("Format is: go_enable <game_object_name> < 1 | enable | 0 | disable> [r[ecursive]]");

  if(value == "1" or value == "enable")
  {
    gSystem_GameObject_Manager[obj_name]->Enable(recursive_b);
    console_msg("\"%s\"%s enabled", obj_name.c_str(), recursive_str.c_str());
  }
  else if(value == "0" or value == "disable")
  {
    gSystem_GameObject_Manager[obj_name]->Disable(recursive_b);
    console_msg("\"%s\"%s disabled", obj_name.c_str(), recursive_str.c_str());
  }
  else
  {
    console_warning_msg("Format is: go_enable <game_object_name> < 1 | enable | 0 | disable> [r[ecursive]]");
  }
}

void CSystem_Debug::Console_command__GO_COMPONENT_ENABLE(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: go_component_enable <game_object_name> <component_name> <1 | enable | 0 | disable>");
    return;
  }

  stringstream ss(arguments);
  string obj_name;
  ss >> obj_name;

  if(obj_name == "")
  {
    console_warning_msg("Format is: go_component_enable <game_object_name> <component_name> <1 | enable | 0 | disable>");
    return;
  }

  CGameObject* go = gSystem_GameObject_Manager[obj_name];

  if(!go)
  {
    console_error_msg("Could not find game object named \"%s\"", obj_name.c_str());
    return;
  }

  string component;
  ss >> component;

  if(component == "")
  {
    console_warning_msg("Format is: go_component_enable <game_object_name> <component_name> <1 | enable | 0 | disable>");
    return;
  }

  int component_i = Components::string_to_component(component);
  if(component_i == Components::__component_not_defined)
  {
    console_error_msg("Component type \"%s\" does not exists.", component.c_str());
    return;
  }

  CComponent* c_go = go->GetComponent((Components::components_t)component_i);

  if(!c_go)
  {
    console_error_msg("Game object \"%s\" does not have the component \"%s\".", obj_name.c_str(), component.c_str());
    return;
  }

  string value;
  ss >> value;
  if(value == "1" or value == "enable")
  {
    c_go->Enable();
    console_msg("\"%s -> %s\" enabled", obj_name.c_str(), component.c_str());
  }
  else if(value == "0" or value == "disable")
  {
    c_go->Disable();
    console_msg("\"%s -> %s\" disabled", obj_name.c_str(), component.c_str());
  }
  else
  {
    console_warning_msg("Format is: go_component_enable <game_object_name> <component_name> <1 | enable | 0 | disable>");
  }
}

void CSystem_Debug::Console_command__GO_COMPONENT_SET(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: go_component_set <game_object_name> <component_name> <atrib> <value>");
    return;
  }

  stringstream ss(arguments);
  string obj_name;
  ss >> obj_name;

  if(obj_name == "")
  {
    console_warning_msg("Format is: go_component_set <game_object_name> <component_name> <atrib> <value>");
    return;
  }

  CGameObject* go = gSystem_GameObject_Manager[obj_name];

  if(!go)
  {
    console_error_msg("Could not find game object named \"%s\"", obj_name.c_str());
    return;
  }

  string component;
  ss >> component;

  if(component == "")
  {
    console_warning_msg("Format is: go_component_set <game_object_name> <component_name> <atrib> <value>");
    return;
  }

  int component_i = Components::string_to_component(component);
  if(component_i == Components::__component_not_defined)
  {
    console_error_msg("Component type \"%s\" does not exists.", component.c_str());
    return;
  }

  CComponent* c_go = go->GetComponent((Components::components_t)component_i);

  if(!c_go)
  {
    console_error_msg("Game object \"%s\" does not have the component \"%s\".", obj_name.c_str(), component.c_str());
    return;
  }

  string data;
  getline(ss, data);

  c_go->parseDebug(data);
}

void CSystem_Debug::Console_command__GO_COMPONENT_GET(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: go_component_get <game_object_name> [component_name]");
    return;
  }

  stringstream ss(arguments);
  string obj_name;
  ss >> obj_name;

  if(obj_name == "")
  {
    console_warning_msg("Format is: go_component_get <game_object_name> [component_name]");
    return;
  }

  CGameObject* go = gSystem_GameObject_Manager[obj_name];

  if(!go)
  {
    console_error_msg("Could not find game object named \"%s\"", obj_name.c_str());
    return;
  }

  string component;
  ss >> component;

  if(component == "")
  {
    for(map<int, CComponent*>::iterator it = go->components.begin(); it != go->components.end(); ++it)
    {
      it->second->printDebug();
      console_warning_msg("");
    }

    return;
  }

  int component_i = Components::string_to_component(component);
  if(component_i == Components::__component_not_defined)
  {
    console_error_msg("Component type \"%s\" does not exists.", component.c_str());
    return;
  }

  CComponent* c_go = go->GetComponent((Components::components_t)component_i);

  if(!c_go)
  {
    console_error_msg("Game object \"%s\" does not have the component \"%s\".", obj_name.c_str(), component.c_str());
    return;
  }

  c_go->printDebug();
}

void CSystem_Debug::Console_command__GO_SEARCH(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: go_search <prefix>");
    return;
  }

  stringstream ss(arguments);
  string prefix;
  ss >> prefix;

  if(prefix == "")
  {
    console_warning_msg("Format is: go_search <prefix>");
    return;
  }

  vector<CGameObject*> v_go = gSystem_GameObject_Manager.Search(prefix);

  if(!v_go.size())
    console_error_msg("Could not find game object prefixed with \"%s\"", prefix.c_str());
  else
    for(vector<CGameObject*>::iterator it = v_go.begin(); it != v_go.end(); ++it)
      gDebug.console_msg("%s", (*it)->GetName().c_str());
}

// Sound
void CSystem_Debug::Console_command__SND_VOLUME(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: snd_volume < volume | $MUTE >");
    return;
  }

  if(arguments == "$MUTE")
  {
    gSystem_Data_Storage.SetFloat("__SOUND_VOLUME", 0);
  }
  else
  {
    stringstream ss(arguments);
    float value = 1.f;
    ss >> value;

    if(value > 1) value = 1.f;
    gSystem_Data_Storage.SetFloat("__SOUND_VOLUME", value);
    console_msg("Set volume to %f", value);
  }
}

void CSystem_Debug::Console_command__SND_MUSIC_VOLUME(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: snd_music_volume < volume | $MUTE >");
    return;
  }

  if(arguments == "$MUTE")
  {
    gSystem_Data_Storage.SetFloat("__SOUND_MUSIC_VOLUME",0);
  }
  else
  {
    stringstream ss(arguments);
    float value = 1.f;
    ss >> value;

    if(value > 1) value = 1.f;
    gSystem_Data_Storage.SetFloat("__SOUND_MUSIC_VOLUME",value);
    console_msg("Set music volume to %f", value);
  }
}

void CSystem_Debug::Console_command__SND_CHECKSOURCES(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: snd_checksources");
    return;
  }

  // Sources
  uint n_sources = gSystem_Mixer.sources_used.size();
  if(n_sources >= gSystem_Mixer.NUMBER_SOURCES*0.95)
    console_error_msg("Sources used:  %3d / %3d", n_sources, gSystem_Mixer.NUMBER_SOURCES);
  else if(n_sources >= gSystem_Mixer.NUMBER_SOURCES*0.8)
    console_warning_msg("Sources used:  %3d / %3d", n_sources, gSystem_Mixer.NUMBER_SOURCES);
  else
    console_msg("Sources used:  %3d / %3d", n_sources, gSystem_Mixer.NUMBER_SOURCES);

  // Oneshots
  n_sources = gSystem_Mixer.oneshot_used.size();
  if(n_sources >= gSystem_Mixer.NUMBER_SOURCES_ONESHOT*0.95)
    console_error_msg("OneShots used: %3d / %3d", n_sources, gSystem_Mixer.NUMBER_SOURCES_ONESHOT);
  else if(n_sources >= gSystem_Mixer.NUMBER_SOURCES_ONESHOT*0.8)
    console_warning_msg("OneShots used: %3d / %3d", n_sources, gSystem_Mixer.NUMBER_SOURCES_ONESHOT);
  else
    console_msg("OneShots used: %3d / %3d", n_sources, gSystem_Mixer.NUMBER_SOURCES_ONESHOT);
}

// Render
void CSystem_Debug::Console_command__R_UPDATE_WINDOW(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: update_window");
    return;
  }

  if(gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH") <= 0 or gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_HEIGHT") <= 0 )
    console_error_msg("Invalid values of __RENDER_RESOLUTION_WIDTH (%i) or __RENDER_RESOLUTION_HEIGHT (%i)", gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH"), gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_HEIGHT"));
  else
  {
    gSystem_Render.ResizeWindow(gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH"), gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_HEIGHT"));
    console_msg("Set window to resolution %i x %i", gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH"), gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_HEIGHT"));
  }
}

void CSystem_Debug::Console_command__R_RESIZE_WINDOW(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: r_resize_window <width> <height> [windowed | fullscreen | fullwindowed]");
    return;
  }

  stringstream ss(arguments);
  int w = 0, h = 0;
  ss >> w >> h;

  string videomode;
  ss >> videomode;

  if(videomode != "")
  {
    if(videomode == "windowed")
      gSystem_Render.SetFullScreenWindow(Render::windowed);
    else if (videomode == "fullscreen")
      gSystem_Render.SetFullScreenWindow(Render::fullscreen);
    else if (videomode == "fullwindowed")
      gSystem_Render.SetFullScreenWindow(Render::fullwindowed);
    else
      console_error_msg("Invalid video mode: %s", videomode.c_str());
  }

  if(w <= 0 or h <= 0)
  {
    console_error_msg("Invalid values of width (%i) or height (%i)", w, h);
    console_warning_msg(
        "Format is: r_resize_window <width> <height> [windowed | fullscreen | fullwindowed]");
  }
  else
  {
    gSystem_Data_Storage.SetInt("__RENDER_RESOLUTION_WIDTH", w);
    gSystem_Data_Storage.SetInt("__RENDER_RESOLUTION_HEIGHT", h);
    Console_command__R_UPDATE_WINDOW(arguments);
    console_msg("Resized window to %i x %i", gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_WIDTH"),
        gSystem_Data_Storage.GetInt("__RENDER_RESOLUTION_HEIGHT"));
  }


}

void CSystem_Debug::Console_command__R_DRAW_TRANSFORM(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: r_draw_transform <0 | 1>");
    return;
  }

  stringstream ss(arguments);
  int val = -1;
  ss >> val;

  if(val < 0 or val > 1)
    console_warning_msg("Format is: r_draw_transform <0 | 1>");
  else
  {
    gSystem_Data_Storage.SetInt("__RENDER_TRANSFORM", val);
    if(val) console_msg("Transform render enabled.");
    else    console_msg("Transform render disabled.");

  }
}

void CSystem_Debug::Console_command__R_DRAW_GRID(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: r_draw_grid <0 | 1>");
    return;
  }

  stringstream ss(arguments);
  int val = -1;
  ss >> val;

  if(val < 0 or val > 1)
    console_warning_msg("Format is: r_draw_grid <0 | 1>");
  else
  {
    gSystem_Data_Storage.SetInt("__RENDER_TRANSFORM_GRID", val);
    if(val) console_msg("Grid render enabled.");
    else    console_msg("Grid render disabled.");
  }
}

void CSystem_Debug::Console_command__R_FPS(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: r_fps");
    return;
  }

  GLdouble fps = gEngine.fps();
  if(fps < 10)
    console_error_msg("FPS: %f", gEngine.fps());
  else if(fps < 30)
    console_warning_msg("FPS: %f", gEngine.fps());
  else
    console_msg("FPS: %f", gEngine.fps());
}

void CSystem_Debug::Console_command__R_DRAW_SOUND(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: r_draw_sound <0 | 1> [Z subdivisions] [X subdivisions]");
    return;
  }

  stringstream ss(arguments);
  int val = -1;
  float sub_x = -1, sub_z = -1;
  ss >> val >> sub_x >> sub_z;

  if(val < 0 or val > 1)
    console_warning_msg("Format is: r_draw_sound <0 | 1> [Z subdivisions] [X subdivisions]");
  else
  {
    gSystem_Data_Storage.SetInt("__RENDER_SOUND_RADIUS", val);
    if(sub_x > 0) gSystem_Data_Storage.SetFloat("__RENDER_SOUND_RADIUS_X", sub_x);
    if(sub_z > 0) gSystem_Data_Storage.SetFloat("__RENDER_SOUND_RADIUS_Z", sub_z);

    if(val) console_msg("Sound radius render enabled.");
    else    console_msg("Sound radius render disabled.");
  }
}

void CSystem_Debug::Console_command__R_GLINFO(string arguments)
{
  if(arguments == "?")
  {
    console_warning_msg("Format is: r_glinfo <show | log> [ext[ension]]");
    return;
  }

  stringstream ss(arguments);
  string argument;
  ss >> argument;

  void (CSystem_Debug::*display_function)(const char*, ...) = NULL;
  if(argument == "show")
  {
    display_function = &CSystem_Debug::console_msg;
  }
  else if(argument == "log")
  {
    display_function = &CSystem_Debug::raw_log;
  }
  else
  {
    console_warning_msg("Format is: r_glinfo <show | log> [ext[ension]]");
    return;
  }

  if(gSystem_Render.GLInfo.size())
  {
    (this->*display_function)("\n\n");
    (this->*display_function)("Opengl Information:\n");
    (this->*display_function)("------------------------------------\n");

    (this->*display_function)("Vendor:             %s\n", gSystem_Render.GLInfo[0].c_str());
    (this->*display_function)("Renderer:           %s\n", gSystem_Render.GLInfo[1].c_str());
    (this->*display_function)("Version:            %s\n", gSystem_Render.GLInfo[2].c_str());
    (this->*display_function)("GLSL Version:       %s\n", gSystem_Render.GLInfo[3].c_str());
    (this->*display_function)("Max MSAA samples:   %s\n", gSystem_Render.GLInfo[4].c_str());

    argument = "";
    ss >> argument;

    if(argument == "ext" or argument == "extension")
    {
      (this->*display_function)("\n");
      (this->*display_function)("Extensions supported:\n");
      (this->*display_function)("------------------------------------\n");

      stringstream ss(gSystem_Render.GLInfo[5]);
      string extension;
      int i = 0;
      while(getline(ss, extension, ' '))
      {
        (this->*display_function)("%d. %s\n", i, extension.c_str());
        i++;
      }
    }
    else if(argument != "")
    {
      console_warning_msg("Format is: r_glinfo <show | log> [ext]");
    }
  }

}
