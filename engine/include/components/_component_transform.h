/**
 * @file
 * @brief Fichero que incluye la clase CComponent_Transform.
 */

#ifndef __COMPONENT_TRANSFORM_H_
#define __COMPONENT_TRANSFORM_H_

#include "_globals.h"
#include "systems/_other.h"
#include "components/_component.h"

/** @addtogroup Componentes */
/*@{*/

/**
 * @brief Componente de transformación.
 *
 * El componente de transformación representa la posición, la orientación y la escala de nuestro objeto en el mundo.
 *
 * @image html components/transform/transform.jpg
 *
 * En un principio, se representaban estos 3 vectores con un vector de 3 componentes flotantes, tales como:
 @code
 vector3f position;
 vector3f angle;
 vector3f scale;
 @endcode
 * Esto nos permite operar de forma sencilla sobre esos valores, con operaciones del tipo traslación, rotación y escalado (locales al eje actual del objeto, y globales (con respecto al eje X, Y, Z).
 *
 * No obstante, la representación de estos valores (en concreto, la orientación con "angle") daba una serie de problemas.
 * Uno de ellos era un manejo lento, complicado, pesado y poco eficiente. De cara al usuario, trabajar con grados sobre los ejes X, Y, Z
 * (ángulos de Euler) era muy cómodo, pero de cara al sistema, lento. Además, hay un fenómeno muy molesto llamado "Gimbal lock",
 * que, dada una combinación de rotaciones en los ejes X, Y, Z, se anula una componente de las rotaciones (es decir, rotando en 2 ejes distintos,
 * el objeto rota sobre el mismo eje en nuestro mundo). Como esto es chapuza problemática, he decidido buscar una mejor solución.
 *
 * @image html components/transform/gimbal-2.gif
 *
 * En todos los sitios relacionados al desarrollo de videojuegos hablaban de los cuaterniones, y lo
 * peor es que hablaban mal de ellos, como una pesadilla, lo cual me empujaba a buscar otras soluciones.
 * Después de numerosos intentos fallidos, he decidido usar los cuaterniones para la representación interna de la orientación de un objeto.
 * Así, evitamos el efecto “Gimbal lock” y ganamos en eficiencia para hacer rotaciones (simplemente toca multiplicar la matriz MODELVIEW actual por la matriz generada por el cuaternión actual).
 *
 * No obstante, como esta representación es bastante confusa, tenemos una simple solución:
 *
 * - De cara al sistema, se trabajará con cuaterniones con pura eficiencia.
 * - De cara al usuario (el que programará sus juegos), se le dará la posibilidad de trabajar con ángulos de Euler
 *    (intuitivos y lentos) o cuaterniones (rápidos). No obstante, en ambos casos se trabajará de manera indirecta con el segundo sistema
 *    (en el primer caso, se pasará de ángulos de Euler a cuaterniones).
 *
 * Como estos cálculos son complejos, hemos decidido ampliar nuestro repositorio de librerías y utilizar "GLM"
 * (Graphics Library Maths), una librería de sólo cabeceras para hacer operaciones matemáticas con gráficos 3D (lo usaremos para los cuaterniones, básicamente).
 * Seguramente, usemos varias funciones de esta librería para "inflar" nuestro sistema "CSystem_Math".
 *
 * Por ende, ahora se representará el componente como:
 @code
 vector3f position;
 vector3f scale;
 glm::quat angle;
 @endcode
 *
 * @see http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
 * @see http://en.wikipedia.org/wiki/Gimbal_lock
 * @see http://es.wikipedia.org/wiki/%C3%81ngulos_de_Euler
 * @see http://glm.g-truc.net/0.9.5/index.html
 */
class CComponent_Transform: public CComponent
{
  friend class CGameObject;
  friend class CSystem_Render;
  friend class CSystem_Debug;

  friend const char* Components::component_to_string(components_t c);
  friend Components::components_t Components::string_to_component(const std::string& c);

  public:
    vector3f position; /**< Posición en el espacio tridimensional. Es la posición **local** con respecto al padre del objeto que tiene el componente. La posición global vendrá dada por la aplicación de métodos como ApplyParentTransform(), entre otras operaciones. */
    vector3f scale;    /**< Escala en el espacio tridimensional. Es la escala **local** con respecto al padre del objeto que tiene el componente. La escala global vendrá dada por la aplicación de métodos como ApplyParentTransform(), entre otras operaciones. */
    glm::quat angle;   /**< Rotación en el espacio tridimensional, representada por un cuaternión. Es la rotación **local** con respecto al padre del objeto que tiene el componente. La escala global vendrá dada por la aplicación de métodos como ApplyParentTransform(), entre otras operaciones. @warning Se recomienda no modificar este valor de manera directa a no ser que se sepa lo que está haciendo. Use SetAngle(), LRotation(), Rotate() en su defecto. @see http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/ */

