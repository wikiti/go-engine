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

    // Geometría
    typedef struct cone_t
    {
      vector3f direction;
      GLfloat angle;
    } cone_t;

    vector3f random_vector()
    {
      // http://www.gamedev.net/topic/499972-generate-a-random-unit-vector/
      float theta = random() * 2.0f * PI;
      float r = sqrt( random() );

      float random_value = 1.f;
      if(rand() < 0.5) random_value = -1.f;

      float z = sqrt( 1.0f - r*r ) * random_value;
      return vector3f( r * std::cos(theta), r * std::sin(theta), z );
    }

    vector3f random_vector(cone_t cone)
    {
      return random_vector(cone.direction, cone.angle);
    }

    inline vector3f random_vector(vector3f direction, GLfloat angle_degrees)
    {
      // http://stackoverflow.com/questions/2659257/perturb-vector-by-some-angle
      NormalizeAngle(angle_degrees);

      vector3f rand_vector = random_vector();
      vector3f cross_vector = (direction % rand_vector).normalize();

      float s = random();
      float r = random();

      float h = cos( angle_degrees );

      float phi = 2 * PI * s;
      float z = h + ( 1 - h ) * r;
      float sinT = sqrt( 1 - z * z );

      float x = std::cos( phi ) * sinT;
      float y = std::sin( phi ) * sinT;

      return (rand_vector * x + cross_vector * y + direction * z).normalize();
    }

    // Aritmética
    float abs(float val)
    {
      return std::abs(val);
    }

    // trigonometría
    inline void NormalizeAngles(GLfloat &deg_x, GLfloat &deg_y, GLfloat &deg_z)
    {
      if(deg_x < 0 ) deg_x = 360 + deg_x;
      else if(deg_x >= 360 ) deg_x = deg_x - 360;

      if(deg_y < 0 ) deg_y = 360 + deg_y;
      else if(deg_y >= 360 ) deg_y = deg_y - 360;

      if(deg_z < 0 ) deg_z = 360 + deg_z;
      else if(deg_z >= 360 ) deg_z = deg_z - 360;
    }

    inline void NormalizeAngle(GLfloat &deg)
    {
      if(deg < 0 ) deg = 360 + deg;
      else if(deg >= 360 ) deg = deg - 360;
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
      return std::cos(_DEG_TO_RAD(degrees));
    }

    float sin(float degrees)
    {
      return std::sin(_DEG_TO_RAD(degrees));
    }

    float tan(float degrees)
    {
      return std::tan(_DEG_TO_RAD(degrees));
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

    inline float random()
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
