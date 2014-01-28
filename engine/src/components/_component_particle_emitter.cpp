#include "systems/_render.h"
#include "systems/_other.h"
#include "systems/_resource.h"
#include "components/_component_particle_emitter.h"

BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Particle_Emitter);

CComponent_Particle_Emitter::CParticle::CParticle()
{
  active = false;
  life = -1.f;

  position = velocity = scale = acceleration = vector3f_t();
  angle = angle_velocity = angle_acceleration = scale_factor = 0.f;
}

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

  min_color = colorf_t(0.f, 0.f, 0.f, 0.f);
  max_color = colorf_t(1.f, 1.f, 1.f, 1.f);

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
  //color(1.f, 1.f, 1.f, 1.f);
  start_max_color(1.f, 1.f, 1.f, 1.f);
  start_min_color(1.f, 1.f, 1.f, 1.f);
  color_adder(0.f, 0.f, 0.f, 0.f);

  //for(vector<CParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
    //(*it) = new CParticle;
  new_particles = 0;
}

CComponent_Particle_Emitter::~CComponent_Particle_Emitter()
{
  for(vector<CParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
    delete (*it);
}

void CComponent_Particle_Emitter::Start()
{
  if(!enabled) return;

  last_pos = gameObject->Transform()->Position();

  particles.resize(max_particles);
  stop = freeze = false;

  vector3f pos_difference(0.f, 0.f, 0.f);
  vector3f current_pos = gameObject->Transform()->Position();

  if(last_pos != current_pos)
  {
    pos_difference = last_pos - current_pos;
    last_pos = current_pos;
  }

  // If it's already started, we must kill (delete) the old particles.
  if(particles.size())
  {
    for(vector<CParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
      delete (*it);

    particles.clear();
    particles.resize(max_particles);
  }

  float new_particles = particles_per_second * gSystem_Time.GetTicks_s();

  for(vector<CParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
  {
    (*it) = new CParticle;

    if((int)new_particles and (it - particles.begin()) < new_particles)  // Even if new_particles is bigger that max_particles, there will be up to max_particles updates (particles.size())
      NewParticle(*it, pos_difference);
  }
}

void CComponent_Particle_Emitter::NewParticle(CParticle* p, vector3f pos_difference)
{
  p->active = true;
  p->life = gMath.random(start_min_life_time, start_max_life_time);
  //p->color = color;
  p->color.r = gMath.random(start_min_color.r, start_max_color.r);
  p->color.g = gMath.random(start_min_color.g, start_max_color.g);
  p->color.b = gMath.random(start_min_color.b, start_max_color.b);
  p->color.a = gMath.random(start_min_color.a, start_max_color.a);

  vector3f random_vector = gMath.random_vector(direction, angle_spread/2);                // Dirección
  vector3f random_vector_XZ = vector3f(random_vector.x, 0, random_vector.z).normalize();  // Separación del origen

  p->position = random_vector * gMath.random(start_min_distance, start_max_distance) + pos_difference;
  p->position += random_vector_XZ * gMath.random(start_max_base_radius, start_min_base_radius);

  p->velocity = random_vector * gMath.random(start_min_vel, start_max_vel);
  p->acceleration = gravity;

  p->angle = gMath.random(start_min_angle, start_max_angle);
  p->angle_velocity = gMath.random(start_min_angle_vel, start_max_angle_vel);
  //angle_aceleration;

  gMath.NormalizeAngle(p->angle);

  p->scale.x = p->scale.y = p->scale.z = gMath.random(start_min_scale, start_max_scale);
  p->scale_factor = gMath.random(start_min_scale_factor, start_max_scale_factor);

  p->material_name = material_name;
}

void CComponent_Particle_Emitter::Stop()
{
  if(enabled) stop = true;
}

void CComponent_Particle_Emitter::Resume()
{
  if(enabled) stop = false;
}

void CComponent_Particle_Emitter::Freeze()
{
  if(enabled) freeze = true;
}

void CComponent_Particle_Emitter::UnFreeze()
{
  if(enabled) freeze = false;
}

void makebillboard_mat4x4(double *BM, double const * const MV)
{
    for(size_t i = 0; i < 3; i++) {

        for(size_t j = 0; j < 3; j++) {
            BM[4*i + j] = i==j ? 1 : 0;
        }
        BM[4*i + 3] = MV[4*i + 3];
    }

    for(size_t i = 0; i < 4; i++) {
        BM[12 + i] = MV[12 + i];
    }
}

// Usamos glBegin() y glEnd() en vez de VBOs, ya que
void CComponent_Particle_Emitter::OnRender()
{
  if(!enabled) return;

  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDepthMask(GL_FALSE);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  for(vector<CParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
  {
    if((*it)->life < 0.f)
      continue;

    float alpha = (*it)->color.a;
    if((*it)->life < 1.f)
      alpha *= (*it)->life;

    glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture((*it)->material_name)->GetID());
    glColor4f((*it)->color.r, (*it)->color.g, (*it)->color.b, alpha);

    glPushMatrix();

    glTranslatef((*it)->position.x, (*it)->position.y, (*it)->position.z);

    double MV[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, MV);
    makebillboard_mat4x4(MV, MV);
    glLoadMatrixd(MV);

    glRotatef((*it)->angle, 0.f, 0.f, 1.f);
    glScalef((*it)->scale.x, (*it)->scale.y, 1.f);

    glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2d(1,1); glVertex3f( 0.25f, 0.25f, 0);
      glTexCoord2d(0,1); glVertex3f(-0.25f, 0.25f, 0);
      glTexCoord2d(1,0); glVertex3f( 0.25f,-0.25f, 0);
      glTexCoord2d(0,0); glVertex3f(-0.25f,-0.25f, 0);
    glEnd();

    glPopMatrix();
  }

  glPopAttrib();
};

void CComponent_Particle_Emitter::OnLoop()
{
  if(freeze || !enabled) return;

  vector3f pos_difference(0.f, 0.f, 0.f);
  vector3f current_pos = gameObject->Transform()->Position();

  if(last_pos != current_pos)
  {
    pos_difference = last_pos - current_pos;
    last_pos = current_pos;
  }

  // Now, count how many particles should we add to the emitter (if necesary) defined by particles_per_second.
  float new_particles_iteration = particles_per_second * gSystem_Time.deltaTime_s();
  new_particles += new_particles_iteration;
  int added_particles = 0;

  for(vector<CParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
  {
    if((*it)->life >= 0 and (*it)->active)
    {
        // Update values
      (*it)->position += (*it)->velocity * gTime.deltaTime_s() + pos_difference;
      (*it)->velocity += (*it)->acceleration * gTime.deltaTime_s();
      (*it)->color += (color_adder * gTime.deltaTime_s());
      gMath.NormalizeColor((*it)->color);

      (*it)->angle += (*it)->angle_velocity * gTime.deltaTime_s();
      gMath.NormalizeAngle((*it)->angle);

      (*it)->scale.x += (*it)->scale_factor * gTime.deltaTime_s();
      (*it)->scale.y += (*it)->scale_factor * gTime.deltaTime_s();
      (*it)->scale.z += (*it)->scale_factor * gTime.deltaTime_s();

      (*it)->life -= gTime.deltaTime_s();
      //if((*it)->life < 0 ) (*it)->life = 0.f;

        // Control values
      // Speed
      // Angle
      // Scale
      // Position
      // etc
      // Color
      if((*it)->color.r < min_color.r) (*it)->color.r = min_color.r;
      if((*it)->color.g < min_color.g) (*it)->color.g = min_color.g;
      if((*it)->color.b < min_color.b) (*it)->color.b = min_color.b;
      if((*it)->color.a < min_color.a) (*it)->color.a = min_color.a;

      if((*it)->color.r > max_color.r) (*it)->color.r = max_color.r;
      if((*it)->color.g > max_color.g) (*it)->color.g = max_color.g;
      if((*it)->color.b > max_color.b) (*it)->color.b = max_color.b;
      if((*it)->color.a > max_color.a) (*it)->color.a = max_color.a;
    }
    else if((*it)->life < 0 and !stop and (int)new_particles and added_particles < (int)new_particles)
    {
      NewParticle(*it, pos_difference);
      added_particles++;
    }
  }

  if(((int)new_particles) > 0)  // If enough time has elapsed, lets reset this var.
    new_particles = 0;
};
