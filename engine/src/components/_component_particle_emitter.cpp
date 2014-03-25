#include "systems/_render.h"
#include "systems/_other.h"
#include "systems/_resource.h"
#include "systems/_shader.h"
#include "components/_component_particle_emitter.h"

//BOOST_CLASS_EXPORT_IMPLEMENT(CComponent_Particle_Emitter);
GLuint CComponent_Particle_Emitter::m_ParticlesVAO = 0;
GLuint CComponent_Particle_Emitter::m_ParticlesVBOVertices = 0;

GLuint CComponent_Particle_Emitter::m_ParticlesVBOPosition = 0;
GLuint CComponent_Particle_Emitter::m_ParticlesVBOAngleScale = 0;
GLuint CComponent_Particle_Emitter::m_ParticlesVBOColor = 0;

bool CComponent_Particle_Emitter::InitRenderVBO()
{
  glGenVertexArrays(1, &m_ParticlesVAO);
  if(!m_ParticlesVAO)
  {
    gSystem_Debug.error(
        "From CComponent_Particle_Emitter: Could not generate Particle Emitter VAO.");
  }

  glBindVertexArray(m_ParticlesVAO);

  glGenBuffers(1, &m_ParticlesVBOVertices);

  glGenBuffers(1, &m_ParticlesVBOPosition);
  glGenBuffers(1, &m_ParticlesVBOAngleScale);
  glGenBuffers(1, &m_ParticlesVBOColor);

  if(!m_ParticlesVBOVertices or !m_ParticlesVBOPosition or !m_ParticlesVBOAngleScale or !m_ParticlesVBOColor)
  {
    gSystem_Debug.error("From CComponent_Particle_Emitter: Could not generate Particle Emitter VBO.");

    glDeleteBuffers(1, &m_ParticlesVBOVertices);

    glDeleteBuffers(1, &m_ParticlesVBOPosition);
    glDeleteBuffers(1, &m_ParticlesVBOAngleScale);
    glDeleteBuffers(1, &m_ParticlesVBOColor);

    glDeleteVertexArrays(1, &m_ParticlesVAO);

    return false;
  }

  const GLfloat p_ParticleVertices[][3] =
  {
      {0.25f, 0.25f, 0.f}, {-0.25f, 0.25f, 0.f},
      {0.25f,-0.25f, 0.f}, {-0.25f, -0.25f, 0.f}
  };

  // This will remain static, so we use GL_STATIC_DRAW
  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOVertices);
  glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), p_ParticleVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // los 2 VBOs de arriban se pueden poner como constantes dentro del shader, ya que permanecen intactos

  // Other, not inited yet
  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOPosition);
  //glBufferData(GL_ARRAY_BUFFER, particles.size() * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOAngleScale);
  //glBufferData(GL_ARRAY_BUFFER, particles.size() * 2 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOColor);
  //glBufferData(GL_ARRAY_BUFFER, particles.size() * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, 0, 0);

  glVertexAttribDivisor(0, 0);
  glVertexAttribDivisor(1, 1);
  glVertexAttribDivisor(2, 1);
  glVertexAttribDivisor(3, 1);

  glBindVertexArray(0);

  return true;
}

void CComponent_Particle_Emitter::CloseRenderVBO()
{
  glDeleteBuffers(1, &m_ParticlesVBOVertices);

  glDeleteBuffers(1, &m_ParticlesVBOPosition);
  glDeleteBuffers(1, &m_ParticlesVBOAngleScale);
  glDeleteBuffers(1, &m_ParticlesVBOColor);

  glDeleteVertexArrays(1, &m_ParticlesVAO);
}


CComponent_Particle_Emitter::CParticle::CParticle()
{
  active = false;
  life = -1.f;

  position = velocity = acceleration = vector3f_t();
  angle = angle_velocity = angle_acceleration = scale = scale_factor = 0.f;
}

