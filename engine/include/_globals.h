#ifndef __GLOBALS_H_
#define __GLOBALS_H_

/**_______C++ STL________**/

#include <algorithm>
#include <array>
#include <cfloat>
#include <climits>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <map>
#include <math.h>
#include <sstream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>


#define THROW(exceptionClass, message) throw exceptionClass(__FILE__, __LINE__, (message) )

class general_exception: public std::exception
{
  private:
    int line;
    std::string file, reason;

  public:
    general_exception(const char* FILE, int LINE, const char* REASON): line(LINE), file(FILE), reason(REASON){ }
    ~general_exception() throw(){};

    virtual const char* what() const throw()
    {
      static char output[1024];
      sprintf(output, "General exception, on file \"%s\" at line %i. Reason: %s", file.c_str(), line, reason.c_str());
      return output;
    }
};

#define _USE_MATH_DEFINES
#define _RAD_TO_DEG(x) x * ( 180  / M_PI )
#define _DEG_TO_RAD(x) x * ( M_PI / 180  )

/**_________BOOST________**/

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>

/**__________SDL_________**/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

extern SDL_Event event;
//extern const Uint8 *gKeyboardState;


/**________OpenGL________**/

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glext.h>
#include <GL/SOIL.h>
#include <GL/glu.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla

/**________OpenAL________**/

#include <AL/al.h>
#include <AL/alc.h>

/**______ERRORS_CHARS_____**/

const char ERROR_RENDER[] = "Render error";
const char ERROR_INIT[] = "Init error";
const char ERROR_FATAL_INIT[] = "Init fatal error";
const char ERROR_FILE[] = "File error";


/**_____GENERAL_CONST_____**/

/**_______Typedef________**/

typedef void* input_t;
typedef void* output_t;

typedef unsigned int flags_t;
typedef unsigned int uint;
typedef unsigned char ubyte;

typedef void (*fpointer)(void);
typedef bool (*fboolpointer)(void);

/**______Namespaces______**/

using namespace std;

/**_______Structs________**/

// Estaría muy bien definir un tipo de excepción propia
// Causante, objetivo, razón, instancia, etc...

typedef struct color_t
{
  ubyte r, g, b, a;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & r & g & b & a;
  }

} color_t;

typedef struct colorf_t
{
  GLfloat r, g, b, a;

  //colorf_t(): r(1.f), g(1.f), b(1.f), a(1.f){};
  colorf_t(GLfloat R = 1.f, GLfloat G = 1.f, GLfloat B = 1.f, GLfloat A = 1.f): r(R), g(G), b(B), a(A){};

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & r & g & b & a;
  }

  void operator()(GLfloat _r  = 1.f, GLfloat _g = 1.f, GLfloat _b = 1.f, GLfloat _a = 1.f)
  {
    r = _r;
    g = _g;
    b = _b;
    a = _a;
  }

  colorf_t operator*(GLfloat c)
  {
    return colorf_t(r*c, g*c, b*c, a*c);
  }

  colorf_t& operator+=(colorf_t c)
  {
    r += c.r;
    g += c.g;
    b += c.b;
    a += c.a;

    return *this;
  }

  float* to_a()
  {
    return (float*)this;
  }

} colorf_t;

typedef struct viewport_t
{
    int x, y;
    int width, height;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & x & y & width & height;
    }

} viewport_t;

typedef struct viewportf_t
{
    // Values between 0.f and 1.f
    GLfloat x, y;
    GLfloat width, height;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & x & y & width & height;
    }

} viewportf_t;

