/**
 * @file
 * @brief Fichero que incluye la clase CComponent_Particle_Emitter
 */

#ifndef __COMPONENT_PARTICLE_EMITTER_H_
#define __COMPONENT_PARTICLE_EMITTER_H_

#include "_globals.h"
#include "components/_component.h"

// Ver http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/ para VBOs


/** @addtogroup Componentes */
/*@{*/

/**
 * @brief Componente emisor de partículas
 *
 * El componente de emisión de partículas se encarga de crear efectos visuales tales como pantallas de humo,
 * fuegos artificiales, disparos, etc.
 *
 * <a href="http://www.youtube.com/watch?v=MWXn0I2ib0o">Enlace a vídeo.</a>
 *
 * Para ello, hemos optado por un diseño bastante sencillo compuesto de 2 partes:
 *
 * - Una cantidad determinada de partículas: cada una se tratará como una entidad (OJO, entidad, no objeto)
 *    que tendrá su posición, su velocidad, su aceleración, su velocidad angular, etc.
 * - Un contenedor que gestionará dichas partículas en cada iteración del bucle principal,
 *    calculando sus nuevas propiedades (posición, velocidad, color, vida…) y mostrándolas
 *    por pantalla en función de dichas propiedades.
 *
 * @image html components/particle_emitter/particle_emitter.png
 *
 * Una partícula, en este caso, es una textura en el mundo 3D que siempre mira a la cámara (billboard).
 * Por tanto, una partícula se define con las siguientes propiedades:
 * - Una posición en el espacio.
 * - Una velocidad.
 * - Una aceleración.
 * - Un ángulo de rotación.
 * - Una velocidad angular.
 * - Una aceleración angular (no implementado).
 * - Una escala.
 * - Un factor de escala.
 * - Un color.
 * - Un material o textura.
 * - Un tiempo de vida.
 * - Etc.
 *
 * Estos valores vendrán definidos por el componente que gestionará dichas partículas, con propiedades como:
 *
 * - Valores máximos y mínimos actuales (restricciones).
 * - Valores máximos y mínimos iniciales (aleatorio).
 * - Numero máximo de partículas.
 * - Etc.
 *
 * Finalmente, el funcionamiento del emisor de partículas es el siguiente:
 *
 * Para iniciar:
 @code
 Desde i = 0 hasta max_particles
     Crear partícula i con propiedades aleatorias.
 @endcode
 *
 * Para mostrar por pantalla
 @code
 Para cada p en particles:Si (p.life > 0) saltamos al siguiente p
     Aplicar textura y color a opengl
     Trasladarse a p.position
     Hacer que la partícula se genere como un sprite o billboard
     Rotar p.angle
     Escalar p.scale
     Dibujar p
 @code
 *
 * Para iterar:
 @code
 Si está congelado (freeze), salir.
 Para cada p en particles:Incrementar p.position a partir de p.velocity
     Incrementar p.velocity a partir de p.acceleration
     // Idem para los ángulos y similar para la escala, y otro tipo de valores (color, etc).
     Decrementar el tiempo de vida de p con el tiempo delta (diferencia de tiempo entre última iteración e iteración actual).
     Si la vida es < 0 y no está en modo parado (stop, sin generar nuevas partículas)
       Generar nueva partícula p.
 @endcode
 *
 * Dicho funcionamiento se realizará de manera automática desde el componente.
 *
 * @warning Algunos atributos están sin usar. Véase código fuente.
 * @warning El movimiento de las partículas depende siempre del tiempo.
 *
 * @see http://en.wikipedia.org/wiki/Particle_system
 */
class CComponent_Particle_Emitter: public CComponent
{
  friend class CGameObject;
  friend class CSystem_Render;

  private:
    class CParticle
    {
      friend class CComponent_Particle_Emitter;

      private:
        bool active;
        GLfloat life;//, fade;

        colorf_t color;
        vector3f_t position, velocity, acceleration;
        GLfloat angle, angle_velocity, angle_acceleration;
        GLfloat scale;
        GLfloat scale_factor;

        //string material_name;
      public:
        CParticle();
        ~CParticle();
    };

    std::vector<CParticle> particles;

    static bool InitRenderVBO();
    static void CloseRenderVBO();

    void NewParticle(CParticle& p, vector3f go_pos);

    // Aux
    float new_particles;

      // VertexArray
    // Esto lo voy a intentar hacer estático, por lo que reducirá enormemente el número de VBOs en escena
    static GLuint m_ParticlesVAO;

      // For all (divisor = 0)
    static GLuint m_ParticlesVBOVertices;

      // Per particle (divisor = 1)
    // Estaría bien, si funciona, usar un mat3 para guardar esto (9 elementos), usando así un único VBO
    static GLuint m_ParticlesVBOPosition;   // vec3
    static GLuint m_ParticlesVBOAngleScale; // vec2
    static GLuint m_ParticlesVBOColor;      // vec4

