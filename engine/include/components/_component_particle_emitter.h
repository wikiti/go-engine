#ifndef __COMPONENT_PARTICLE_EMITTER_H_
#define __COMPONENT_PARTICLE_EMITTER_H_

#include "_globals.h"
#include "components/_component.h"

// falta activar los valores "maximos" durante la emisión de la partícula.
class CComponent_Particle_Emitter: public CComponent
{
  public:
    friend class CGameObject;

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
        vector3f_t scale;
        GLfloat scale_factor;

        string material_name;
    };

    vector<CParticle*> particles;

    void NewParticle(CParticle* p, vector3f go_pos);

  public:
    // GameObject stuff
    vector3f last_pos;

    // Se podrían usar varios materiales, con una probabilidad P(X)c[0.f, 1.f] de que una particula use el material X
    uint max_particles;
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
    static int GetID() { return components::particle_emitter; }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & boost::serialization::base_object<CComponent>(*this);
    }

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
    void OnRender();
    void OnLoop();
};

BOOST_CLASS_EXPORT_KEY( CComponent_Particle_Emitter );


#endif /* __COMPONENT_PARTICLE_EMITTER_H_ */
