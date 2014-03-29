#include "components/_component_audio.h"

#include "systems/_other.h"
#include "systems/_debug.h"
#include "systems/_render.h"
#include "systems/_mixer.h"
#include "systems/_data.h"
#include "systems/_shader.h"

#include "_object.h"

using namespace std;

CComponent_Audio_Source::CComponent_Audio_Source(CGameObject* gameObject): CComponent(gameObject)
{
  playing = paused = false;
  // Solicitar source al Mixer
  //source_attached = 0;
  source_attached = 0; //gSystem_Mixer.GetFreeSource();

  sound = NULL;
  pitch = volume = 1.f;

  mute = loop = start_playing = music = false;
  affected_by_time = true;

  everywhere = false;

  max_distance = 20.f;
  min_distance = 5.f;
}

CComponent_Audio_Source::~CComponent_Audio_Source()
{
  UnBind();
}

void CComponent_Audio_Source::OnLoop()
{
  if(!sound || !enabled || !source_attached) return;
  //if(mute && playing)
    //alSourceStop(source_attached);

  if(playing && !paused)
    Setup();
}

void CComponent_Audio_Source::OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix)
{
  if(everywhere) return;

  float sub_x = gSystem_Data_Storage.GetFloat("__RENDER_SOUND_RADIUS_X"), sub_z = gSystem_Data_Storage.GetFloat("__RENDER_SOUND_RADIUS_Z");

  gSystem_Render.RenderSphere(max_distance, sub_x, sub_z);
  gSystem_Render.RenderSphere(min_distance, sub_x, sub_z);
}


void CComponent_Audio_Source::Setup()
{
  vector3f vel, pos, euler;
  if(!everywhere)
  {
    pos = gameObject->Transform()->Position();
    euler = gameObject->Transform()->EulerAngles();
  }
  else
  {
    alGetListenerfv(AL_POSITION,    ((float*) &pos)   );
    //alGetListenerfv(AL_VELOCITY,    ((float*) &vel)   );
    //alGetListenerfv(AL_ORIENTATION, ((float*) &euler) );
  }

  alSourcefv(source_attached, AL_POSITION,  ((float*) &pos)   );
  alSourcefv(source_attached, AL_VELOCITY,  ((float*) &vel)   );
  alSourcefv(source_attached, AL_DIRECTION, ((float*) &euler) );

  if(affected_by_time) alSourcef(source_attached, AL_PITCH, pitch*gSystem_Time.timeScale());
  else                 alSourcef(source_attached, AL_PITCH, pitch);

  float gVolume = 1.f;
  if(music)
    gVolume = gSystem_Data_Storage.GetFloat("__SOUND_MUSIC_VOLUME");
  else
    gVolume = gSystem_Data_Storage.GetFloat("__SOUND_VOLUME");

  alSourcef(source_attached, AL_GAIN, (mute)? 0.f : volume*gVolume);

  alSourcef(source_attached, AL_MAX_DISTANCE, max_distance);
  alSourcef(source_attached, AL_REFERENCE_DISTANCE, min_distance);

  alSourcei(source_attached, AL_LOOPING, loop);
}

void CComponent_Audio_Source::Play()
{
  if(!enabled/* || mute*/) return;

  if(!sound)
  {
    gSystem_Debug.console_error_msg("Playing an empty audio source from \"%s\"", gameObject->GetName().c_str());
    return;
  }

  if(!source_attached)
  {
    gSystem_Debug.console_error_msg("Playing an unbinded audio source from \"%s\"", gameObject->GetName().c_str());
    return;
  }

  if(playing && !paused)
    alSourceStop(source_attached);

  Setup();

  alSourcePlay(source_attached);
  ALenum error;
  if ((error = alGetError()) != AL_NO_ERROR)
    gSystem_Debug.console_error_msg("Error %d trying to play sound \"%s\" from \"%s\"", error, sound->File().c_str(), gameObject->GetName().c_str());

  playing = true;
}