      // Used to store update info.
    std::vector<GLfloat> v_ParticlePosition_data;
    std::vector<GLfloat> v_ParticlesAngleScale_data;
    std::vector<GLfloat> v_ParticlesColor_data;

    void UpdateVBO();

    vector3f last_pos;

  protected:
    void parseDebug(std::string command);
    void printDebug();

  public:
    // Se podrían usar varios materiales, con una probabilidad P(X)c[0.f, 1.f] de que una particula use el material X
    uint max_particles;            /**< Cantidad máxima de partículas a emitir. @warning No debe cambiarse una vez iniciado el emisor. Debe pararse y ejecutarse. */
    GLfloat particles_per_second;  /**< Ratio de emisión (partículas por segundo). Puede cambiarse de manera dinámica. */
    std::string material_name;     /**< Nombre de la textura a usar para las partículas. Puede cambiarse de manera dinámica. @see CSystem_Resources @see CResource_Texture */
    //vector<string> materials; // random materials, pero esto se podría conseguir con 2 emisores de partículas...

    // Usado para CSystem_Math::random_vector(direction, angle_spreed);
    //CSystem_Math::cone_t spread_cone;

    // Propiedades del "emisor" de partículas
    bool freeze; /**< Congelar partículas (Inmovilizar, Inmortalizar). */
    bool stop;   /**< Dejar de emitir partículas. Las partículas vivas seguirán su trayectoria hasta morir. */

    // Cono
    vector3f_t direction;            /**< Dirección de emisión de partículas. Se usa para saber donde apunta la base del cono del emisor (plano de la base del cono perpendicular al vector, siendo un plano al mismo).*/
    GLfloat angle_spread;            /**< Ángulo de dispersión. Ampliación del cono de generación de partículas para generar vectores aleatorios. Representa el ángulo total, no desde un borde al centro. */

    GLfloat max_vel,                 /**< Máxima velocidad de una partícula mientras esté viva.*/
            min_vel;                 /**< Mínima velocidad de una partícula mientras esté viva. */
    GLfloat max_angle_vel,           /**< Máxima velocidad angular de una partícula mientras esté viva. */
            min_angle_vel;           /**< Mínima velocidad angular de una partícula mientras esté viva. */
    GLfloat max_scale,               /**< Máxima escala de una partícula mientras esté viva. */
            min_scale;               /**< Mínima escala de una partícula mientras esté viva. */
    //GLfloat max_live_time, min_live_time;
    colorf_t max_color,              /**< Máximo color RGBA de una partícula mientras esté viva. */
             min_color;              /**< Mínimo color RGBA de una partícula mientras esté viva. */

    // Start values
    GLfloat start_max_life_time,     /**< Tiempo máximo de vida asignado a la partícula una vez nacida. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
            start_min_life_time;     /**< Tiempo mínimo de vida asignado a la partícula una vez nacida. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
    GLfloat start_max_distance,      /**< Ditancia máxima desde el origen (punta del cono, posición del objeto ) hasta el punto sobre el vector del cono que se le ha asignado a la partícula una vez nacida. Cuanto mayor sea, más alejada del origen podrá estar la partícula al nacer. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
            start_min_distance;      /**< Ditancia mínima desde el origen (punta del cono, posición del objeto ) hasta el punto sobre el vector del cono que se le ha asignado a la partícula una vez nacida. Cuanto mayor sea, más cerca al origen podrá estar la partícula al nacer. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
    GLfloat start_max_angle,         /**< Máximo ángulo con el que nace la partícula. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
            start_min_angle;         /**< Mínimo ángulo con el que nace la partícula. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
    GLfloat start_max_angle_vel,     /**< Máxima velocidad angular con la que nace la partícula. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
            start_min_angle_vel;     /**< Mínima velocidad angular con la que nace la partícula. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
    GLfloat start_max_vel,           /**< Máxima velocidad con la que saldrá la partícula, en dirección de un vector perteneciente al cono definido por el componente. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
            start_min_vel;           /**< Mínima velocidad con la que saldrá la partícula, en dirección de un vector perteneciente al cono definido por el componente. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
    GLfloat start_max_scale,         /**< Máxima escala con la que nacerán las nuevas partículas. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
            start_min_scale;         /**< Mínima escala con la que nacerán las nuevas partículas. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
    GLfloat start_max_scale_factor,  /**< Máximo factor de escala con la que nacerán las nuevas partículas. A medida que avance el tiempo, las partículas se veran afectadas por este factor. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
            start_min_scale_factor;  /**< Mínimo factor de escala con la que nacerán las nuevas partículas. A medida que avance el tiempo, las partículas se veran afectadas por este factor. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */

