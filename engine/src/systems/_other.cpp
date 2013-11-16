#include "systems/_other.h"

CSystem_Time gSystem_Time;
CSystem_Time& gTime = gSystem_Time;

bool CSystem_Time::Init()
{
  deltaT = lastT = 0;
  Tscale = 1.f;
  return true;
}

void CSystem_Time::Close()
{
  Tscale = 0.f;

  deltaT = lastT = 0;
}

void CSystem_Time::OnLoop()
{
  Uint32 now=SDL_GetTicks();

  if(now > lastT)
  {
    deltaT = now - lastT;
    lastT = now;
  }
}
