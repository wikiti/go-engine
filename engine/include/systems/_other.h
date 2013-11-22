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

    // Aritmética
    float abs(float val)
    {
      return std::abs(val);
    }

    // trigonometría
    inline void NormalizeAngles(GLfloat &deg_x, GLfloat &deg_y, GLfloat &deg_z)
    {
      if(deg_x < 0 ) deg_x = 360 + deg_x;
      if(deg_y < 0 ) deg_y = 360 + deg_y;
      if(deg_z < 0 ) deg_z = 360 + deg_z;

      if(deg_x >= 360 ) deg_x = deg_x - 360;
      if(deg_y >= 360 ) deg_y = deg_y - 360;
      if(deg_z >= 360 ) deg_z = deg_z - 360;
    }


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
      return std::cos(_RAD_TO_DEG(degrees));
    }

    float sin(float degrees)
    {
      return std::sin(_RAD_TO_DEG(degrees));
    }

    float tan(float degrees)
    {
      return std::tan(_RAD_TO_DEG(degrees));
    }

    float acos(float radians)
    {
      return _RAD_TO_DEG(std::acos(radians));
    }

    float asin(float radians)
    {
      return _RAD_TO_DEG(std::asin(radians));
    }

    float atan(float radians)
    {
      return _RAD_TO_DEG(std::atan(radians));
    }

    // ...

    float random()
    {
      return (float)rand()/RAND_MAX;
    }

    // vector

  public:
    const vector3f X_AXIS = vector3f(1.f, 0.f, 0.f);
    const vector3f Y_AXIS = vector3f(0.f, 1.f, 0.f);
    const vector3f Z_AXIS = vector3f(0.f, 0.f, 1.f);

    void normalize(vector3f& v)
    {
      float len = v.length();
      v.x /= len;
      v.y /= len;
      v.z /= len;
    }

    vector3f_t cross_product(vector3f v1, vector3f_t v2)
    {
      vector3f_t out;

      out.x = v1.y*v2.z - v1.z*v2.y;
      out.y = v1.x*v2.z - v1.z*v2.x;
      out.z = v1.x*v2.y - v1.y*v2.x;

      return out;
    }

    typedef glm::quat quat;

    quat RotationBetweenVectors(vector3f start, vector3f dest)
    {
      normalize(start);
      normalize(dest);

      float cosTheta = start.dot_product(dest);
      vector3f rotationAxis;

      if (cosTheta < -1 + 0.001f){
            // special case when vectors in opposite directions:
            // there is no "ideal" rotation axis
            // So guess one; any will do as long as it's perpendicular to start
        rotationAxis = cross_product(Z_AXIS, start);
        if (glm::length2(rotationAxis.to_glm()) < 0.01 ) // bad luck, they were parallel, try again!
            rotationAxis = cross_product(X_AXIS, start);

        normalize(rotationAxis);
        return glm::angleAxis(180.0f, rotationAxis.to_glm());
      }

      rotationAxis = cross_product(start, dest);

      float s = sqrt( (1+cosTheta)*2 );
      float invs = 1 / s;

      return quat(
        s * 0.5f,
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs
      );
    }

    vector3f transform(glm::quat& q, vector3f v)
    {
       glm::quat tmp(0.0, v.to_glm());

       tmp = (q * tmp * glm::conjugate(q));
       //this(0);
       return vector3f(tmp.x, tmp.y, tmp.z);
    }

};

extern CSystem_Math gSystem_Math;
extern CSystem_Math& gMath;


#endif /* __CSYSTEM_OTHER_H_ */
