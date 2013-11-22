#ifndef __COMPONENT_PARTICLE_EMITTER_H_
#define __COMPONENT_PARTICLE_EMITTER_H_

#include "_globals.h"
#include "components/_component.h"

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
        GLfloat live, fade;

        colorf_t color;
        vector3f_t position, velocity, aceleration;
        GLfloat angle, angle_velocity, angle_aceleration;
        vector3f_t scale, scale_factor;

        // ¿?
        //void OnRender();
        //void OnLoop();
    };

    vector<CParticle> particles;

    // Se podrían usar varios materiales, con una probabilidad P(X)c[0.f, 1.f] de que una particula use el material X
    string particle_material_name;

    // Usado para CSystem_Math::random_vector(direction, angle_spreed);
    vector3f_t direction;
    GLfloat angle_spreed;
    //CSystem_Math::cone_t spread_cone;

    // Propiedades del "emisor" de partículas
    bool freeze; // Congelar partículas.
    bool stop;   // Dejar de emitir partículas.

    GLfloat max_vel, min_vel;
    GLfloat max_angle_vel, min_angle_vel;
    GLfloat max_scale, min_scale;

    // max/min_position, max/min_angle, max/min_scale_factor...

    // Para cambiar el nuevo color de las partículas
    colorf_t color;

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
    CComponent_Particle_Emitter(CGameObject* gameObject){};
    ~CComponent_Particle_Emitter(){};

    void OnRender(){};
    void OnLoop(){};
};

BOOST_CLASS_EXPORT_KEY( CComponent_Particle_Emitter );


#endif /* __COMPONENT_PARTICLE_EMITTER_H_ */
