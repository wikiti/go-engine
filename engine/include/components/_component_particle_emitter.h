#ifndef __COMPONENT_PARTICLE_EMITTER_H_
#define __COMPONENT_PARTICLE_EMITTER_H_

#include "_globals.h"
#include "components/_component.h"

// Ver http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/ para VBOs

// falta activar los valores "maximos" durante la emisión de la partícula.
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

    vector<CParticle> particles;

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
    vector<GLfloat> v_ParticlePosition_data;
    vector<GLfloat> v_ParticlesAngleScale_data;
    vector<GLfloat> v_ParticlesColor_data;

    void UpdateVBO();

    vector3f last_pos;

  protected:
    void parseDebug(string command);
    void printDebug();

  public:
    // GameObject stuff

    // Se podrían usar varios materiales, con una probabilidad P(X)c[0.f, 1.f] de que una particula use el material X
    uint max_particles;
    GLfloat particles_per_second;
    string material_name;
    //vector<string> materials; // random materials, pero esto se podría conseguir con 2 emisores de partículas...

    // Usado para CSystem_Math::random_vector(direction, angle_spreed);
    //CSystem_Math::cone_t spread_cone;

    // Propiedades del "emisor" de partículas
    bool freeze; // Congelar partículas.
    bool stop;   // Dejar de emitir partículas.

    // Values
    vector3f_t direction;
    GLfloat angle_spread;

    GLfloat max_vel, min_vel;
    GLfloat max_angle_vel, min_angle_vel;
    GLfloat max_scale, min_scale;
    //GLfloat max_live_time, min_live_time;
    colorf_t max_color, min_color;

    // Start values
    GLfloat start_max_life_time, start_min_life_time;
    GLfloat start_max_distance, start_min_distance;
    GLfloat start_max_angle, start_min_angle;
    GLfloat start_max_angle_vel, start_min_angle_vel;
    GLfloat start_max_vel, start_min_vel;
    GLfloat start_max_scale, start_min_scale;
    GLfloat start_max_scale_factor, start_min_scale_factor;

    //colorf_t color;
    colorf_t start_max_color, start_min_color;
    vector3f gravity;

    colorf_t color_adder;

    // Radio de la base
    GLfloat start_max_base_radius;
    GLfloat start_min_base_radius;

  private:
    static int GetID() { return Components::particle_emitter; }

    /*friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & boost::serialization::base_object<CComponent>(*this);
    }*/

  public:
    CComponent_Particle_Emitter(){};
    CComponent_Particle_Emitter(CGameObject* gameObject);
    ~CComponent_Particle_Emitter();

    void Start();

    void Stop();
    void Resume();

    void Freeze();
    void UnFreeze();

    // Usamos glBegin() y glEnd() en vez de VBOs, ya que
    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);
    void OnLoop();
};

//BOOST_CLASS_EXPORT_KEY( CComponent_Particle_Emitter );


#endif /* __COMPONENT_PARTICLE_EMITTER_H_ */