CComponent_Particle_Emitter::CParticle::~CParticle()
{

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

  //for(vector<CParticle>::iterator it = particles.begin(); it != particles.end(); ++it)
    //(*it) = new CParticle;
  new_particles = 0;
}

CComponent_Particle_Emitter::~CComponent_Particle_Emitter()
{
  /*for(vector<CParticle>::iterator it = particles.begin(); it != particles.end(); ++it)
  {
    delete (*it);
    // ->xBUG Tiempos demasiado largos a la hora de borrar partículas. Normal, son demasiadas, y parece que "delete" es lento.
    // Parece arreglado, ya que no da problemas. Ahora no se usan punteros, sino que se utilizan objetos. Tal vez, en un futuro, se usen punteros por simplicidad.
  }*/

  particles.clear();
  v_ParticlePosition_data.clear();
  v_ParticlesAngleScale_data.clear();
  v_ParticlesColor_data.clear();

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
    //for(vector<CParticle>::iterator it = particles.begin(); it != particles.end(); ++it)
      //delete (*it);

    particles.clear();
  }

  particles.resize(max_particles);

  if(particles_per_second == 0)
    new_particles = max_particles;
  else
  {
    if(gSystem_Time.deltaTime())
      new_particles = particles_per_second * gSystem_Time.deltaTime_s();
    else
      new_particles = particles_per_second * gSystem_Time.GetTicks_s();
  }

  for(vector<CParticle>::iterator it = particles.begin(); it != particles.end(); ++it)
  {
    //(*it) = new CParticle;
    //(*it) = CParticle();

    if((int)new_particles and (it - particles.begin()) < new_particles)  // Even if new_particles is bigger that max_particles, there will be up to max_particles updates (particles.size())
      NewParticle(*it, pos_difference);
  }

  v_ParticlePosition_data.clear();
  v_ParticlesAngleScale_data.clear();
  v_ParticlesColor_data.clear();

  v_ParticlePosition_data.resize(max_particles*3);
  v_ParticlesAngleScale_data.resize(max_particles*2);
  v_ParticlesColor_data.resize(max_particles*4);
}

void CComponent_Particle_Emitter::UpdateVBO()
{
  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOPosition);
  glBufferData(GL_ARRAY_BUFFER, particles.size() * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * 3 * sizeof(GLfloat), &v_ParticlePosition_data[0]);
  //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOAngleScale);
  glBufferData(GL_ARRAY_BUFFER, particles.size() * 2 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * 2 * sizeof(GLfloat), &v_ParticlesAngleScale_data[0]);
  //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOColor);
  glBufferData(GL_ARRAY_BUFFER, particles.size() * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * 4 * sizeof(GLfloat), &v_ParticlesColor_data[0]);
  //glVertexAttribPointer(3, 4, GL_FLOAT, GL_TRUE, 0, 0);

  //glBindVertexArray(0);
}