  private:
    static GLuint m_TransformVBOVertices;
    static GLuint m_TransformVBOColors;
    static GLuint m_TransformVAO;

    static int GetID() { return Components::transform; }

    void parseDebug(std::string command);
    void printDebug();

  protected:
    void ApplyParentTransform(CGameObject* parent, glm::mat4& transformMatrix);

  public:
    /**
     * @brief Constructor vacío.
     */
    CComponent_Transform(){};
    /** @brief Constructor con objeto asociado.
     *
     * Asocia al objeto pasado como argumento el componente creado. Además, inicializa los atributos de la clase a unos ciertos valores:
     *
     @code
     position.x = position.y = position.z = 0;
     scale.x = scale.y = scale.z = 1.f;
     // cuaternión iniciado por glm (sin rotación).
     @endcode
     *
     * @param gameObject Objeto que guardará el componente.
     */
    CComponent_Transform(CGameObject* gameObject);
    /**
     * @brief Destructor.
     *
     * Destruye el componente.
     */
    ~CComponent_Transform();

    /**
     * @brief @no_use
     *
     * @no_use
     * @param data
     */
    inline virtual void Set(input_t data);
    /**
     * @brief @no_use
     *
     * @no_use
     * @return
     */
    inline virtual output_t Get();

  public:
    // Movimientos locales o globales al eje x,y,z
    // Cambia mucha la cosa:
    //    Por ejemplo, mover rotar un objeto con hijos... ¿dónde acaban los hijos?
    //      -> Posición de los hijos LOCAL al padre (calculada o guardada) <- DECISIÓN
    //          -> Se deberá calcular de manera recursiva hacia arriba hasta llegar al mayor padre sin hijos.
    //          -> No hace falta calcular la posición y luego hacer la transformación
    //              -> Se realizarán las transformaciones recursivamente desde arriba hasta abajo, sin calcular posiciones
    // ¿qué tal un booleano para saber cuando se ha renderizado un hijo? Así, las transformaciones se realizan en cascada de arriba a abajo, pasando por todos los hijos
    //    -> Problematico? Y si no usamos DEPTH_TEST? Mejor dejarlo así

