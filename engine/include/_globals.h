#ifndef __GLOBALS_H_
#define __GLOBALS_H_

/*______C++ STL_______*/

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
#include <ctime>
#include <vector>

/**
 * Definición de excepción para devolver una excepción en una determinada línea.
 */
#define THROW(exceptionClass, message) throw exceptionClass(__FILE__, __LINE__, (message) )

/**
 * Tipo personalizado de excepción. Se caracteriza por almacenar el fichero y la línea desde el lugar en el que se lanzó la excepción.
 */
class general_exception: public std::exception
{
  private:
    int line; /**< Línea del lanzamiento de la excepción.*/
    std::string file; /**< Fichero de código fuente del lanzamiento de la excepción." */
    std::string reason; /**< Razón del lanzamiento de la excepción. */

  public:
    /**
     * Constructor principal de la excepción.
     * @param FILE Fichero descriptor del lugar del error.
     * @param LINE Línea descriptora del lugar error.
     * @param REASON Razón descriptora del error.
     */
    general_exception(const char* FILE, int LINE, const char* REASON): line(LINE), file(FILE), reason(REASON){ }
    /**
     * Destructor. No hace nada por defecto.
     */
    ~general_exception() throw(){};

    /**
     * Chivato de la excepción. Nos indica un mensaje descriptivo del error.
     * @return Devuelve una descripción en una string del error generado.
     */
    virtual const char* what() const throw()
    {
      static char output[1024];
      sprintf(output, "General exception, on file \"%s\" at line %i. Reason: %s", file.c_str(), line, reason.c_str());
      return output;
    }
};

#define _USE_MATH_DEFINES
/**
 * Macro para pasar de radianes a grados.
 */
#define _RAD_TO_DEG(x) x * ( 180  / M_PI )

/**
 * Macro para pasar de grados a radianes.
 */
#define _DEG_TO_RAD(x) x * ( M_PI / 180  )

/*________BOOST_______*/

/*#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>*/

/*_________SDL________*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

/**
 * Estructura de eventos (entrada, salida, ventana, etc) principal de SDL2. Para más información, véase el siguiente enlace:
 *
 * <ol><li>http://wiki.libsdl.org/SDL_Event</ol>
 *
 *
 */
extern SDL_Event event;
//extern const Uint8 *gKeyboardState;


/*_______OpenGL_______*/

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
//#include <GL/glext.h>
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

/*_______OpenAL_______*/

#include <AL/al.h>
#include <AL/alc.h>

/*_____ERRORS_CHARS____*/

/**
 * Título de error de renderizado para "message boxes". Véase CSystem_Debug::msg_box.
 */
const char ERROR_RENDER[] = "Render error";
/**
 * Título de error de inicialización para "message boxes". Véase CSystem_Debug::msg_box.
 */
const char ERROR_INIT[] = "Init error";
/**
 * Título de error fatal de inicialización para "message boxes". Véase CSystem_Debug::msg_box.
 */
const char ERROR_FATAL_INIT[] = "Init fatal error";
/**
 * Título de error fatal para "message boxes". Véase CSystem_Debug::msg_box.
 */
const char ERROR_FATAL[] = "Fatal error";
/**
 * Título de error de fichero para "message boxes". Véase CSystem_Debug::msg_box.
 */
const char ERROR_FILE[] = "File error";


/*____GENERAL_CONST____*/

/*______Typedef_______*/

typedef void* input_t;  /**< Tipo para representar una entrada genérica.*/
typedef void* output_t; /**< Tipo para representar una salida genérica.*/

typedef unsigned int flags_t;  /**< Tipo para representar flags u opciones.*/
typedef unsigned int uint;     /**< Tipo para representar un entero sin signo.*/
typedef unsigned char ubyte;   /**< Tipo para representar un byte sin signo.*/

typedef void (*fpointer)(void);     /**< Tipo para representar un puntero a función del tipo "void f()".*/
typedef bool (*fboolpointer)(void); /**< Tipo para representar un puntero a función del tipo "bool f()".*/

/*_____Namespaces_____*/

//using namespace std;

/*______Structs_______*/

// Estaría muy bien definir un tipo de excepción propia
// Causante, objetivo, razón, instancia, etc...

/**
 * Estructura de datos para representar un color.<br>
 *
 * El contenido será representado como un color en el espacio RGB, con formato "RGBA", siendo cada componente un byte sin signo (0...255).
 */
typedef struct color_t
{
  ubyte r; /**< Componente roja del color. Valor entre 0 y 255. */
  ubyte g; /**< Componente verde del color. Valor entre 0 y 255. */
  ubyte b; /**< Componente azul del color. Valor entre 0 y 255. */
  ubyte a; /**< Componente alpha del color. Valor entre 0 y 255. */

} color_t;