void CComponent_Particle_Emitter::NewParticle(CParticle& p, vector3f pos_difference)
{
  p.active = true;
  p.life = gMath.random(start_min_life_time, start_max_life_time);
  //p.color = color;
  p.color.r = gMath.random(start_min_color.r, start_max_color.r);
  p.color.g = gMath.random(start_min_color.g, start_max_color.g);
  p.color.b = gMath.random(start_min_color.b, start_max_color.b);
  p.color.a = gMath.random(start_min_color.a, start_max_color.a);

  vector3f random_vector = gMath.random_vector(direction.normalize(), angle_spread/2);                // Dirección
  vector3f random_vector_XZ = vector3f(random_vector.x, 0, random_vector.z).normalize();  // Separación del origen
  // ->POR-HACER Hay que hacer que el área de generación aleatoria de partículas sea perpendicular al vector de dirección.

  p.position = random_vector * gMath.random(start_min_distance, start_max_distance) + pos_difference;
  p.position += random_vector_XZ * gMath.random(start_max_base_radius, start_min_base_radius);

  p.velocity = random_vector * gMath.random(start_min_vel, start_max_vel);
  p.acceleration = gravity;

  p.angle = gMath.random(start_min_angle, start_max_angle);
  p.angle_velocity = gMath.random(start_min_angle_vel, start_max_angle_vel);
  //angle_aceleration;

  gMath.NormalizeAngle(p.angle);

  p.scale = gMath.random(start_min_scale, start_max_scale);
  p.scale_factor = gMath.random(start_min_scale_factor, start_max_scale_factor);

  //p->material_name = material_name;
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

// ->NOTA En CComponent_Particle_Emitter::OnRender(), se producen algunos bajones de fps cuando el número de particulas supera una cierta cantidad (50.000). No debería ser muy problemático para casos sencillos.
void CComponent_Particle_Emitter::OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix)
{
  if(!enabled) return;

  glDepthMask(GL_FALSE);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glBindVertexArray(m_ParticlesVAO);
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

  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOVertices);
  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOPosition);
  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOAngleScale);
  glBindBuffer(GL_ARRAY_BUFFER, m_ParticlesVBOColor);

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles.size());

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);

  glBindVertexArray(0);

  glDepthMask(GL_TRUE);
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
  if(particles_per_second == 0)
    new_particles = max_particles;
  else
  {
    float new_particles_iteration = particles_per_second * gSystem_Time.deltaTime_s();
    new_particles += new_particles_iteration;
  }

  int added_particles = 0;
  for(vector<CParticle>::iterator it = particles.begin(); it != particles.end(); ++it)
  {
    if((*it).life >= 0 and (*it).active)
    {
        // Update values
      (*it).position += (*it).velocity * gTime.deltaTime_s() + pos_difference;
      (*it).velocity += (*it).acceleration * gTime.deltaTime_s();
      (*it).color += (color_adder * gTime.deltaTime_s());
      gMath.NormalizeColor((*it).color);

      (*it).angle += (*it).angle_velocity * gTime.deltaTime_s();
      gMath.NormalizeAngle((*it).angle);

      (*it).scale += (*it).scale_factor * gTime.deltaTime_s();
      //(*it).scale.y += (*it).scale_factor * gTime.deltaTime_s();
      //(*it).scale.z += (*it).scale_factor * gTime.deltaTime_s(); // <-- useless

      (*it).life -= gTime.deltaTime_s();
      //if((*it).life < 0 ) (*it).life = 0.f;

        // Control values
      // Speed
      // Angle
      // Scale
      if((*it).scale < min_scale) (*it).scale = min_scale;
      if((*it).scale > max_scale) (*it).scale = max_scale;
      // Position
      // etc
      // Color
      if((*it).color.r < min_color.r) (*it).color.r = min_color.r;
      if((*it).color.g < min_color.g) (*it).color.g = min_color.g;
      if((*it).color.b < min_color.b) (*it).color.b = min_color.b;
      if((*it).color.a < min_color.a) (*it).color.a = min_color.a;

      if((*it).color.r > max_color.r) (*it).color.r = max_color.r;
      if((*it).color.g > max_color.g) (*it).color.g = max_color.g;
      if((*it).color.b > max_color.b) (*it).color.b = max_color.b;
      if((*it).color.a > max_color.a) (*it).color.a = max_color.a;


      // Update VBO aux variables
      int index = it - particles.begin();

      // Position
      v_ParticlePosition_data[3*index + 0] = (*it).position.x;
      v_ParticlePosition_data[3*index + 1] = (*it).position.y;
      v_ParticlePosition_data[3*index + 2] = (*it).position.z;

      // Angle and scale
      v_ParticlesAngleScale_data[2*index + 0] = (*it).angle;
      v_ParticlesAngleScale_data[2*index + 1] = (*it).scale;

      // Color
      float alpha = (*it).color.a;
      if((*it).life < 1.f)
        alpha *= (*it).life;

      v_ParticlesColor_data[4*index + 0] = (*it).color.r;
      v_ParticlesColor_data[4*index + 1] = (*it).color.g;
      v_ParticlesColor_data[4*index + 2] = (*it).color.b;
      v_ParticlesColor_data[4*index + 3] = alpha;
    }
    else if((*it).life < 0 and !stop and (int)new_particles and added_particles < (int)new_particles)
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

void CComponent_Particle_Emitter::parseDebug(string command)
{
  stringstream ss(command);
  string attrib;
  ss >> attrib;

  if(attrib == "help" or attrib == "?" or attrib == "")
  {
    printDebug();

    return;
  }

  if(attrib == "angle_spread" or attrib == "particles_per_second" or attrib == "max_vel" or attrib == "min_vel"
      or attrib == "max_angle_vel"  or attrib == "min_angle_vel" or attrib == "max_scale" or attrib == "min_scale"
      or attrib == "start_max_life_time" or attrib == "start_min_life_time" or attrib == "start_max_angle" or attrib == "start_min_angle"
      or attrib == "start_max_angle_vel" or attrib == "start_min_angle_vel" or attrib == "start_max_vel"
      or attrib == "start_min_vel" or attrib == "start_max_scale" or attrib == "start_min_scale" or attrib == "start_max_scale_factor"
      or attrib == "start_min_scale_factor" or attrib == "start_max_base_radius" or attrib == "start_min_base_radius")
  {
    GLfloat data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }

    if(attrib == "angle_spread")
      angle_spread = data;
    else if(attrib == "particles_per_second")
      particles_per_second = data;
    else if(attrib == "max_vel")
      max_vel = data;
    else if(attrib == "min_vel")
      min_vel = data;
    else if(attrib == "max_angle_vel")
      max_angle_vel = data;
    else if(attrib == "min_angle_vel")
      min_angle_vel = data;
    else if(attrib == "max_scale")
      max_scale = data;
    else if(attrib == "min_scale")
      min_scale = data;
    else if(attrib == "start_max_life_time")
      start_max_life_time = data;
    else if(attrib == "start_min_life_time")
      start_min_life_time = data;
    else if(attrib == "start_max_angle")
      start_max_angle = data;
    else if(attrib == "start_min_angle")
      start_min_angle = data;
    else if(attrib == "start_max_angle_vel")
      start_max_angle_vel = data;
    else if(attrib == "start_min_angle_vel")
      start_min_angle_vel = data;
    else if(attrib == "start_max_vel")
      start_max_vel = data;
    else if(attrib == "start_min_vel")
      start_min_vel = data;
    else if(attrib == "start_max_scale")
      start_max_scale = data;
    else if(attrib == "start_min_scale")
      start_min_scale = data;
    else if(attrib == "start_max_scale_factor")
      start_max_scale_factor = data;
    else if(attrib == "start_min_scale_factor")
      start_min_scale_factor = data;
    else if(attrib == "start_max_base_radius")
      start_max_base_radius = data;
    else if(attrib == "start_min_base_radius")
      start_min_base_radius = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%f\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), data );
  }
  else if(attrib == "stop" or attrib == "freeze")
  {
    bool data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }

    if(attrib == "stop")
      stop = data;
    else if(attrib == "freeze")
      freeze = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%d\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), (int)data );
  }
  else if(attrib == "direction" or attrib == "gravity")
  {
    vector3f data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }

    if(attrib == "direction")
      direction = data;
    else if(attrib == "gravity")
      gravity = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%s\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), data.str().c_str() );
  }
  else if(attrib == "max_particles")
  {
    uint data;
    ss >> data;
    if(ss.fail() or data < 0)
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }
  }
  else if(attrib == "material_name")
  {
    string data;
    ss >> data;

    material_name = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%s\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), data.c_str() );
  }
  /*
   *     gSystem_Debug.console_warning_msg("start_[max|min]_color          colorf_t      %s/%s", start_max_color.str().c_str(), start_min_color.str().c_str());
    gSystem_Debug.console_warning_msg("[max|min]_color                colorf_t      %s%s", max_color.str().c_str(), min_color.str().c_str());
    gSystem_Debug.console_warning_msg("color_adder                    colorf_t      %s", color_adder.str().c_str());
   */
  else if(attrib == "start_max_color" or attrib ==  "start_min_color" or attrib == "max_color" or attrib == "min_color" or attrib == "color_adder")
  {
    colorf_t data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }

    if(attrib == "start_max_color")
      start_max_color = data;
    else if(attrib == "start_min_color")
      start_min_color = data;
    else if(attrib == "max_color")
      max_color = data;
    else if(attrib == "min_color")
      min_color = data;
    else if(attrib == "color_adder")
      color_adder = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%s\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), data.str().c_str() );
  }
  else
  {
    gSystem_Debug.console_error_msg("From component %s - %s: Unknow attribute \"%s\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str() );
  }

}

