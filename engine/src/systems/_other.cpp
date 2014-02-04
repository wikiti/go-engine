#include "systems/_other.h"

CSystem_Time gSystem_Time;
CSystem_Time& gTime = gSystem_Time;

bool CSystem_Time::Init()
{
  if(enabled) return true;
  CSystem::Init();

  deltaT = lastT = 0;
  Tscale = 1.f;
  return true;
}

void CSystem_Time::Close()
{
  if(!enabled) return;
  CSystem::Close();

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

CSystem_Math gSystem_Math;
CSystem_Math& gMath = gSystem_Math;

const float CSystem_Math::PI = M_PI;

const vector3f CSystem_Math::X_AXIS = vector3f(1.f, 0.f, 0.f);
const vector3f CSystem_Math::Y_AXIS = vector3f(0.f, 1.f, 0.f);
const vector3f CSystem_Math::Z_AXIS = vector3f(0.f, 0.f, 1.f);
const vector3f CSystem_Math::ORIGIN = vector3f(0.f, 0.f, 0.f);
