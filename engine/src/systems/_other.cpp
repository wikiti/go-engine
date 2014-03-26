#include "systems/_other.h"

CSystem_Time gSystem_Time;
CSystem_Time& gTime = gSystem_Time;

using namespace std;

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

vector3f CSystem_Math::slerp(vector3f from, vector3f to, float alpha)
{
  if(alpha < 0.f or alpha > 1.f)
    alpha = 1.f;

  from = this->NormalizeAngles(from);
  to = this->NormalizeAngles(to);

  from = this->deg_to_rad(from);
  to = this->deg_to_rad(to);

  vector3f output(glm::mix(((vector3f)from).to_glm(), ((vector3f)to).to_glm(), alpha));
  output = this->rad_to_deg(output);
  output = this->NormalizeAngles(output);

  return output;
}

// http://stackoverflow.com/questions/2708476/rotation-interpolation
float CSystem_Math::lerpAngle(float from, float to, float t)
{
  if(t < 0.f or t > 1.f)
    t = 1.f;

  if(abs(to - from) > 180)
  {
    if(to > from)
      from += 360;
    else
      to += 360;
  }

  return NormalizeAngle(from + ((to - from) * t));
}

vector3f CSystem_Math::lerpAngles(vector3f from, vector3f to, float t)
{
  return vector3f(lerpAngle(from.x, to.x, t), lerpAngle(from.y, to.y, t), lerpAngle(from.z, to.z, t));
}

float CSystem_Math::lerp(float from, float to, float t)
{
  return from + (to - from)*t;
}

vector3f CSystem_Math::lerp(vector3f from, vector3f to, float t)
{
  return from + (to - from)*t;
}