void CComponent_Particle_Emitter::printDebug()
{
  gSystem_Debug.console_warning_msg("Component %s uses the following attributes:", Components::component_to_string( (Components::components_t)GetID()));
  gSystem_Debug.console_warning_msg("Attribute             Type                   Value");
  gSystem_Debug.console_warning_msg("--------------------------------------------------");
  gSystem_Debug.console_warning_msg("max_particles                  unsigned int  %d", max_particles);
  gSystem_Debug.console_warning_msg("material_name                  string        %s", material_name.c_str());
  gSystem_Debug.console_warning_msg("stop                           bool          %d", (int)stop);
  gSystem_Debug.console_warning_msg("freeze                         bool          %d", (int)freeze);
  gSystem_Debug.console_warning_msg("gravity                        vector3f      %s", gravity.str().c_str());
  gSystem_Debug.console_warning_msg("direction                      vector3f      %s", direction.str().c_str());
  gSystem_Debug.console_warning_msg("angle_spread                   float         %f", angle_spread);
  gSystem_Debug.console_warning_msg("particles_per_second           float         %f", particles_per_second);
  gSystem_Debug.console_warning_msg("[max|min]_vel                  float         %f / %f", max_vel, min_vel);
  gSystem_Debug.console_warning_msg("[max|min]_angle_vel            float         %f / %f", max_angle_vel, min_vel);
  gSystem_Debug.console_warning_msg("[max|min]_scale                float         %f / %f", max_scale, min_scale);
  gSystem_Debug.console_warning_msg("start_[max|min]_life_time      float         %f / %f", start_max_life_time, start_min_life_time);
  gSystem_Debug.console_warning_msg("start_[max|min]_angle          float         %f / %f", start_max_angle, start_min_angle);
  gSystem_Debug.console_warning_msg("start_[max|min]_angle_vel      float         %f / %f", start_max_angle_vel, start_min_angle_vel);
  gSystem_Debug.console_warning_msg("start_[max|min]_vel            float         %f / %f", start_max_vel, start_min_vel);
  gSystem_Debug.console_warning_msg("start_[max|min]_scale          float         %f / %f", start_max_scale, start_min_scale);
  gSystem_Debug.console_warning_msg("start_[max|min]_scale_factor   float         %f / %f", start_max_scale_factor, start_min_scale_factor);
  gSystem_Debug.console_warning_msg("start_[max|min]_base_radius    float         %f / %f", start_max_base_radius, start_min_base_radius);
  gSystem_Debug.console_warning_msg("start_[max|min]_color          colorf_t      %s / %s", start_max_color.str().c_str(), start_min_color.str().c_str());
  gSystem_Debug.console_warning_msg("[max|min]_color                colorf_t      %s / %s", max_color.str().c_str(), min_color.str().c_str());
  gSystem_Debug.console_warning_msg("color_adder                    colorf_t      %s", color_adder.str().c_str());

}
