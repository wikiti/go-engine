#include "systems/_render.h"
#include "systems/_other.h"
#include "systems/_resource.h"
#include "systems/_shader.h"
#include "components/_component_particle_emitter.h"

//BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Particle_Emitter);

CComponent_Particle_Emitter::CParticle::CParticle()
{
  active = false;
  life = -1.f;

  position = velocity = acceleration = vector3f_t();
  angle = angle_velocity = angle_acceleration = scale = scale_factor = 0.f;
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

  v_ParticlePosition_data.clear();
  v_ParticlesAngleScale_data.clear();
  v_ParticlesColor_data.clear();

  glDeleteBuffers(1, &m_ParticlesVBOVertices);
  glDeleteBuffers(1, &m_ParticlesVBOTexCoords);

  glDeleteBuffers(1, &m_ParticlesVBOPosition);
  glDeleteBuffers(1, &m_ParticlesVBOAngleScale);
  glDeleteBuffers(1, &m_ParticlesVBOColor);

  glDeleteVertexArrays(1, &m_ParticlesVAO);
}

void CComponent_Particle_Emitter::Start()
{
  if(!enabled) return;

  last_pos = gameObject->Transform()->Position();

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
    //particles.resize(max_particles);
  }

  particles.resize(max_particles);

  float new_particles = particles_per_second * gSystem_Time.GetTicks_s();

  for(vector<CParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
  {
    (*it) = new CParticle;

    if((int)new_particles and (it - particles.begin()) < new_particles)  // Even if new_particles is bigger that max_particles, there will be up to max_particles updates (particles.size())
      NewParticle(*it, pos_difference);
  }

  v_ParticlePosition_data.clear();
  v_ParticlesAngleScale_data.clear();
  v_ParticlesColor_data.clear();

  v_ParticlePosition_data.resize(max_particles*3);
  v_ParticlesAngleScale_data.resize(max_particles*2);
  v_ParticlesColor_data.resize(max_particles*4);


  glGenVertexArrays(1, &m_ParticlesVAO);
  if(!m_ParticlesVAO)
  {
    gSystem_Debug.error("From CComponent_Particle_Emitter: Could not generate Particle Emitter VAO.");
  }

  glBindVertexArray(m_ParticlesVAO);

  glGenBuffers(1, &m_ParticlesVBOVertices);
  glGenBuffers(1, &m_ParticlesVBOTexCoords);

  glGenBuffers(1, &m_ParticlesVBOPosition);
  glGenBuffers(1, &m_ParticlesVBOAngleScale);
  glGenBuffers(1, &m_ParticlesVBOColor);

  if(!m_ParticlesVBOVertices or !m_ParticlesVBOTexCoords or !m_ParticlesVBOPosition or !m_ParticlesVBOAngleScale or !m_ParticlesVBOColor)
  {
    gSystem_Debug.error("From CComponent_Particle_Emitter: Could not generate Particle Emitter VBO.");

    glDeleteBuffers(1, &m_ParticlesVBOVertices);
    glDeleteBuffers(1, &m_ParticlesVBOTexCoords);

    glDeleteBuffers(1, &m_ParticlesVBOPosition);
    glDeleteBuffers(1, &m_ParticlesVBOAngleScale);
    glDeleteBuffers(1, &m_ParticlesVBOColor);

    glDeleteVertexArrays(1, &m_ParticlesVAO);
  }

  const GLfloat p_ParticleVertices [][3] =
  {
      { 0.25f, 0.25f, 0.f}, {-0.25f, 0.25f, 0.f},
      { 0.25f,-0.25f, 0.f}, {-0.25f,-0.25f, 0.f}
  };

  const GLfloat p_ParticleTexCoords [][2] =
  {
      {1,1}, {0,1},
      {1,0}, {0,0}
  };

  // This will remain static, so we use GL_STATIC_DRAW
  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOVertices);
  glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), p_ParticleVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOTexCoords);
  glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), p_ParticleTexCoords, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

  // los 2 VBOs de arriban se pueden poner como constantes dentro del shader, ya que permanecen intactos

  // Other, not inited yet
  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOPosition);
  glBufferData(GL_ARRAY_BUFFER, particles.size() * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOAngleScale);
  glBufferData(GL_ARRAY_BUFFER, particles.size() * 2 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOColor);
  glBufferData(GL_ARRAY_BUFFER, particles.size() * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, 0, 0);

  glVertexAttribDivisor(0, 0);
  glVertexAttribDivisor(1, 0);
  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);
  glVertexAttribDivisor(4, 1);

  glBindVertexArray(0);
}

void CComponent_Particle_Emitter::UpdateVBO()
{
  glBindVertexArray(m_ParticlesVAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOPosition);
  glBufferData(GL_ARRAY_BUFFER, particles.size() * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * 3 * sizeof(GLfloat), &v_ParticlePosition_data[0]);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOAngleScale);
  glBufferData(GL_ARRAY_BUFFER, particles.size() * 2 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * 2 * sizeof(GLfloat), &v_ParticlesAngleScale_data[0]);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOColor);
  glBufferData(GL_ARRAY_BUFFER, particles.size() * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * 4 * sizeof(GLfloat), &v_ParticlesColor_data[0]);
  glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, 0, 0);

  //glBindVertexArray(0);
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

  p->scale = gMath.random(start_min_scale, start_max_scale);
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

