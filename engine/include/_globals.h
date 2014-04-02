/**
 * @file
 * @brief Estructuras y métodos globales.
 */

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
 * @brief Definición de excepción para devolver una excepción en una determinada línea.
 */
#define THROW(exceptionClass, message) throw exceptionClass(__FILE__, __LINE__, (message) )

/**
 * @brief Tipo personalizado de excepción. Se caracteriza por almacenar el fichero y la línea desde el lugar en el que se lanzó la excepción.
 */
class general_exception: public std::exception
{
  private:
    int line; /**< Línea del lanzamiento de la excepción.*/
    std::string file; /**< Fichero de código fuente del lanzamiento de la excepción." */
    std::string reason; /**< Razón del lanzamiento de la excepción. */

  public:
    /**
     * @brief Constructor principal de la excepción.
     * @param FILE Fichero descriptor del lugar del error.
     * @param LINE Línea descriptora del lugar error.
     * @param REASON Razón descriptora del error.
     */
    general_exception(const char* FILE, int LINE, const char* REASON): line(LINE), file(FILE), reason(REASON){ }
    /**
     * @brief Destructor. No hace nada por defecto.
     */
    ~general_exception() throw(){};

    /**
     * @brief Chivato de la excepción. Nos indica un mensaje descriptivo del error.
     * @return Devuelve una descripción en una string del error generado.
     */
    virtual const char* what() const throw()
    {
      static char output[1024];
      sprintf(output, "General exception, on file \"%s\" at line %i. Reason: %s", file.c_str(), line, reason.c_str());
      return output;
    }
};

/** Usar defines de la librería &lt;cmath&gt;.*/
#define _USE_MATH_DEFINES
/**
 * @brief Macro para pasar de radianes a grados.
 */
#define _RAD_TO_DEG(x) x * ( 180  / M_PI )

/**
 * @brief Macro para pasar de grados a radianes.
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
 * @brief Estructura de eventos (entrada, salida, ventana, etc) principal de SDL2.
 *
 * @see http://wiki.libsdl.org/SDL_Event
 */
extern SDL_Event event;
//extern const Uint8 *gKeyboardState;


/*_______OpenGL_______*/

#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
//#include <GL/glext.h>
#include <GL/SOIL.h>
#include <GL/glu.h>

/** Forzar el uso de radianes para glm.*/
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
 * @brief Título de error de renderizado para "message boxes".
 *
 * @see CSystem_Debug::msg_box()
 */
const char ERROR_RENDER[] = "Render error";
/**
 * @brief Título de error de inicialización para "message boxes".
 *
 * @see CSystem_Debug::msg_box()
 */
const char ERROR_INIT[] = "Init error";
/**
 * @brief Título de error fatal de inicialización para "message boxes".
 *
 * @see CSystem_Debug::msg_box()
 */
const char ERROR_FATAL_INIT[] = "Init fatal error";
/**
 * @brief Título de error fatal para "message boxes".
 *
 * @see CSystem_Debug::msg_box()
 */