    // Radio de la base
    GLfloat start_max_base_radius,   /**< Máximo radio desde el que se empezarán a emitir partículas. Hace que el emisor se comporte como un embudo, siendo la "punta" del cono una circunferencia. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
            start_min_base_radius;   /**< Mínimo radio desde el que se empezarán a emitir partículas. Hace que el emisor se comporte como un embudo, siendo la "punta" del cono una circunferencia. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */

    //colorf_t color;
    colorf_t start_max_color,        /**< Máximo color RGBA con el que nacerán las nuevas partículas. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
             start_min_color;        /**< Mínimo color RGBA con el que nacerán las nuevas partículas. Usado para generar valores aleatorios en un rango a la hora de crear partículas. */
    vector3f gravity;                /**< Gravedad que afectará a las partículas como una aceleración. Si no se quiere usar gravedad, basta con que el vector sea *(0, 0, 0)*  */

    colorf_t color_adder;            /**< Adicionador de color. Se añadirá una cierta cantidad de color por segundo al color actual, creando efectos de degradado. Puede ser tanto positivo como negativo. */


  private:
    static int GetID() { return Components::particle_emitter; }

  public:
    /**
     * @brief Constructor vacío.
     */
    CComponent_Particle_Emitter(){};

    /** @brief Constructor con objeto asociado.
     *
     * Asocia al objeto pasado como argumento el componente creado. Además, inicializa los atributos de la clase a unos ciertos valores:
     *
     @code
     material_name = "";

     freeze = stop = false;

     direction = gMath.Y_AXIS;
     angle_spread = 60;

     max_vel = 20.f;
     min_vel = 15.f;

     max_angle_vel = min_angle_vel = 0.f;
     max_scale =  10.f;
     min_scale = -10.f;

     min_color(0.f, 0.f, 0.f, 0.f);
     max_color(1.f, 1.f, 1.f, 1.f);

     // Start values
     start_max_life_time = 2.f;
     start_min_life_time = 1.f;

     start_max_distance = start_min_distance = 0.f;
     start_max_angle = start_min_angle = 0.f;

     start_max_vel = 9.f;
     start_min_vel = 8.f;

     start_min_scale = start_max_scale = 1.f;
     start_max_scale_factor = start_min_scale_factor = 0.f;
     start_max_angle_vel = 120;
     start_min_angle_vel = -120;

     start_max_base_radius = start_min_base_radius = 0.f;

     max_particles = 100;
     particles_per_second = 100;
     particles.resize(0);

     gravity(0.f, -9.81f, 0.f);
     start_max_color(1.f, 1.f, 1.f, 1.f);
     start_min_color(1.f, 1.f, 1.f, 1.f);
     color_adder(0.f, 0.f, 0.f, 0.f);

     new_particles = 0;
     @endcode
     *
     * @param gameObject Objeto que guardará el componente.
     */
    CComponent_Particle_Emitter(CGameObject* gameObject);

    /**
     * @brief Destructor.
     *
     * Destruye el componente.
     */
    ~CComponent_Particle_Emitter();
    /**
     * @brief Inicia la emisión de partículas.
     *
     * Empiez a expulsar partículas, respetando los límites y los criterios establecidos en los atributos del componente.
     *
     * @warning Elimina todas las partículas y las crea de nuevo, quitando las que estaban vivas. Para este caso, use Resume().
     */
    void Start();

    /**
     * @brief Para la emisión de partículas.
     *
     * Una vez se esten emitiendo partículas, se puede parar la emisión.
     * Por ejemplo, en la vida real, cuando se cierra un grifo, el agua que estaba saliendo
     * y que no ha caido a la bañera, terminará de caer, y quedará combinada junto con el resto.
     *
     * En este caso, las partículas emitidas que sigan con vida seguirán viviendo, hasta que mueran,
     * y no se emitirán nuevas partículas.
     */
    void Stop();
    /**
     * @brief Reanudar la emisión de partículas.
     *
     * Si se ha parado la emisión de partículas, con este método se podrá reanudar la emisión de partículas,
     * sin destruir partículas que pudiesen estar vivas mientras se generan las nuevas.
     */
    void Resume();

    /**
     * @brief Congelar las partículas.
     *
     * Hace inmortales a las partículas (no mueren) y permanecen inmutables mientras el emisor se considere como "congelado".
     */
    void Freeze();

    /**
     * @brief Descongelar las partículas.
     *
     * Hace mortales a las partículas (mueren) y dejan de ser inmutables, pasando a estar no-congelado.
     */
    void UnFreeze();

  protected:
    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);
    void OnLoop();
};

/*@}*/

#endif /* __COMPONENT_PARTICLE_EMITTER_H_ */
