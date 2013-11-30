#include "components/_component_audio.h"

#include "systems/_other.h"
#include "systems/_debug.h"
#include "systems/_render.h"
#include "systems/_mixer.h"
#include "systems/_data.h"

#include "_object.h"

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
  if(mute && playing)
    alSourceStop(source_attached);

  if(playing && !paused)
    Setup();
}

void CComponent_Audio_Source::OnRender()
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

  alSourcef(source_attached, AL_GAIN, volume*gVolume);

  alSourcef(source_attached, AL_MAX_DISTANCE, max_distance);
  alSourcef(source_attached, AL_REFERENCE_DISTANCE, min_distance);

  alSourcei(source_attached, AL_LOOPING, loop);
}

void CComponent_Audio_Source::Play()
{
  if(!enabled || mute) return;

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


}

void CComponent_Audio_Source::PlayAt(vector3f pos)
{
  PlayAt(pos.x, pos.y, pos.z);

}

void CComponent_Audio_Source::PlayAt(ALfloat x, ALfloat y, ALfloat z)
{
  if(!sound || !enabled || mute) return;

  // usar PlayOneShot() para esto, evitando que OnLoop() cambie los valores de volumne, pitch o posición
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
    source_attached = 0;
    alSourcei(source_attached, AL_BUFFER, 0);
  }
}
