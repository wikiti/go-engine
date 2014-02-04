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
    CSystem_Time(): CSystem() {};

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


    // No scale, just execution
    Uint32 GetTicks()
    {
      return SDL_GetTicks();
    }

    GLfloat GetTicks_s()
    {
      return SDL_GetTicks()/1000.f;
    }

    void Pause() { Tscale = 0.f; }
    void Resume() { Tscale = 1.f; }
};

extern CSystem_Time gSystem_Time;
extern CSystem_Time& gTime;

class CSystem_Math: public CSystem
{
  public:
    static const float PI;

    static const vector3f X_AXIS;
    static const vector3f Y_AXIS;
    static const vector3f Z_AXIS;
    static const vector3f ORIGIN;

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

    void NormalizeColor(colorf_t& color)
    {
      if(color.r < 0) color.r = 0.f;
      else if(color.r > 1.f) color.r = 1.f;

      if(color.g < 0) color.g = 0.f;
      else if(color.g > 1.f) color.g = 1.f;

      if(color.b < 0) color.b = 0.f;
      else if(color.b > 1.f) color.b = 1.f;

      if(color.a < 0) color.a = 0.f;
      else if(color.a > 1.f) color.a = 1.f;
    }

    // Aritmética
    float abs(float val)
    {
      return std::abs(val);
    }

    float sqroot(float val)
    {
      return std::sqrt(val);
    }

    bool IsPowerOfTwo(unsigned int x)
    {
       return ((x != 0) && !(x & (x - 1)));
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

    inline float random(float a, float b)
    {
      return random()*(b-a) + a;
    }

    inline int random(int a, int b)
    {
      return (int)(random()*(b-a)) + a;
    }

    // vector

    double distance(vector3f_t v1, vector3f v2)
    {
      return (v1 - v2).length();
    }

  public:

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

    quat LookAt(vector3f position, vector3f target, vector3f up = vector3f(0.f, 1.f, 0.f), vector3f forward = vector3f(0.f, 0.f, 1.f))
    {
      //http://stackoverflow.com/questions/12435671/quaternion-lookat-function
      vector3f forwardVector = (target - position).normalize();

      float dot = forward * forwardVector;

      if (abs(dot - (-1.0f)) < 0.000001f)
      {
        return glm::angleAxis(3.1415926535897932f, up.to_glm());
      }
      if (abs(dot - (1.0f)) < 0.000001f)
      {
        return glm::quat();
      }

      float rotAngle = (float)acos(dot);
      vector3f rotAxis = forward % forwardVector;
      normalize(rotAxis);

      return glm::angleAxis(rotAngle, rotAxis.to_glm());
    }

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

    // Random point - Sphere
    vector3f random_point(GLfloat sphere_radius, vector3f origin = ORIGIN)
    {
      return origin + (random_vector(Y_AXIS, 180)*(random()*sphere_radius));
    }

    // Random point - Cuboid
    vector3f random_point(vector3f dimensions, vector3f origin = ORIGIN)
    {
      return vector3f(origin.x + random()*dimensions.x, origin.y + random()*dimensions.y, origin.z + random()*dimensions.z);
    }

};

extern CSystem_Math gSystem_Math;
extern CSystem_Math& gMath;


#endif /* __CSYSTEM_OTHER_H_ */