const char ERROR_FATAL[] = "Fatal error";
/**
 * @brief Título de error de fichero para "message boxes".
 *
 * @see CSystem_Debug::msg_box()
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
 * @brief Estructura de datos para representar un color.<br>
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
 * @brief Estructura de datos para representar un color.<br>
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
   * @brief Constructor principal del color. No se comprobarán valores de entrada, asumiendo que se colocan de manera correcta (0...1).
   * @param R Componente roja del color.
   * @param G Componente verde del color.
   * @param B Componente azul del color.
   * @param A Componente alpha del color.
   */
  colorf_t(GLfloat R = 1.f, GLfloat G = 1.f, GLfloat B = 1.f, GLfloat A = 1.f): r(R), g(G), b(B), a(A){};

  /**
   * @brief Operador paréntesis. Se usa para cambiar el contenido del color (estética). No se comprobarán valores de entrada, asumiendo que se colocan de manera correcta (0...1).
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
   * @brief Multiplicador de un color. Multiplica el color por un valor flotante dado, multiplicando todas las componentes por el valor. No se comprobarán valores de entrada, asumiendo que se colocan de manera correcta (0...1).
   * @param c
   * @return
   */
  colorf_t operator*(GLfloat c)
  {
    return colorf_t(r*c, g*c, b*c, a*c);
  }

  /**
   * @brief Operador asignación-suma. Suma y asigna al color el valor c, sumando a todas las componentes el valor "c".  No se comprobarán valores de entrada, asumiendo que se colocan de manera correcta (0...1).
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
   * @brief Transforma el color en un array de tamaño 4 de valores flotantes.
   *
   * Se podrán acceder a los elementos de la siguiente manera:
   *
   * @code
    colorf_t color(1.0, 1.0, 1.0, 1.0);
    color.to_a()[0] = 0.5f; // Componente roja
    color.to_a()[1] = 0.2f; // Componente verde
    color.to_a()[2] = 0.3f; // Componente azul
    color.to_a()[2] = 1.0f; // Componente alpha
   * @endcode
   *
   * @return float* de tamaño 4, siendo una referencia al color.
   */
  float* to_a()
  {
    return (float*)this;
  }

  /**
   * @brief Color a cadena.
   *
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
   * @brief Operador de redirección de salida.
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
   * @brief Operador de redirección de entrada.
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
 * @brief Estructura para representar un "viewport" o una fracción de pantalla.
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
 * @see CComponent_Camera
 * @see CSystem_Render.
 */
