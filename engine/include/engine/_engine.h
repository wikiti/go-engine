#ifndef __ENGINE_H_
#define __ENGINE_H_

#include "_globals.h"
#include "engine/_instance.h"

#include "systems/_render.h"

// Engine resource files
// icono, etc

class CEngine
{
  protected:
    //vector<CInstance*> instances;
    std::map<std::string, CInstance*> instances;
    std::string title;
    bool running;

    std::string current_instance;

    // Arguments
    std::vector<std::string> arguments;

  protected:
    void RemoveAllInstances();

  public:
    CEngine();
    virtual ~CEngine(){};

    virtual bool Init(int argc, char* argv[]);
    virtual void Close();

    virtual int OnExecute(int argc, char* argv[]);

    // Should be 32x32 RGBA format
    void SetIcon(std::string icon_name);

    std::string GetTitle()
    {
      return title;
    }

    void SetTitle(const char* text)
    {
      if(text)
        title = text;

      SDL_SetWindowTitle(gSystem_Render.window, title.c_str());
    }

    void AddInstance(fboolpointer load_gameObject_function, std::string resource_file, std::string instance_name);

    GLdouble fps()
    {
      return instances[current_instance]->fps();
    }

    CInstance* CurrentInstance()
    {
      return instances[current_instance];
    }

    std::vector<std::string>& Arguments()
    {
      return arguments;
    }

    // Exits without saving.
    // Should never be used
    void Exit()
    {
      exit(0);
    }

    void Quit()
    {
      running = false;
      if(instances.find(current_instance) != instances.end())
        instances[current_instance]->i_running = false;
    }

    std::string GetCurrentInstance()
    {
      return current_instance;
    }

    std::string GetNextInstance()
    {
      if(instances.find(current_instance) != instances.end())
        return instances[current_instance]->next_instance;

      return "";
    }

    void NextInstance(std::string instance)
    {
      if(instances.find(current_instance) != instances.end())
      {
        instances[current_instance]->NextInstance(instance);
      }
    }

    void SetNextInstance(std::string instance)
    {
      if(instances.find(current_instance) != instances.end())
        instances[current_instance]->SetNextInstance(instance);
    }

    void EndInstance()
    {
      instances[current_instance]->EndInstance();
    }
};

extern CEngine gEngine;


#endif /* __ENGINE_H_ */