void CComponent_Audio_Source::PlayOneShot()
{
  if(!sound || !enabled || mute) return;

  ALuint source_oneshot = gSystem_Mixer.GetFreeOneShot();
  if(!source_oneshot) return;

  ALuint buffer = sound->BufferID();
  alSourceQueueBuffers(source_oneshot, 1, &buffer);

  vector3f vel, pos, euler;
  if(!everywhere)
  {
    pos = gameObject->Transform()->Position();
    euler = gameObject->Transform()->EulerAngles();
  }
  else
  {
    alGetListenerfv(AL_POSITION,    ((float*) &pos)   );
    //alGetListenerfv(AL_VELOCITY,    ((float*) &vel)   );
    //alGetListenerfv(AL_ORIENTATION, ((float*) &euler) );
  }

  alSourcefv(source_oneshot, AL_POSITION, pos.to_a());
  alSourcefv(source_oneshot, AL_VELOCITY, vel.to_a());
  alSourcefv(source_oneshot, AL_DIRECTION, euler.to_a());

  if(affected_by_time) alSourcef(source_oneshot, AL_PITCH, pitch*gSystem_Time.timeScale());
  else                 alSourcef(source_oneshot, AL_PITCH, pitch);

  float gVolume = 1.f;
  if(music)
    gVolume = gSystem_Data_Storage.GetFloat("__SOUND_MUSIC_VOLUME");
  else
    gVolume = gSystem_Data_Storage.GetFloat("__SOUND_VOLUME");

  alSourcef(source_oneshot, AL_GAIN, volume*gVolume);

  alSourcef(source_oneshot, AL_MAX_DISTANCE, max_distance);
  alSourcef(source_oneshot, AL_REFERENCE_DISTANCE, min_distance);

  //alSourcei(source_oneshot, AL_LOOPING, loop);
  alSourcePlay(source_oneshot);
}

void CComponent_Audio_Source::PlayOneShotAt(vector3f pos)
{
  if(!sound || !enabled || mute) return;

  ALuint source_oneshot = gSystem_Mixer.GetFreeOneShot();
  if(!source_oneshot)
    return;

  ALuint buffer = sound->BufferID();
  alSourceQueueBuffers(source_oneshot, 1, &buffer);

  // Vectores vacíos, de momento
  vector3f vel, euler;

  alSourcefv(source_oneshot, AL_POSITION, pos.to_a());
  alSourcefv(source_oneshot, AL_VELOCITY, vel.to_a());
  alSourcefv(source_oneshot, AL_DIRECTION, euler.to_a());

  if(affected_by_time) alSourcef(source_oneshot, AL_PITCH, pitch * gSystem_Time.timeScale());
  else                 alSourcef(source_oneshot, AL_PITCH, pitch);

  float gVolume = 1.f;
  if(music)
    gVolume = gSystem_Data_Storage.GetFloat("__SOUND_MUSIC_VOLUME");
  else
    gVolume = gSystem_Data_Storage.GetFloat("__SOUND_VOLUME");

  alSourcef(source_oneshot, AL_GAIN, volume * gVolume);

  alSourcef(source_oneshot, AL_MAX_DISTANCE, max_distance);
  alSourcef(source_oneshot, AL_REFERENCE_DISTANCE, min_distance);

  //alSourcei(source_oneshot, AL_LOOPING, loop);
  alSourcePlay(source_oneshot);

  // usar PlayOneShot() para esto, evitando que OnLoop() cambie los valores de volumne, pitch o posición
}

void CComponent_Audio_Source::PlayOneShotAt(ALfloat x, ALfloat y, ALfloat z)
{
  PlayOneShotAt(vector3f(x, y, z));
}

void CComponent_Audio_Source::Stop()
{
  if(!sound || !enabled || !source_attached) return;

  if(playing)
  {
    alSourceStop(source_attached);
    playing = paused = false;
  }

  ALenum error;
  if ((error = alGetError()) != AL_NO_ERROR)
    gSystem_Debug.console_error_msg("Error %d trying to stop sound \"%s\" from \"%s\"", error, sound->File().c_str(), gameObject->GetName().c_str());
}

void CComponent_Audio_Source::Pause()
{
  if(!sound || !enabled || !source_attached) return;

  if(playing)
  {
    alSourcePause(source_attached);
    playing = false;
    paused = true;
  }

  ALenum error;
  if ((error = alGetError()) != AL_NO_ERROR)
    gSystem_Debug.console_error_msg("Error %d trying to pause sound \"%s\" from \"%s\"", error, sound->File().c_str(), gameObject->GetName().c_str());
}

