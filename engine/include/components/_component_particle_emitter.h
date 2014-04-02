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
 * ¡¡VÍDEO AQUÍ!!
 *
 * Para ello, hemos optado por un diseño bastante sencillo compuesto de 2 partes:
 *
 * - Una cantidad determinada de partículas: cada una se tratará como una entidad (OJO, entidad, no objeto)
 *    que tendrá su posición, su velocidad, su aceleración, su velocidad angular, etc.
 * - Un contenedor que gestionará dichas partículas en cada iteración del bucle principal,
 *    calculando sus nuevas propiedades (posición, velocidad, color, vida…) y mostrándolas
 *    por pantalla en función de dichas propiedades.
 *
 * ¡¡IMAGEN AQUÍ!!
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
    vector3f_t direction;          /**< Dirección de emisión de partículas. Se usa para saber donde apunta la base del cono del emisor (plano de la base del cono perpendicular al vector, siendo un plano al mismo).*/
    GLfloat angle_spread;          /**< Ángulo de dispersión. Ampliación del cono de generación de partículas para generar vectores aleatorios. Representa el ángulo total, no desde un borde al centro. */

    GLfloat max_vel,               /**< Máxima velocidad de una partícula mientras esté viva.*/
            min_vel;               /**< Mínima velocidad de una partícula mientras esté viva. */
    GLfloat max_angle_vel,         /**< Máxima velocidad angular de una partícula mientras esté viva. */
            min_angle_vel;         /**< Mínima velocidad angular de una partícula mientras esté viva. */
    GLfloat max_scale,             /**< Máxima escala de una partícula mientras esté viva. */
            min_scale;             /**< Mínima escala de una partícula mientras esté viva. */
    //GLfloat max_live_time, min_live_time;
    colorf_t max_color,            /**< Máximo color RGBA de una partícula mientras esté viva. */
             min_color;            /**< Mínimo color RGBA de una partícula mientras esté viva. */

    // Start values
    GLfloat start_max_life_time,
            start_min_life_time;
    GLfloat start_max_distance,
            start_min_distance;
    GLfloat start_max_angle,
            start_min_angle;
    GLfloat start_max_angle_vel,
            start_min_angle_vel;
    GLfloat start_max_vel,
            start_min_vel;
    GLfloat start_max_scale,
            start_min_scale;
    GLfloat start_max_scale_factor,
            start_min_scale_factor;

    //colorf_t color;
    colorf_t start_max_color,
             start_min_color;
    vector3f gravity;

    colorf_t color_adder;

    // Radio de la base
    GLfloat start_max_base_radius,
            start_min_base_radius;

  private:
    static int GetID() { return Components::particle_emitter; }

  public:
    CComponent_Particle_Emitter(){};
    CComponent_Particle_Emitter(CGameObject* gameObject);
    ~CComponent_Particle_Emitter();

    void Start();

    void Stop();
    void Resume();

    void Freeze();
    void UnFreeze();

  protected:
    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);
    void OnLoop();
};

/*@}*/

#endif /* __COMPONENT_PARTICLE_EMITTER_H_ */