// No se si funciona, hay que probarlo
void makebillboardGLM(glm::mat4& matrix)
{
  glm::mat4 output;
  for(size_t i = 0; i < 3; i++)
  {
    for(size_t j = 0; j < 3; j++)
      output[i][j] = i == j ? 1 : 0;
    output[i][3] = matrix[i][3];
  }

  for(size_t i = 0; i < 4; i++)
    output[3][i] = matrix[3][i];

  matrix = output;
}

// Usamos glBegin() y glEnd() en vez de VBOs, ya que
void CComponent_Particle_Emitter::OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix)
{
  if(!enabled) return;

  glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glDepthMask(GL_FALSE);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  //glBlendFunc(GL_DST_COLOR, GL_ONE);

  UpdateVBO();

  glActiveTexture(GL_TEXTURE0);
  if(material_name != "") glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture(material_name)->GetID());
  else                    glBindTexture(GL_TEXTURE_2D, 0); // ¿?

  CShader* shader = gSystem_Shader_Manager.UseShader("__particlesShader");
  glUniformMatrix4fv(shader->GetUniformIndex("ProjMatrix") , 1, GL_FALSE, glm::value_ptr(projMatrix));
  glUniformMatrix4fv(shader->GetUniformIndex("ModelViewMatrix") , 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
  glUniform1i(shader->GetUniformIndex("texture"), 0);
  glUniform1f(shader->GetUniformIndex("textureFlag"), 1.f);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOVertices);
  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOTexCoords);

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOPosition);
  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOAngleScale);
  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOColor);



  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles.size());

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(4);

  glBindVertexArray(0);

  /*for(vector<CParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
  {
    if((*it)->life < 0.f)
      continue;

    // Esto lo voy a poner en OnLoop()
    float alpha = (*it)->color.a;
    if((*it)->life < 1.f)
      alpha *= (*it)->life;

    glBindTexture(GL_TEXTURE_2D, gSystem_Resources.GetTexture((*it)->material_name)->GetID());
    glColor4f((*it)->color.r, (*it)->color.g, (*it)->color.b, alpha);

    glPushMatrix();

    glTranslatef((*it)->position.x, (*it)->position.y, (*it)->position.z);

    // El billboarding debería hacerse aquí
    double MV[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, MV);
    makebillboard_mat4x4(MV, MV);
    glLoadMatrixd(MV);

    glRotatef((*it)->angle, 0.f, 0.f, 1.f);
    glScalef((*it)->scale, (*it)->scale, 1.f);

    glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2d(1,1); glVertex3f( 0.25f, 0.25f, 0);
      glTexCoord2d(0,1); glVertex3f(-0.25f, 0.25f, 0);
      glTexCoord2d(1,0); glVertex3f( 0.25f,-0.25f, 0);
      glTexCoord2d(0,0); glVertex3f(-0.25f,-0.25f, 0);
    glEnd();

    glPopMatrix();
  }*/

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

      (*it)->scale += (*it)->scale_factor * gTime.deltaTime_s();
      //(*it)->scale.y += (*it)->scale_factor * gTime.deltaTime_s();
      //(*it)->scale.z += (*it)->scale_factor * gTime.deltaTime_s(); // <-- useless

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


      // Update VBO aux variables
      int index = it - particles.begin();

      // Position
      v_ParticlePosition_data[3*index + 0] = (*it)->position.x;
      v_ParticlePosition_data[3*index + 1] = (*it)->position.y;
      v_ParticlePosition_data[3*index + 2] = (*it)->position.z;

      // Angle and scale
      v_ParticlesAngleScale_data[2*index + 0] = (*it)->angle;
      v_ParticlesAngleScale_data[2*index + 1] = (*it)->scale;

      // Color
      float alpha = (*it)->color.a;
      if((*it)->life < 1.f)
        alpha *= (*it)->life;

      v_ParticlesColor_data[4*index + 0] = (*it)->color.r;
      v_ParticlesColor_data[4*index + 1] = (*it)->color.g;
      v_ParticlesColor_data[4*index + 2] = (*it)->color.b;
      v_ParticlesColor_data[4*index + 3] = alpha;
    }
    else if((*it)->life < 0 and !stop and (int)new_particles and added_particles < (int)new_particles)
    {
      NewParticle(*it, pos_difference);
      added_particles++;
    }
  }

  if(((int)new_particles) > 0)  // If enough time has elapsed, lets reset this var.
    new_particles = 0;

  /*v_ParticlesColor_data[0] = 1.f;
  v_ParticlesColor_data[1] = 0.f;
  v_ParticlesColor_data[2] = 0.f;
  v_ParticlesColor_data[3] = 1.f;

  v_ParticlesColor_data[4] = 1.f;
  v_ParticlesColor_data[5] = 0.f;
  v_ParticlesColor_data[6] = 0.f;
  v_ParticlesColor_data[7] = 1.f;

  v_ParticlesColor_data[8] = 1.f;
  v_ParticlesColor_data[9] = 0.f;
  v_ParticlesColor_data[10] = 0.f;
  v_ParticlesColor_data[11] = 1.f;

  v_ParticlesColor_data[12] = 1.f;
  v_ParticlesColor_data[13] = 0.f;
  v_ParticlesColor_data[14] = 0.f;
  v_ParticlesColor_data[15] = 1.f;*/

  // Now, update VBOs
  // ...
};