// Esto debería ir en Math...
typedef struct vector3f_t
{
  GLfloat x, y, z;

  vector3f_t(): x(0), y(0), z(0) { };
  vector3f_t(float a, float b, float c): x(a), y(b), z(c) { };
  vector3f_t(const vector3f_t& v): x(v.x), y(v.y), z(v.z) { };
  vector3f_t(glm::vec3& v): x(v.x), y(v.y), z(v.z) { };

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & x;
    ar & y;
    ar & z;
  }

  inline glm::vec3 to_glm()
  {
    return glm::vec3(x, y, z);
  }

  vector3f_t operator+(vector3f_t v)
  {
    vector3f_t out(x + v.x,  y + v.y, z + v.z);

    return out;
  }
  vector3f_t& operator+=(vector3f_t v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }
  vector3f_t operator-(vector3f_t v)
  {
    vector3f_t out;
    out.x = x - v.x;
    out.y = y - v.y;
    out.z = z - v.z;
    return out;
  }
  vector3f_t& operator-=(vector3f_t v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }
  vector3f_t& operator=(vector3f_t v)
  {
    x = v.x;
    y = v.y;
    z = v.z;

    return *this;
  }

  vector3f_t operator*(double value)
  {
    vector3f_t out;

    out.x = x * value;
    out.y = y * value;
    out.z = z * value;
    return out;
  }

  vector3f_t& operator*=(double value)
  {
    x *= value;
    y *= value;
    z *= value;

    return *this;
  }

  float operator*(vector3f_t& v)
  {
    return x*v.x + y*v.y + z*v.z;
  }

  vector3f_t operator/(double value)
  {
    vector3f_t out;

    out.x = x / value;
    out.y = y / value;
    out.z = z / value;
    return out;
  }

  vector3f_t& operator/=(double value)
  {
    x /= value;
    y /= value;
    z /= value;

    return *this;
  }

  vector3f_t operator%(vector3f_t& v)
  {
    return cross_product(v);
  }

  void operator()(GLfloat x, GLfloat y, GLfloat z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }


  float& operator[](int pos)
  {
    switch(pos)
    {
      case 0: return x;
      case 1: return y;
      case 2: return z;
      default: break;
    }
    return x;
  }

  inline GLfloat dot_product(vector3f_t v)
  {
    return x*v.x + y*v.y + z*v.z;
  }

  inline vector3f_t cross_product(vector3f_t v)
  {
    vector3f_t out;

    out.x = y*v.z - z*v.y;
    out.y = z*v.x - x*v.z;
    out.z = x*v.y - y*v.x;

    return out;
  }

  GLfloat length()
  {
    return sqrt(x*x + y*y + z*z);
  }

  friend ostream& operator<<(ostream& os, vector3f_t v)
  {
    os << v.x << " " << v.y << " " << v.z;
    return os;
  }

  friend istream& operator>>(istream& is, vector3f_t& v)
  {
    is >> v.x >> v.y >> v.z;
    return is;
  }

  vector3f_t normalize()
  {
    GLfloat v = length();
    return vector3f_t(x/v, y/v, z/v);
  }

  vector3f_t up()
  {
    return vector3f_t(0.f, 1.f, 0.f);
  }

  vector3f_t forward()
  {
    return vector3f_t(0.f, 0.f, 1.f);
  }

  vector3f_t left()
  {
    return vector3f_t(1.f, 0.f, 0.f);
  }

  bool operator==(vector3f_t other)
  {
    if(x != other.y or y != other.y or z != other.z)
      return false;

    return true;
  }

  bool operator!=(vector3f_t other)
  {
    return !(*this == other);
  }

  vector3f_t up(glm::quat angle)
  {
    glm::vec3 y_axis(0.f, 1.f, 0.f);

    glm::vec3 out = glm::rotate(angle, y_axis);
    return vector3f_t(out.x, out.y, out.z);
  }

  vector3f_t left(glm::quat angle)
  {
    glm::vec3 x_axis(1.f, 0.f, 0.f);

    glm::vec3 out = glm::rotate(angle, x_axis);
    return vector3f_t(out.x, out.y, out.z);
  }

  vector3f_t forward(glm::quat angle)
  {
    glm::vec3 z_axis(0.f, 0.f, 1.f);

    glm::vec3 out = glm::rotate(angle, z_axis);
    return vector3f_t(out.x, out.y, out.z);
  }

  double distance_to(vector3f_t v)
  {
    return ((*this) - v).length();
  }

} vector3f;


/**______Functions_______**/

namespace GO_Utils
{
  string string_generate_random_alphanumeric(uint n = 4);
  string string_to_lower(string& str);
  string string_to_upper(string& str);

  SDL_Surface* sdl_cargar_img(std::string s);
  bool validateIdentifier(string identifier);

  //void glhLookAtf2(glm::mat4& matrix, vector3f& eyePosition3D, vector3f& center3D, vector3f& upVector3D ); // http://www.opengl.org/wiki/GluLookAt_code
  //vector3f glComputeNormalOfPlane( vector3f& vec1, vector3f& vec2); // https://code.google.com/p/lightsimulator/source/browse/trunk/src/glutshadowmap.cpp?spec=svn133&r=133
}





/*typedef struct matrixf_t
{
  vector< vector<GLfloat> > matrix;

  matrixf_t(uint n, uint m)
  {
    if(!n) n = 1;
    if(!m) m = 1;

    matrix.resize(n);
    for(uint i = 0; i < n; i++)
      matrix[i].resize(m);

    for(uint i = 0; i < n; i++)
      for(uint j = 0; j < m; i++)
        matrix[i][j] = 0.f;
  }

  matrixf_t()
  {
    matrixf_t(1, 1);
  }

  uint N() { return matrix.size(); }
  uint M() { return matrix[0].size(); }

  vector<GLfloat> operator[](uint index)
  {
    return matrix[index];
  }

  matrixf_t operator*(matrixf_t m)
  {
    matrixf_t out(M(), m.N());
    if(M() == m.N())
    {
      for(uint i = 0; i < N(); i++)
        for(uint j = 0; j < m.M(); j++)
          for(uint k = 0; k < M(); j++)
            out[i][j] = out[i][j] + matrix[i][k]* m[k][j];
    }

    return out;
  }

  matrixf_t& operator=(matrixf_t m)
  {
    if(N() == m.N() && M() == m.M())
    {
      for(uint i = 0; i < N(); i++)
        for(uint j = 0; j < M(); j++)
          matrix[i][j] = m[i][j];
    }

    return *this;
  }



} matrixf;*/

#endif /* __GLOBALS_H_ */