/**
 * Estructura de datos para representar un color.<br>
 *
 * El contenido será representado como un color en el espacio RGB, con formato "RGBA", siendo cada componente un flotante sin signo (0...1).
 *
 * No se comprobarán valores de entrada, asumiendo que se colocan de manera correcta (0...1).
 */
typedef struct colorf_t
{
  GLfloat r; /**< Componente roja del color. Valor flotante entre 0 y 1. */
  GLfloat g; /**< Componente verde del color. Valor flotante entre 0 y 1. */
  GLfloat b; /**< Componente azul del color. Valor flotante entre 0 y 1. */
  GLfloat a; /**< Componente alpha del color. Valor flotante entre 0 y 1. */

  //colorf_t(): r(1.f), g(1.f), b(1.f), a(1.f){};
  /**
   * Constructor principal del color. No se comprobarán valores de entrada, asumiendo que se colocan de manera correcta (0...1).
   * @param R Componente roja del color.
   * @param G Componente verde del color.
   * @param B Componente azul del color.
   * @param A Componente alpha del color.
   */
  colorf_t(GLfloat R = 1.f, GLfloat G = 1.f, GLfloat B = 1.f, GLfloat A = 1.f): r(R), g(G), b(B), a(A){};

  /**
   * Operador paréntesis. Se usa para cambiar el contenido del color (estética). No se comprobarán valores de entrada, asumiendo que se colocan de manera correcta (0...1).
   * @param _r Componente roja del color.
   * @param _g Componente verde del color.
   * @param _b Componente azul del color.
   * @param _a Componente alpha del color.
   */
  void operator()(GLfloat _r  = 1.f, GLfloat _g = 1.f, GLfloat _b = 1.f, GLfloat _a = 1.f)
  {
    r = _r;
    g = _g;
    b = _b;
    a = _a;
  }

  /**
   * Multiplicador de un color. Multiplica el color por un valor flotante dado, multiplicando todas las componentes por el valor. No se comprobarán valores de entrada, asumiendo que se colocan de manera correcta (0...1).
   * @param c
   * @return
   */
  colorf_t operator*(GLfloat c)
  {
    return colorf_t(r*c, g*c, b*c, a*c);
  }

  /**
   * Operador asignación-suma. Suma y asigna al color el valor c, sumando a todas las componentes el valor "c".  No se comprobarán valores de entrada, asumiendo que se colocan de manera correcta (0...1).
   * @param c Valor flotante a sumar.
   * @return Devuelve el color en sí (referencia).
   */
  colorf_t& operator+=(colorf_t c)
  {
    r += c.r;
    g += c.g;
    b += c.b;
    a += c.a;

    return *this;
  }

  /**
   * Transforma el color en un array de tamaño 4 de valores flotantes.
   *
   * Se podrán acceder a los elementos de la siguiente manera:
   *
   * @code
   * colorf_t color(1.0, 1.0, 1.0, 1.0);
   * color.to_a()[0] = 0.5f; // Componente roja
   * color.to_a()[1] = 0.2f; // Componente verde
   * color.to_a()[2] = 0.3f; // Componente azul
   * color.to_a()[2] = 1.0f; // Componente alpha
   * @endcode
   *
   * @return float* de tamaño 4, siendo una referencia al color.
   */
  float* to_a()
  {
    return (float*)this;
  }

  /**
   * Devuelve una representación en forma de std::string del color.
   * @return std::string que almacena el color con el formato "r g b a", con cada elemento como un valor flotante.
   */
  std::string str()
  {
    std::stringstream ss;
    ss << r << " " << g << " " << b << " " << a;

    return ss.str();
  }

  /**
   * Operador de redirección de salida.
   *
   * Escribirá el color de la misma manera que en la función colorf_t::str()
   * @param os Flujo de salida.
   * @param c Color.
   * @return Referencia al flujo de salida.
   */
  friend std::ostream& operator<<(std::ostream& os, colorf_t c)
  {
    os << c.r << " " << c.g << " " << c.b << " " << c.a;
    return os;
  }

  /**
   * Operador de redirección de entrada.
   *
   * leerá el color de la misma manera que en la función colorf_t::str()
   * @param is Flujo de entrada.
   * @param c Color.
   * @return Referencia al flujo de entrada.
   */
  friend std::istream& operator>>(std::istream& is, colorf_t& c)
  {
    is >> c.r >> c.g >> c.b >> c.a;
    return is;
  }

} colorf_t;