typedef struct viewport_t
{
    int x;      /**< Posición en el eje x de la pantalla del viewport. */
    int y;      /**< Posición en el eje y de la pantalla del viewport. */
    int width;  /**< Ancho del viewport, expresado en píxeles. */
    int height; /**< Alto del viewport, expresado en píxeles. */

    /**
     * @brief Operador de redirección de salida.
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
     * @brief Operador de redirección de entrada.
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
     * @brief Viewport a cadena.
     *
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

/**
 * @brief Estructura para representar un "viewport" o una fracción de pantalla.
 *
 * @see http://en.wikipedia.org/wiki/Viewport
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
 * ^             width            |
 * |__>___________________________|
 * </pre>
 *
 * Se diferencia de viewport_t en que esta no usa píxeles para el tamaño, sino un valor flotante entre 0 y 1, siendo, para el ancho, "1" el ancho de la ventana, y para el alto, "1" el alto de la ventana.
 *
 * @see CComponent_Camera
 * @see CSystem_Render.
 */
typedef struct viewportf_t
{
    // Values between 0.f and 1.f
    GLfloat x;      /**< Posición en el eje x de la pantalla del viewport. Varía entre 0 y 1. Para el valor 0, el punto empieza en la parte derecha de la ventana, y para 1, se encuentra en la parte izquierda. */
    GLfloat y;      /**< Posición en el eje y de la pantalla del viewport. Varía entre 0 y 1. Para el valor 0, el punto empieza en la parte inferior de la ventana, y para 1, se encuentra en la parte superior. */
    GLfloat width;  /**< Ancho del viewport, expresado en un valor del rango [0,1], siendo "1" el ancho de la ventana (véase gSystem_Render.GetWindowSize()). */
    GLfloat height; /**< Alto del viewport, expresado en un valor del rango [0,1], siendo "1" el alto de la ventana (véase gSystem_Render.GetWindowSize()). */

    /**
     * @brief Operador de redirección de salida.
     *
     * Escribirá el viewport de la misma manera que en la función viewportf_t::str()
     * @param os Flujo de salida.
     * @param v Viewport.
     * @return Referencia al flujo de salida.
     */
    friend std::ostream& operator<<(std::ostream& os, viewportf_t v)
    {
      os << v.x << " " << v.y << " " << v.width << " " << v.height;
      return os;
    }

    /**
     * @brief Operador de redirección de entrada.
     *
     * Leerá el viewport de la misma manera que en la función viewportf_t::str()
     * @param is Flujo de entrada.
     * @param v Viewport.
     * @return Referencia al flujo de entrada.
     */
    friend std::istream& operator>>(std::istream& is, viewportf_t& v)
    {
      is >> v.x >> v.y >> v.width >> v.height;
      return is;
    }

    /**
     * @brief Viewport a cadena.
     *
     * Devuelve una representación en forma de std::string del viewport.
     * @return std::string que almacena el los datos del viewport con el formato "x y width height", manteniendo su tipo original.
     */
    std::string str()
    {
      std::stringstream ss;
      ss << x << " " << y << " " << width << " " << height;

      return ss.str();
    }

} viewportf_t;

/**
 * @brief Vector tridimensional.
 *
 * Estructura que representa un vector de flotantes con 3 componentes:
 *
 @code
  (x, y, z)
 @endcode
 *
 * Con esta clase, se puede hace gran variedad de operaciones.
 *
 * @see http://es.wikipedia.org/wiki/Vector
 * @see CSystem_Math
 */
typedef struct vector3f_t // ->NOTA La clase vector3f_t debería ir en CSystem_Math, no en ámbito global. Difícil decisión...
{
  GLfloat x; /**< Componente "x" del vector, del tipo flotante, siendo la primera componente. */
  GLfloat y; /**< Componente "y" del vector, del tipo flotante, siendo la segunda componente. */
  GLfloat z; /**< Componente "z" del vector, del tipo flotante, siendo la tercera componente. */

  /**
   * @brief Constructor sencillo.
   *
   * Crea el vector (0, 0, 0).
   */
  vector3f_t(): x(0), y(0), z(0) { };
  /**
   * @brief Constructor completo.
   *
   * Crea un vector a partir de 3 componentes (a, b, c), dando como resultado (x = a, y = b, z = c)
   * @param a Primera componente (x).
   * @param b Segunda componente (y).
   * @param c Tercera componente (z).
   */
  vector3f_t(float a, float b, float c): x(a), y(b), z(c) { };

  /**
   * @brief Constructor de copia.
   *
   * Copia un vector a partir de otro vector "v" del tipo vector3f_t, creando el vector (x = v.x, y = v.y, z = v.z).
   * @param v Vector para hacer la copia.
   */
  vector3f_t(const vector3f_t& v): x(v.x), y(v.y), z(v.z) { };

  /**
   * @brief Constructor de copia.
   *
   * Copia un vector a partir de otro vector "v" del tipo glm::vec3, creando el vector (x = v.x, y = v.y, z = v.z).
   *
   * @see http://glm.g-truc.net/0.9.5/index.html
   * @param v Vector a copiar.
   */
  vector3f_t(glm::vec3 v): x(v.x), y(v.y), z(v.z) { };

  /**
   * @brief Transformación a formato glm.
   *
   * Transforma el vector al tipo glm::vec3 para que pueda ser procesado por la librería GLM.
   *
   * @see http://glm.g-truc.net/0.9.5/index.html
   * @return Devuelve un vector tridimensional de tipo glm::vec3 con el mismo contenido que el vector.
   */
  inline glm::vec3 to_glm()
  {
    return glm::vec3(x, y, z);
  }

  /**
   * @brief Operador suma.
   *
   * Realiza la suma componente a componente del vector más el vector "v", dando como resultado (x + v.x, y + v.y, z + v.z).
   * @param v Vector a sumar.
   * @return Suma de los vectores.
   */
  vector3f_t operator+(vector3f_t v)
  {
    vector3f_t out(x + v.x,  y + v.y, z + v.z);

    return out;
  }


  /**
   * @brief Operador asignación-suma.
   *
   * Realiza la suma componente a componente del vector más el vector "v", dando como resultado (x + v.x, y + v.y, z + v.z), y le asigna el valor al vector.
   * @param v Vector a sumar.
   * @return Referencia al vector modificado.
   */
  vector3f_t& operator+=(vector3f_t v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }


  /**
   * @brief Operador resta.
   *
   * Realiza la resta componente a componente del vector menos el vector "v", dando como resultado (x - v.x, y - v.y, z - v.z).
   * @param v Vector a restar.
   * @return Resta de los vectores.
   */
  vector3f_t operator-(vector3f_t v)
  {
    vector3f_t out;
    out.x = x - v.x;
    out.y = y - v.y;
    out.z = z - v.z;
    return out;
  }

  /**
   * @brief Operador asignación-resta.
   *
   * Realiza la resta componente a componente del vector menos el vector "v", dando como resultado (x + v.x, y + v.y, z + v.z), y le asigna el valor al vector.
   * @param v Vector a restar.
   * @return Referencia al vector modificado.
   */
  vector3f_t& operator-=(vector3f_t v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  /**
   * @brief Operador de asignación.
   *
   * Le asigna al vector actual los valores del vector "v", dando como resultado (v.x, v.y, v.z).
   * @param v Vector a copiar.
   * @return Referencia al vector modificado.
   */
  vector3f_t& operator=(vector3f_t v)
  {
    x = v.x;
    y = v.y;
    z = v.z;

    return *this;
  }

  /**
   * @brief Operador producto (producto por un escalar).
   *
   * Realiza la multiplicación componente a componente del vector por el escalar "value", dando como resultado (x*value, y*value, z*value).
   * @param value Escalar a multiplicar.
   * @return Vector resultado.
   */
  vector3f_t operator*(double value)
  {
    vector3f_t out;

    out.x = x * value;
    out.y = y * value;
    out.z = z * value;
    return out;
  }

  /**
   * @brief Operador asingación-producto (producto por un escalar).
   *
   * Realiza la multiplicación componente a componente del vector por el escalar "value", dando como resultado (x*value, y*value, z*value), y le asigna el valor al vector.
   * @param value Escalar a multiplicar.
   * @return Referencia al vector modificado.
   */
  vector3f_t& operator*=(double value)
  {
    x *= value;
    y *= value;
    z *= value;

    return *this;
  }

  /**
   * @brief Operador de producto escalar.
   *
   * @see http://es.wikipedia.org/wiki/Vector#Producto_de_un_vector_por_un_escalar
   * @param v Vector a multiplicar.
   * @return Devuelve el escalar resultado de realizar el producto escalar del vector actual por el vector "v".
   */
  float operator*(vector3f_t& v)
  {
    return dot_product(v);
  }

  /**
   * @brief Operador división (división por un escalar).
   *
   * Realiza la disivión componente a componente del vector entre el escalar "value", dando como resultado (x/value, y/value, z/value).
   * @param value Escalar a dividir.
   * @return Vector resultado.
   */
  vector3f_t operator/(double value)
  {
    vector3f_t out;

    out.x = x / value;
    out.y = y / value;
    out.z = z / value;
    return out;
  }

  /**
   * @brief Operador asingación-divisón (divisón por un escalar).
   *
   * Realiza la división componente a componente del vector por el escalar "value", dando como resultado (x/value, y/value, z/value), y le asigna el valor al vector.
   * @param value Escalar a realizar la división.
   * @return Referencia al vector modificado.
   */
  vector3f_t& operator/=(double value)
  {
    x /= value;
    y /= value;
    z /= value;

    return *this;
  }

 /**
  * @brief Operador de producto cartesiano.
  *
  * @see http://es.wikipedia.org/wiki/Producto_cartesiano.
  * @param v Vector a multiplicar.
  * @return Devuelve el vector resultado de realizar el producto cartesiano del vector actual por el vector "v".
  */
  vector3f_t operator%(vector3f_t& v)
  {
    return cross_product(v);
  }

  /**
   * @brief Operador paréntesis.
   *
   * Por estética, modifica los valores de un vector de manera sencilla, tal que:
   *
   * @code
    vector3f_t v1;
    v1(1.0f, 2.0f, 30.0f); // El vector pasa a ser (1.0, 2.0, 30.0).
   * @endcode
   *
   * @param x Nueva componente x.
   * @param y Nueva componente y.
   * @param z Nueva componente z.
   */
  void operator()(GLfloat x, GLfloat y, GLfloat z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  /**
   * @brief Operador de acceso.
   *
   * Sirve para acceder a las componentes por índice. La asignación es:
   *
   * <pre>
   * 0:     x
   * 1:     y
   * 2:     z
   * resto: x
   * </pre>
   *
   * @param pos Posición o índice de la componente.
   * @return Valor de la componente. Devuelve "x" en caso de error.
   */
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

  /**
   * @brief Producto escalar.
   *
   * @see http://es.wikipedia.org/wiki/Vector#Producto_de_un_vector_por_un_escalar
   * @param v Vector a multiplicar.
   * @return Devuelve el escalar resultado de realizar el producto escalar del vector actual por el vector "v".
   */
  inline GLfloat dot_product(vector3f_t v)
  {
    return x*v.x + y*v.y + z*v.z;
  }

  /**
   * @brief Producto cartesiano.
   *
   * @see http://es.wikipedia.org/wiki/Producto_cartesiano.
   * @param v Vector a multiplicar.
   * @return Devuelve el vector resultado de realizar el producto cartesiano del vector actual por el vector "v".
   */
  inline vector3f_t cross_product(vector3f_t v)
  {
    vector3f_t out;

    out.x = y*v.z - z*v.y;
    out.y = z*v.x - x*v.z;
    out.z = x*v.y - y*v.x;

    return out;
  }

  /** Módulo del vector
   *
   * Devuelve el módulo del vector actual.
   *
   * @see http://es.wikipedia.org/wiki/M%C3%B3dulo_(vector)
   *
   * @return Valor del módulo del vector. Si el valor es "1", se dice que el vector está <b>normalizado</b> o que es un vector <b>unitario</b>.
   */
  GLfloat length()
  {
    return sqrt(x*x + y*y + z*z);
  }

  /**
   * @brief Operador de redirección de salida.
   *
   * Escribirá el vector de la misma manera que en la función vector3f_t::str()
   * @param os Flujo de salida.
   * @param v Vector.
   * @return Referencia al flujo de salida.
   */
  friend std::ostream& operator<<(std::ostream& os, vector3f_t v)
  {
    os << v.x << " " << v.y << " " << v.z;
    return os;
  }

  /**
   * @brief Operador de redirección de entrada.
   *
   * Leerá el vector de la misma manera que en la función vector3f_t::str()
   * @param is Flujo de entrada.
   * @param v Vector.
   * @return Referencia al flujo de entrada.
   */
  friend std::istream& operator>>(std::istream& is, vector3f_t& v)
  {
    is >> v.x >> v.y >> v.z;
    return is;
  }

  /**
   * @brief Normalizar el vector
   *
   * Hace que el módulo del vector valga "1" modifcando sus componentes.
   *
   * @see http://es.wikipedia.org/wiki/Vector_unitario
   * @return Vector unitario del vector actual.
   */
  vector3f_t normalize()
  {
    GLfloat v = length();
    return vector3f_t(x/v, y/v, z/v);
  }

  /**
   * @brief Vector "arriba"
   *
   * Sencillamente, es el vector "Y" o (0, 1, 0).
   * @see CSystem_Math.Y_AXIS
   * @return Devuelve el vector (0, 1, 0)
   */
  vector3f_t up()
  {
    return vector3f_t(0.f, 1.f, 0.f);
  }

  /**
   * @brief Vector "adelante"
   *
   * Sencillamente, es el vector "Z" o (0, 0, 10).
   * @see CSystem_Math.Z_AXIS
   * @return Devuelve el vector (0, 0, 1)
   */
  vector3f_t forward()
  {
    return vector3f_t(0.f, 0.f, 1.f);
  }

  /**
   * @brief Vector "izquierda"
   *
   * Sencillamente, es el vector "Y" o (1, 01, 0).
   * @see CSystem_Math.X_AXIS
   * @return Devuelve el vector (1, 0, 0)
   */
  vector3f_t left()
  {
    return vector3f_t(1.f, 0.f, 0.f);
  }

  /**
   * @brief Precisión.
   *
   * Se utiliza para comparar si 2 puntos son equivalentes, dada una precisión decimal para cada componente.
   * Así, si el valor absoluto de la diferencia entre 2 vectores es menor que la precisión para cada componente, se dice que son iguales.
   */
  static GLfloat precision;

  /**
   * @brief Vector absoluto
   *
   * Transforma cada componente para que sea positiva (valor absoluto).
   * @return Devuelve el vector transformado, tal que sea (|x|, |y|, |z|).
   */
  vector3f_t abs()
  {
    vector3f_t out;

    out.x = std::abs(x);
    out.y = std::abs(y);
    out.z = std::abs(z);

    return out;
  }

  /**
   * @brief Operador de igualdad.
   *
   * Compara si dos vectores son iguales dada la precisión vector3f_t::precision
   * @param other Otro vector a comparar.
   * @return true si son iguales, false en otro caso.
   */
  bool operator==(vector3f_t other)
  {
    //if(x != other.y or y != other.y or z != other.z)
    if(std::abs(x - other.x) >= std::abs(precision) or std::abs(y - other.y) >= std::abs(precision) or std::abs(z - other.z) >= std::abs(precision))
      return false;

    return true;
  }

  /**
    * @brief Operador de desigualdad.
    *
    * Compara si dos vectores son distintos dada la precisión vector3f_t::precision
    * @param other Otro vector a comparar.
    * @return true si son dinstitos, false en otro caso.
    */
  bool operator!=(vector3f_t other)
  {
    return !(*this == other);
  }

  /**
   * @brief Comparador de módulo.
   * @param other Vector a comparar.
   * @return true si el módulo del vector actual es menor que el módulo de other. false en otro caso.
   */
  bool operator<(vector3f_t other)
  {
    return length() < other.length();
  }

  /**
   * @brief Comparador de módulo.
   * @param other Vector a comparar.
   * @return true si el módulo del vector actual es menor o igual que el módulo de other. false en otro caso.
   */
  bool operator <=(vector3f_t other)
  {
    return *this < other or *this == other;
  }

  /**
   * @brief Comparador de módulo.
   * @param other Vector a comparar.
   * @return true si el módulo del vector actual es mayor que el módulo de other. false en otro caso.
   */
  bool operator>(vector3f_t other)
  {
    return !(*this <= other);
  }

  /**
   * @brief Comparador de módulo.
   * @param other Vector a comparar.
   * @return true si el módulo del vector actual es mayor o igual que el módulo de other. false en otro caso.
   */
  bool operator>=(vector3f_t other)
  {
    return !(*this < other);
  }

  /**
   * @brief Vector "arriba" rotado.
   *
   * Data una rotación expresada por un cuaternión (véase), devuelve el vector "arriba" rotado por dicho cuaternión.
   *
   * @see vector3f_t::up()
   * @see http://es.wikipedia.org/wiki/Cuaterni%C3%B3n
   * @param angle Cuaternión para aplicar la rotación.
   * @return Vector "arriba" rotado.
   */
  vector3f_t up(glm::quat angle)
  {
    glm::vec3 y_axis(0.f, 1.f, 0.f);

    glm::vec3 out = glm::rotate(angle, y_axis);
    return vector3f_t(out.x, out.y, out.z);
  }

  /**
   * @brief Vector "izquierda" rotado.
   *
   * Data una rotación expresada por un cuaternión (véase), devuelve el vector "izquierda" rotado por dicho cuaternión.
   *
   * @see vector3f_t::left()
   * @see http://es.wikipedia.org/wiki/Cuaterni%C3%B3n
   * @param angle Cuaternión para aplicar la rotación.
   * @return Vector "izquierda" rotado.
   */
  vector3f_t left(glm::quat angle)
  {
    glm::vec3 x_axis(1.f, 0.f, 0.f);

    glm::vec3 out = glm::rotate(angle, x_axis);
    return vector3f_t(out.x, out.y, out.z);
  }

  /**
   * @brief Vector "adelante" rotado.
   *
   * Data una rotación expresada por un cuaternión (véase), devuelve el vector "adelante" rotado por dicho cuaternión.
   *
   * @see vector3f_t::forward()
   * @see http://es.wikipedia.org/wiki/Cuaterni%C3%B3n
   * @param angle Cuaternión para aplicar la rotación.
   * @return Vector "adelante" rotado.
   */
  vector3f_t forward(glm::quat angle)
  {
    glm::vec3 z_axis(0.f, 0.f, 1.f);

    glm::vec3 out = glm::rotate(angle, z_axis);
    return vector3f_t(out.x, out.y, out.z);
  }

  /**
   * @brief Distancia entre 2 puntos.
   *
   * Dado el vector actual como un punto, calcula la distancia entre el punto actual y el punto "v".
   * @param v Punto destino.
   * @return Distancia desde el punto actual hasta "v".
   */
  double distance_to(vector3f_t v)
  {
    return ((*this) - v).length();
  }

  /**
   * @brief Vector a cadena
   *
   * Devuelve una representación en forma de std::string del vectorviewport.
   * @return std::string que almacena el los datos del vector con el formato "x y z", manteniendo su tipo original.
   */
  std::string str()
  {
    std::stringstream ss;
    ss << x << " " << y << " " << z;

    return ss.str();
  }

  /**
   * @brief Transforma el vector en un array de tamaño 3 de valores flotantes.
   *
   * Se podrán acceder a los elementos de la siguiente manera:
   *
   * @code
    vector3f v(1.0, 1.0, 1.0, 1.0);
    v.to_a()[0] = 0.5f; // Componente x
    v.to_a()[1] = 0.2f; // Componente y
    v.to_a()[2] = 0.3f; // Componente z
   * @endcode
   *
   * @return float* de tamaño 3, siendo una referencia al vector.
   */
  float* to_a()
  {
    return (float*)this;
  }

} vector3f; /**< Sinónimo para vector3f_t */


/*_____Functions______*/

//! Espacio de nombres de algunas utilidades del motor gráfico.
namespace Utils
{
  /**
   * @brief Generador de cadenas aleatorias.
   *
   * Genera una cadena de caracteres alfanuméricos ([0-9a-zA-Z]) de tamaño "n".
   * @param n Tamaño de la cadena a generar.
   * @return Cadena generada.
   */
  std::string string_generate_random_alphanumeric(uint n = 4);

  /**
   * @brief Cadena a minúsculas.
   *
   * Transforma los caracteres de una cadena de mayúsculas a minúsculas.
   * @param str Cadena a transformar.
   * @return Cadena transformada.
   */
  std::string string_to_lower(std::string& str);

  /**
   * @brief Cadena a mayúsculas.
   *
   * Transforma los caracteres de una cadena de minúsculas a mayúsculas.
   * @param str Cadena a transformar.
   * @return Cadena transformada.
   */
  std::string string_to_upper(std::string& str);

  /**
   * @brief Carga de imágenes con SDL.
   *
   * Carga una imagen en formato SDL. Se usa solo para cargar el icono de la aplicación.
   *
   * @see CEngine::SetIcon()
   * @param s Fichero que contiene la imagen.
   * @return Superficie de SDL con los datos de la imagen.
   */
  SDL_Surface* sdl_cargar_img(std::string s);

  /**
   * @brief Comprobador de validez de identificadores.
   *
   * Comprueba si un identificador es válido. Esto es, debe contener sólo caracteres alfanuméricos o guiones bajos ([0-9a-zA-Z_]).
   * @param identifier Cadena que representa un identificador a valida.
   * @return true si el identificador es válido, false en caso contrario.
   */
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

/*___Módulos_Doxygen__*/

/** @defgroup GameObjects GameObjects
 *  @brief Módulo que engloba los gameObjects. */

/** @defgroup Componentes Componentes
 *  @brief Módulo que engloba todos los componentes del motor.
 *  @addtogroup GameObjects */

/** @defgroup Sistemas Sistemas
 *  @brief Módulo que engloba todos los sistemas del motor. */

/** @defgroup Otros Otros
 *  @brief Módulo que engloba otros elementos del motor. */



#endif /* __GLOBALS_H_ */
