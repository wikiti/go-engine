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

class CSystem_Math: public CSystem
{
  public:
    const float PI = M_PI;

  public:
    CSystem_Math(): CSystem(){}
    virtual ~CSystem_Math(){};

    bool Init()
    {
      srand(time(NULL));

      return true;
    }

    void Close() {}

    float rad_to_deg(float rad)
    {
      return _RAD_TO_DEG(rad);
    }

    float deg_to_rad(float deg)
    {
      return _DEG_TO_RAD(deg);
    }

    float cos(float degrees)
    {
      return cos(_RAD_TO_DEG(degrees));
    }

    float sin(float degrees)
    {
      return sin(_RAD_TO_DEG(degrees));
    }

    // ...

    float random()
    {
      return (float)rand()/RAND_MAX;
    }
};

extern CSystem_Math gSystem_Math;
extern CSystem_Math& gMath;


#endif /* __CSYSTEM_OTHER_H_ */