void CComponent_Audio_Source::Resume()
{
  if(!sound || !enabled || !source_attached) return;

  //if(paused)
  //{
    alSourcePlay(source_attached);
    playing = true;
    paused = false;
  //}

  ALenum error;
  if ((error = alGetError()) != AL_NO_ERROR)
    gSystem_Debug.console_error_msg("Error %d trying to pause sound \"%s\" from \"%s\"", error, sound->File().c_str(), gameObject->GetName().c_str());
}

void CComponent_Audio_Source::Rewind()
{
  if(!sound || !enabled || !source_attached) return;

  alSourceRewind(source_attached);
  ALenum error;
  if ((error = alGetError()) != AL_NO_ERROR)
    gSystem_Debug.console_error_msg("Error %d trying to rewind sound \"%s\" from \"%s\"", error, sound->File().c_str(), gameObject->GetName().c_str());
}

void CComponent_Audio_Source::Bind()
{
  if(!source_attached && sound)
  {
    source_attached = gSystem_Mixer.GetFreeSource();
    alSourcei(source_attached, AL_BUFFER, sound->BufferID());

    if(start_playing) Play();
  }
}

void CComponent_Audio_Source::UnBind()
{
  if(source_attached)
  {
    gSystem_Mixer.AddFreeSource(source_attached);

    alSourceStop(source_attached);
    alSourcei(source_attached, AL_BUFFER, 0);

    source_attached = 0;
  }
}

void CComponent_Audio_Source::parseDebug(string command)
{
  stringstream ss(command);
  string attrib;
  ss >> attrib;

  if(attrib == "help" or attrib == "?" or attrib == "")
  {
    printDebug();

    return;
  }

  if(attrib == "mute" or attrib == "music" or attrib == "loop" or attrib == "start_playing"  or attrib == "affected_by_time"  or attrib == "everywhere")
  {
    bool data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }

    if(attrib == "mute")
      mute = data;
    else if(attrib == "music")
      music = data;
    else if(attrib == "loop")
      loop = data;
    else if(attrib == "start_playing")
      start_playing = data;
    else if(attrib == "affected_by_time")
      affected_by_time = data;
    else if(attrib == "everywhere")
      everywhere = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%d\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), (int)data );
  }
  else if(attrib == "pitch" or attrib == "volume" or attrib == "max_distance" or attrib == "min_distance")
  {
    float data;
    ss >> data;

    if(ss.fail())
    {
      gSystem_Debug.console_error_msg("From component %s - %s: Invalid format. Data format is: \"<atribute> <attriube type value>\"", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()) );
      return;
    }

    if(attrib == "pitch")
      pitch = data;
    else if(attrib == "volume")
      volume = data;
    else if(attrib == "max_distance")
      max_distance = data;
    else if(attrib == "min_distance")
      min_distance = data;

    gSystem_Debug.console_msg("From component %s - %s: Set variable \"%s\" to value \"%f\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str(), data );
  }
  else
  {
    gSystem_Debug.console_error_msg("From component %s - %s: Unknow attribute \"%s\".", gameObject->GetName().c_str(), Components::component_to_string( (Components::components_t)GetID()), attrib.c_str() );
  }

}

void CComponent_Audio_Source::printDebug()
{
  gSystem_Debug.console_warning_msg("Component %s uses the following attributes:", Components::component_to_string( (Components::components_t)GetID()));
  gSystem_Debug.console_warning_msg("Attribute            Type          Value");
  gSystem_Debug.console_warning_msg("-----------------------------------------");
  gSystem_Debug.console_warning_msg("mute                 bool          %d", (int)mute);
  gSystem_Debug.console_warning_msg("music                bool          %d", (int)music);
  gSystem_Debug.console_warning_msg("loop                 bool          %d", (int)loop);
  gSystem_Debug.console_warning_msg("start_playing        bool          %d", (int)start_playing);
  gSystem_Debug.console_warning_msg("affected_by_time     bool          %d", (int)affected_by_time);
  gSystem_Debug.console_warning_msg("everywhere           bool          %d", (int)everywhere);
  gSystem_Debug.console_warning_msg("pitch                float         %f", pitch);
  gSystem_Debug.console_warning_msg("volume               float         %f", volume);
  gSystem_Debug.console_warning_msg("max_distance         float         %f", max_distance);
  gSystem_Debug.console_warning_msg("min_distance         float         %f", min_distance);
}
