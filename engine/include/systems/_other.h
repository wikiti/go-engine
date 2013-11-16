#ifndef __CSYSTEM_OTHER_H_
#define __CSYSTEM_OTHER_H_

#include "_globals.h"
#include "_system.h"

class CSystem_Time: public CSystem
{
  private:
    Uint32 deltaT;
    Uint32 lastT;

    GLfloat Tscale;

  public:
    CSystem_Time() {};

    bool Init();
    void Close();

    void OnLoop();

    void SetTimeScale(GLfloat tscale)
    {
      if(tscale < 0.f)
        tscale = -tscale;
      Tscale = tscale;
    }

    GLfloat timeScale()
    {
      return Tscale;
    }

    Uint32 deltaTime()
    {
      return deltaT * Tscale;
    }

    GLfloat deltaTime_s()
    {
      return deltaT/1000.f * Tscale;
    }

    Uint32 GetTicks()
    {
      //return SDL_GetTicks();
      return SDL_GetTicks()*Tscale;
    }

    GLfloat GetTicks_s()
    {
      //return SDL_GetTicks()/1000.f;
      return Tscale*SDL_GetTicks()/1000.f;
    }

    void Pause() { Tscale = 0.f; }
    void Resume() { Tscale = 1.f; }
};

extern CSystem_Time gSystem_Time;
extern CSystem_Time& gTime;

/** Poner la consola en debug?
class CSystem_Console: public CSystem
{
  private:
    Uint32 deltaT;
    Uint32 lastT;

    GLfloat Tscale;

  public:
    CSystem_Console() {};

    bool Init();
    void Close();

    void OnLoop();

    void SetTimeScale(GLfloat tscale)
    {
      if(tscale < 0.f)
        tscale = -tscale;
      Tscale = tscale;
    }

    GLfloat timeScale()
    {
      return Tscale;
    }

    Uint32 deltaTime()
    {
      return deltaT * Tscale;
    }

    GLfloat deltaTime_s()
    {
      return deltaT/1000.f * Tscale;
    }

    void Pause() { Tscale = 0.f; }
    void Resume() { Tscale = 1.f; }
};

extern CSystem_Time gSystem_Time;
extern CSystem_Time& gTime; **/

#endif /* __CSYSTEM_OTHER_H_ */