    // Transformaciones locales ¿?
    /**
     * @brief Traslación local.
     *
     * Traslada el objeto de manera local a su padre, viéndose afectada por la transformación de sus ancestros.
     * En caso de no tener padre, se considerará traslación global.
     *
     * @param x Unidades a trasladar en el eje x local al objeto.
     * @param y Unidades a trasladar en el eje y local al objeto.
     * @param z Unidades a trasladar en el eje z local al objeto.
     */
    void LTranslate(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    /**
     * @brief Rotación local.
     *
     * Rota el objeto de manera local a su padre, viéndose afectada por la transformación de sus ancestros.
     * En caso de no tener padre, se considerará rotación global.
     *
     * @param x Unidades a rotar en el eje x local al objeto, expresada en grados.
     * @param y Unidades a rotar en el eje y local al objeto, expresada en grados.
     * @param z Unidades a rotar en el eje z local al objeto, expresada en grados.
     */
    void LRotate(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0); // <- ANGULOS DE EULER Y CUATERNIONES!!!
    //void LScale(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);

    //vector3f AxisAngles();
    /**
     * @brief Ángulos de euler.
     *
     * Devuelve una representación, en ángulos de Euler, del cuaternión almacenado en angle.
     *
     * @warning Se recomienda usar LRotation() en vez de EulerAngles(), ya que es más intuitivo.
     *
     * @return Valor almacenado en angle como un vector de 3 componentes, siendo los ángulos de Euler.
     *
     * @see http://es.wikipedia.org/wiki/%C3%81ngulos_de_Euler
     */
    vector3f EulerAngles();

    // Problemática: Rotaciones locales y globales: las rotaciones "globales" funcionan como pseudolocales
    //  -> Usar cuaterniones, ya que la rotación de un eje altera el resto de ejes.
    // Problemática: Gimbal Lock
    //  -> Usar cuaterniones en vez de ángulos de eulers, y mostrar al usuario los ángulos como ángulos de euler
    //  -> Set: Euler a Cuaterniones
    //  -> Get: Cuaterniones a Euler
    //  -> Se trabaja internamente con cuaterniones. No se podrá modificar los ángulos de euler a mano (angle.x ...), ya que internamente no existen. Se usará el set y el get exclusivamente.
    //  -> http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
    //     - Nota: se puede usar glRotatef con un cuaternion?

    /**
     * @brief Traslación global.
     *
     * Traslada el objeto de manera global, moviéndose sobre los ejes x,y,z globales (movimiento libre por el mundo), con respecto al padre.
     *
     * @param v Vector de unidades a trasladar en los ejes globales.
     */
    void Translate(vector3f v);
    /**
     * @brief Traslación global.
     *
     * Traslada el objeto de manera global, moviéndose sobre los ejes x,y,z globales (movimiento libre por el mundo), con respecto al padre.
     *
     * @param x Unidades a trasladar en el eje x global.
     * @param y Unidades a trasladar en el eje y global.
     * @param z Unidades a trasladar en el eje z global.
     */
    void Translate(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    /**
     * @brief Cambiar posición global, con respecto al padre.
     *
     * Cambia la posición del objeto, que depende de la transformación de sus ancestros. Por ejemplo, si se cambiase a *(0, 0, 0)*, se colocaría en el mismo sitio en el que su padre.
     * En caso de no tener padre, se considerará posición global.
     *
     * @param v Vector que representa la nueva posición local al padre.
     */
    void SetPosition(vector3f v);
    /**
     * @brief Cambiar posición global, con respecto al padre.
     *
     * Cambia la posición del objeto, que depende de la transformación de sus ancestros. Por ejemplo, si se cambiase a *(0, 0, 0)*, se colocaría en el mismo sitio en el que su padre.
     * En caso de no tener padre, se considerará posición global.
     *
     * @param x Nueva posición en el eje x, local al padre.
     * @param y Nueva posición en el eje y, local al padre.
     * @param z Nueva posición en el eje z, local al padre.
     */
    void SetPosition(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    /**
     * @brief Rotación global.
     *
     * Rota el objeto de manera global, rotando sobre los ejes x,y,z globales (movimiento libre por el mundo), con respecto al padre.
     *
     * @param v Vector de unidades a rotar en los ejes globales, con componentes expresadas en grados.
     */
    void Rotate(vector3f v);
    /**
     * @brief Rotación global.
     *
     * Rota el objeto de manera global, rotando sobre los ejes x,y,z globales (movimiento libre por el mundo), con respecto al padre.
     *
     * @param x Unidades a rotar en el eje x global, expresadas en grados.
     * @param y Unidades a rotar en el eje y global, expresadas en grados.
     * @param z Unidades a rotar en el eje z global, expresadas en grados.
     */
    void Rotate(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    /**
     * @brief Cambiar rotación  global, con respecto al padre.
     *
     * Cambia la rotación del objeto, que depende de la rotación de sus ancestros. Por ejemplo, si se cambiase a *(0, 0, 0)*, se rotaría a la misma orientación que su padre.
     * En caso de no tener padre, se considerará rotación global.
     *
     * @param v Vector que representa la nueva rotación local al padre.
     */
    void SetAngle(vector3f v);
    /**
     * @brief Cambiar rotación  global, con respecto al padre.
     *
     * Cambia la rotación del objeto, que depende de la rotación de sus ancestros. Por ejemplo, si se cambiase a *(0, 0, 0)*, se rotaría a la misma orientación que su padre.
     * En caso de no tener padre, se considerará rotación global.
     *
     * @param x Nueva rotación en el eje x, local al padre, expresada en grados.
     * @param y Nueva rotación en el eje y, local al padre, expresada en grados.
     * @param z Nueva rotación en el eje z, local al padre, expresada en grados.
     */
    void SetAngle(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0);
    /**
     * @brief Escala global.
     *
     * Escala el objeto de manera global, escalando sobre los ejes x,y,z globales (movimiento libre por el mundo), con respecto al padre.
     *
     * @warning La escala no es aditiva, sino multiplicativa. Si se escala el valor *1.5* por *2*, tendremos *3*, y no *3.5*.
     *
     * @param v Vector de unidades a escalar en los ejes globales.
     */
    void Scale(vector3f v);
    /**
     * @brief Escala global.
     *
     * Escala el objeto de manera global, escalando sobre los ejes x,y,z globales (movimiento libre por el mundo), con respecto al padre.
     *
     * @warning La escala no es aditiva, sino multiplicativa. Si se escala el valor *1.5* por *2*, tendremos *3*, y no *3.5*.
     *
     * @param x Unidades a escalar en el eje x global.
     * @param y Unidades a escalar en el eje y global.
     * @param z Unidades a escalar en el eje z global.
     */
    void Scale(GLfloat x = 1.f, GLfloat y = 1.f, GLfloat z = 1.f);
    /**
     * @brief Cambiar escala  global, con respecto al padre.
     *
     * Cambia la escala del objeto, que depende de la transformación de sus ancestros. Por ejemplo, si se cambiase a *(1, 1, 1)*, se escalaría a igual escala que su padre.
     * En caso de no tener padre, se considerará escala global.
     *
     * @param v Vector que representa la nueva escala local al padre.
     */
    void SetScale(vector3f v);
    /**
     * @brief Cambiar escala  global, con respecto al padre.
     *
     * Cambia la escala del objeto, que depende de la transformación de sus ancestros. Por ejemplo, si se cambiase a *(1, 1, 1)*, se escalaría a igual escala que su padre.
     * En caso de no tener padre, se considerará escala global.
     *
     * @param x Nueva escala en el eje x, local al padre.
     * @param y Nueva escala en el eje y, local al padre.
     * @param z Nueva escala en el eje z, local al padre.
     */
    void SetScale(GLfloat x = 1.f, GLfloat y = 1.f, GLfloat z = 1.f);

    /**
     * @brief Apuntar a una posición
     *
     * Dado un punto en el espacio (global), se ajustará el objeto para que sus componentes apunten al punto determinado.
     *
     * @warning Se modificarán todos los ejes del objeto para apuntar. En caso de no querer usar uno, se debe deshacer de forma
     * manual con SetAngle(), quitando la/s componente/s deseada/s, y dejando las que resten.
     *
     * @param target Punto en el espacio, representado como un vector *(x, y, z)*.
     * @param up_vector Vector que apunta hacia arriba. Por defecto, es CSystem_Math::Y_AXIS (hacia arriba). Puede usarse el vector up() del componente actual para mejores resultados.
     * @param forward_vector Vector que apunta hacia delante. Por defecto, es CSystem_Math::Z_Axis (hacia delante).  Puede usarse el vector forward() del componente actual para mejores resultados.
     */
    void LookAt(vector3f target, vector3f up_vector = gMath.Y_AXIS, vector3f forward_vector = gMath.Z_AXIS);
    /**
     * @brief Apuntar a una posición
     *
     * Dado un punto en el espacio (global), se ajustará el objeto para que sus componentes apunten al punto determinado.
     *
     * @warning Se modificarán todos los ejes del objeto para apuntar. En caso de no querer usar uno, se debe deshacer de forma
     * manual con SetAngle(), quitando la/s componente/s deseada/s, y dejando las que resten.
     *
     * @param x Coordenada x del punto a mirar.
     * @param y Coordenada y del punto a mirar.
     * @param z Coordenada z del punto a mirar.
     * @param up_vector Vector que apunta hacia arriba. Por defecto, es CSystem_Math::Y_AXIS (hacia arriba). Puede usarse el vector up() del componente actual para mejores resultados.
     * @param forward_vector Vector que apunta hacia delante. Por defecto, es CSystem_Math::Z_Axis (hacia delante).  Puede usarse el vector forward() del componente actual para mejores resultados.
     */
    void LookAt(GLfloat x, GLfloat y, GLfloat z, vector3f up_vector = gMath.Y_AXIS, vector3f forward_vector = gMath.Z_AXIS);

    /**
     * @brief Aplicar transformación a una matriz de transformación.
     *
     * Aplica los datos del componente (posición, rotación y escala, en ese orden) y la de sus ancentros, empezando por el primero, y
     * acabando en el objeto actual, para generar una matriz modelview, usada internamente por openGL.
     *
     * No es necesario usarla, pero puede ser útil en algunos casos muy particulares.
     *
     * @param modelviewMatrix Matriz actual (por ejemplo, generada por la cámara).
     * @return Devuelve la matriz "modelviewMatrix" transformada por las componentes descritas anteriormente.
     */
    glm::mat4 ApplyTransform(const glm::mat4& modelviewMatrix);

    /**
     * @brief Normalizar ángulos.
     *
     * Normaliza los ángulos.
     *
     * @deprecated Al usar cuaterniones, esta función deja de tener sentido. No es conveniente usarla en ningún momento.
     */
    inline void NormalizeAngles()
    {
      if(angle.x < 0 ) angle.x = 360 + angle.x;
      if(angle.y < 0 ) angle.y = 360 + angle.y;
      if(angle.z < 0 ) angle.z = 360 + angle.z;

      if(angle.x >= 360 ) angle.x = angle.x - 360;
      if(angle.y >= 360 ) angle.y = angle.y - 360;
      if(angle.z >= 360 ) angle.z = angle.z - 360;
    }

    /**
     * @brief Vector hacia "arriba" del componente.
     *
     * Dada una rotación por angle, se generará un vector que simule la dirección "arriba" para ese objeto rotado (eje y local).
     *
     * @bug No se tiene en cuenta a los padres para este cálculo.
     *
     * @return Vector *(0.f, 1.f, 0.f)* rotado por la rotación scale.
     */
    vector3f up();
    /**
     * @brief Vector hacia la "izquierda" del componente.
     *
     * Dada una rotación por angle, se generará un vector que simule la dirección "izquierda" para ese objeto rotado (eje x local).
     *
     * @bug No se tiene en cuenta a los padres para este cálculo.
     *
     * @return Vector *(1.f, 0.f, 0.f)* rotado por la rotación scale.
     */
    vector3f left();
    /**
     * @brief Vector hacia "adelante" del componente.
     *
     * Dada una rotación por angle, se generará un vector que simule la dirección "adelante" para ese objeto rotado (eje z local).
     *
     * @bug No se tiene en cuenta a los padres para este cálculo.
     *
     * @return Vector *(0.f, 0.f, 1.f)* rotado por la rotación scale.
     */
    vector3f forward();

    /**
     * @brief Alabeo.
     *
     * Retorna la rotación local en el eje Z.
     *
     * @image html components/transform/pyr1.jpg
     *
     * @image html components/transform/pyr2.jpg
     *
     * @return Ángulo, expresado en grados, de la rotación del eje Z del componente.
     */
    GLfloat pitch();
    /**
     * @brief Guiñada.
     *
     * Retorna la rotación local en el eje Y.
     *
     * @image html components/transform/pyr1.jpg
     *
     * @image html components/transform/pyr2.jpg
     *
     * @return Ángulo, expresado en grados, de la rotación del eje Y del componente.
     */
    GLfloat yaw();
    /**
     * @brief Cabeceo.
     *
     * Retorna la rotación local en el eje X.
     *
     * @image html components/transform/pyr1.jpg
     *
     * @image html components/transform/pyr2.jpg
     *
     * @return Ángulo, expresado en grados, de la rotación del eje X del componente.
     */
    GLfloat roll();

    /**
     * @brief Rotación global.
     *
     * Devuelve la rotación global del objeto, aplicando las rotaciones de los padres (si los hubiese).
     *
     * @return Vector de 3 componentes, expresadas en grados, de la rotación global del objeto.
     */
    vector3f Rotation();
    /**
     * @brief Rotación local.
     *
     * Devuelve la rotación local del objeto a su padre. Si no hay padre, la rotación se considerará global.
     * Este método es idéntico a obtener pitch(), yaw() y roll().
     *
     * @return Vector de 3 componentes, expresadas en grados, de la rotación local del objeto.
     */
    vector3f LRotation();  // Pitch, yaw, roll

    /**
     * @brief Posición global.
     *
     * Devuelve la posición global del objeto, aplicando las transformaciones de los padres (si los hubiese).
     *
     * @return Vector de 3 componentes de la posición global del objeto.
     */
    vector3f_t Position();
    /**
     * @brief Posición local.
     *
     * Devuelve la posición local del objeto a su padre. Si no hay padre, la posición se considerará global.
     * Es idéntico a acceder al atributo position.
     *
     * @return Vector de 3 componentes de la posición local del objeto.
     */
    vector3f_t LPosition()
    {
      return position;
    }

    //vector3f_t Scalation();
    // vector3f_t LScalation();

  protected:
    void OnRender(glm::mat4 modelViewMatrix, glm::mat4 projMatrix);
    static bool InitRenderVBO();
    static void CloseRenderVBO();
};

/*@}*/

#endif /* __COMPONENT_TRANSFORM_H_ */