/**
 * Estructura para representar un "viewport" o una fracción de pantalla.
 *
 * http://en.wikipedia.org/wiki/Viewport
 *
 * Un viewport no es más que una abstracción de una "ventana" o portal desde la ventana de nuestro programa hasta el mundo en 3D de la aplicación.
 * Básicamente, es una porción de pantalla (definida por una posición y un tamaño) en la que se dibujarán ciertos elementos del juego.
 *
 * En una ventana, un viewport se define más o menos así:
 *
 * <pre>
 *  ______________________________
 * |                              |
 * |      x,y_____________        |
 * |        |             |       |
 * |        |             |height |
 * |        |_____________|       |
 * |             width            |
 * |______________________________|
 * </pre>
 *
 * Para más información, véase CComponent_Camera y CSystem_Render.
 */
typedef struct viewport_t
{
    int x;      /**< Posición en el eje x de la pantalla del viewport. */
    int y;      /**< Posición en el eje y de la pantalla del viewport. */
    int width;  /**< Ancho del viewport, expresado en píxeles. */
    int height; /**< Alto del viewport, expresado en píxeles. */

    /**
     * Operador de redirección de salida.
     *
     * Escribirá el viewport de la misma manera que en la función viewport_t::str()
     * @param os Flujo de salida.
     * @param v Viewport.
     * @return Referencia al flujo de salida.
     */
    friend std::ostream& operator<<(std::ostream& os, viewport_t v)
    {
      os << v.x << " " << v.y << " " << v.width << " " << v.height;
      return os;
    }

    /**
     * Operador de redirección de salida.
     *
     * Leerá el viewport de la misma manera que en la función viewport_t::str()
     * @param is Flujo de entrada.
     * @param v Viewport.
     * @return Referencia al flujo de entrada.
     */
    friend std::istream& operator>>(std::istream& is, viewport_t& v)
    {
      is >> v.x >> v.y >> v.width >> v.height;
      return is;
    }

    /**
     * Devuelve una representación en forma de std::string del viewport.
     * @return std::string que almacena el los datos del viewport con el formato "x y width height", manteniendo su tipo original.
     */
    std::string str()
    {
      std::stringstream ss;
      ss << x << " " << y << " " << width << " " << height;

      return ss.str();
    }

} viewport_t;

typedef struct viewportf_t
{
    // Values between 0.f and 1.f
    GLfloat x, y;
    GLfloat width, height;

    friend std::ostream& operator<<(std::ostream& os, viewportf_t v)
    {
      os << v.x << " " << v.y << " " << v.width << " " << v.height;
      return os;
    }

    friend std::istream& operator>>(std::istream& is, viewportf_t& v)
    {
      is >> v.x >> v.y >> v.width >> v.height;
      return is;
    }

    std::string str()
    {
      std::stringstream ss;
      ss << x << " " << y << " " << width << " " << height;

      return ss.str();
    }

} viewportf_t;

// Esto debería ir en Math...
typedef struct vector3f_t
{
  GLfloat x, y, z;

  vector3f_t(): x(0), y(0), z(0) { };
  vector3f_t(float a, float b, float c): x(a), y(b), z(c) { };
  vector3f_t(const vector3f_t& v): x(v.x), y(v.y), z(v.z) { };
  //vector3f_t(const glm::vec3& v): x(v.x), y(v.y), z(v.z) { };
  vector3f_t(glm::vec3 v): x(v.x), y(v.y), z(v.z) { };

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

  friend std::ostream& operator<<(std::ostream& os, vector3f_t v)
  {
    os << v.x << " " << v.y << " " << v.z;
    return os;
  }

  friend std::istream& operator>>(std::istream& is, vector3f_t& v)
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

  static GLfloat precision;

  vector3f_t abs()
  {
    vector3f_t out;

    out.x = std::abs(x);
    out.y = std::abs(y);
    out.z = std::abs(z);

    return out;
  }

  bool operator==(vector3f_t other)
  {
    //if(x != other.y or y != other.y or z != other.z)
    if(std::abs(x - other.x) >= std::abs(precision) or std::abs(y - other.y) >= std::abs(precision) or std::abs(z - other.z) >= std::abs(precision))
      return false;

    return true;
  }

  bool operator<(vector3f_t other)
  {
    return length() < other.length();
  }

  bool operator <=(vector3f_t other)
  {
    return *this < other or *this == other;
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

  std::string str()
  {
    std::stringstream ss;
    ss << x << " " << y << " " << z;

    return ss.str();
  }

} vector3f;


/*_____Functions______*/

namespace GO_Utils
{
  std::string string_generate_random_alphanumeric(uint n = 4);
  std::string string_to_lower(std::string& str);
  std::string string_to_upper(std::string& str);

  SDL_Surface* sdl_cargar_img(std::string s);
  bool validateIdentifier(std::string identifier);

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
