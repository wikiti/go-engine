#include "_systems.h"
#include "components/_component_particle_emitter.h"


BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Particle_Emitter);

CComponent_Particle_Emitter::CComponent_Particle_Emitter(CGameObject* gameObject): CComponent(gameObject)
{
  material_name = "";

  freeze = stop = false;

  direction = gMath.Y_AXIS;
  angle_spread = 60;

  max_vel = 20.f;
  min_vel = 15.f;

  max_angle_vel = min_angle_vel = 0.f;
  max_scale = min_scale = 1.f;

  // Start values
  start_max_life_time = 2.f;
  start_min_life_time = 1.f;

  start_max_distance = start_min_distance = 0.f;
  start_max_angle = start_min_angle = 0.f;

  start_max_vel = 9.f;
  start_min_vel = 8.f;

  start_min_scale = start_max_scale = 1.f;
  start_max_scale_factor = start_min_scale_factor = 1.f;

  max_particles = 100;
  particles.resize(0);

  gravity(0.f, -9.81f, 0.f);
  color(1.f, 1.f, 1.f, 1.f);
  color_adder(0.f, 0.f, 0.f, 0.f);
}

CComponent_Particle_Emitter::~CComponent_Particle_Emitter()
{

}

void CComponent_Particle_Emitter::Start()
{
  particles.resize(max_particles);
  stop = freeze = false;

  for(vector<CParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
  {
    //bool active;
    //GLfloat live, fade;
    (*it) = new CParticle;

    NewParticle(*it);
  }
}

void CComponent_Particle_Emitter::NewParticle(CParticle* p)
{
  //p = new CParticle;
  p->active = true;
  p->life = gMath.random(start_min_life_time, start_max_life_time);
  //p->fade
  p->color = color;

  vector3f random_vector = gMath.random_vector(direction, angle_spread/2);
  p->position = random_vector * gMath.random(start_min_distance, start_max_distance);
  p->velocity = random_vector * gMath.random(start_min_vel, start_max_vel);
  p->acceleration = gravity;

  p->angle = gMath.random(start_min_angle, start_max_angle);
  p->angle_velocity = gMath.random(start_min_angle_vel, start_max_angle_vel);
  //angle_aceleration;

  p->scale.x = p->scale.y = p->scale.z = gMath.random(start_min_scale, start_max_scale);
  p->scale_factor.x = p->scale_factor.y = p->scale_factor.z = gMath.random(start_min_scale_factor, start_max_scale_factor);
}

void CComponent_Particle_Emitter::Stop()
{
  stop = true;

}

void CComponent_Particle_Emitter::Resume()
{
  stop = false;
}

void CComponent_Particle_Emitter::Freeze()
{
  freeze = true;
}

void CComponent_Particle_Emitter::UnFreeze()
{
  freeze = false;
}

// Usamos glBegin() y glEnd() en vez de VBOs, ya que
void CComponent_Particle_Emitter::OnRender()
{
  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDepthMask(GL_FALSE);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture(material_name)->GetID());

  for(vector<CParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
  {
    glColor4f((*it)->color.r, (*it)->color.b, (*it)->color.b, (*it)->color.a*(*it)->life);
    //glColor4f((*it)->color.r, (*it)->color.g, (*it)->color.b, 1.f);

    //glRotatef((*it)->angle, 0.f, 0.f, 1.f);
    //glScalef((*it)->scale.x, (*it)->scale.y, (*it)->scale.z);

    glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2d(1,1); glVertex3f((*it)->position.x+0.25f,(*it)->position.y+0.25f,(*it)->position.z);
      glTexCoord2d(0,1); glVertex3f((*it)->position.x-0.25f,(*it)->position.y+0.25f,(*it)->position.z);
      glTexCoord2d(1,0); glVertex3f((*it)->position.x+0.25f,(*it)->position.y-0.25f,(*it)->position.z);
      glTexCoord2d(0,0); glVertex3f((*it)->position.x-0.25f,(*it)->position.y-0.25f,(*it)->position.z);
    glEnd();
  }

  //glEnable(GL_DEPTH_TEST);
  glPopAttrib();
  //glDisable(GL_BLEND);
};

void CComponent_Particle_Emitter::OnLoop()
{
  if(freeze) return;

  for(vector<CParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
  {
    (*it)->position += (*it)->velocity * gTime.deltaTime_s();
    (*it)->velocity += (*it)->acceleration * gTime.deltaTime_s();
    (*it)->color += (color_adder * gTime.deltaTime_s());

    (*it)->life -= gTime.deltaTime_s();

    gMath.NormalizeColor((*it)->color);

    if((*it)->life < 0 && !stop)
    {
      delete (*it);
      (*it) = new CParticle;
      NewParticle(*it);
    }
  }
};
