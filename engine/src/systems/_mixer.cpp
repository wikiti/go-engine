#include "systems/_mixer.h"
#include "systems/_debug.h"
#include "systems/_resource.h"
#include "systems/_render.h"
#include "systems/_manager.h"

CSystem_Mixer gSystem_Mixer;
CSystem_Mixer& gMixer = gSystem_Mixer;

const uint CSystem_Mixer::NUMBER_SOURCES = 31;

bool CSystem_Mixer::Init()
{
  CSystem::Init();

  /* Init SDL_Mixer */
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_channels = 1;
  int audio_buffers = 4096;

  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
  {
    gSystem_Debug.error("Error from Mixer: Could not load SDL_Mixer: %s.", Mix_GetError());
    return false;
  }

  ALCdevice *device;
  ALCcontext *ctx;

  /* Open and initialize a device with default settings */
  device = alcOpenDevice(NULL);
  if(!device)
  {
    gSystem_Debug.error("Error from Mixer: Could not load a sound device.");
    return false;
  }

  ctx = alcCreateContext(device, NULL);
  if(ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
  {
    if(ctx != NULL)
      alcDestroyContext(ctx);
    alcCloseDevice(device);

    gSystem_Debug.error("Error from Mixer: Could not set an OpenAL context!");

    return false;
  }

  ALenum error;
  ALuint source[NUMBER_SOURCES];
  // Generate the sources
  alGenSources(NUMBER_SOURCES, source);
  if ((error = alGetError()) != AL_NO_ERROR)
  {
    gSystem_Debug.error("Error from Mixer: Could not create OpenAL sources!: %d", error);
    return false;
  }

  source_list.resize(NUMBER_SOURCES);
  for(uint i = 0; i < NUMBER_SOURCES; i++)
    source_list[i] = source[i];

  listener = "";

  return true;
}

void CSystem_Mixer::Close()
{
  Mix_CloseAudio();

  // Borrar sources
  for(vector<ALuint>::iterator it = source_list.begin(); it != source_list.end(); it++)
    alDeleteSources(1, &(*it));

  ALCdevice *device;
  ALCcontext *ctx;

  ctx = alcGetCurrentContext();
  if(ctx == NULL)
      return;

  device = alcGetContextsDevice(ctx);

  alcMakeContextCurrent(NULL);
  alcDestroyContext(ctx);
  alcCloseDevice(device);
}

bool CSystem_Mixer::PlaySound(string name, CGameObject* source)
{
  CResource_Sound* sound   = gSystem_Resources.GetSound(name);
  CGameObject* go_listener = gSystem_GameObject_Manager[listener];

  if(!sound || !source) return false;

  if(!go_listener) go_listener = gSystem_Render.GetMainCamera();
  if(!go_listener) return false;

  // Source propertiers
  vector3f vel, pos, euler;
  pos = source->Transform()->Position();
  euler = source->Transform()->EulerAngles();

  alSourcefv (sound->source_attached, AL_POSITION,  ((float*) &pos)   );
  alSourcefv (sound->source_attached, AL_VELOCITY,  ((float*) &vel)   );
  alSourcefv (sound->source_attached, AL_DIRECTION, ((float*) &euler) ); // <- no funcional :(

  // Listener propertiers
  pos = go_listener->Transform()->Position();
  euler = go_listener->Transform()->EulerAngles();
  alListenerfv(AL_POSITION,    ((float*) &pos)   );
  alListenerfv(AL_VELOCITY,    ((float*) &vel)   );
  alListenerfv(AL_ORIENTATION, ((float*) &euler) );

  ALenum error;
  ALuint buffer;

  alSourceStop(sound->source_attached);
  if ((error = alGetError()) != AL_NO_ERROR)
  {
    gSystem_Debug.console_error_msg("Error from Mixer: alSourceStop error for \"%s\": %d\n", name.c_str(), error);
    return false;
  }

  alGetSourcei(sound->source_attached, AL_BUFFER, (ALint*)&buffer);
  if ((error = alGetError()) != AL_NO_ERROR)
  {
    gSystem_Debug.console_error_msg("Error from Mixer: alGetSourcei error for \"%s\": %d\n", name.c_str(), error);
    return false;
  }

  //alSourceUnqueueBuffers(sound->source_attached, 1, &buffer);
  if ((error = alGetError()) != AL_NO_ERROR)
  {
    gSystem_Debug.console_error_msg("Error from Mixer: alSourceUnqueueBuffers error for \"%s\": %d\n", name.c_str(), error);
    return false;
  }

  alSourceQueueBuffers(sound->source_attached, 1, &sound->buffer_id);
  if ((error = alGetError()) != AL_NO_ERROR)
  {
    gSystem_Debug.console_error_msg("Error from Mixer: alSourceQueueBuffers error for \"%s\": %d\n", name.c_str(), error);
    return false;
  }

  alSourcePlay(sound->source_attached);
  if ((error = alGetError()) != AL_NO_ERROR)
  {
    gSystem_Debug.console_error_msg("Error from Mixer: alSourcePlay error for \"%s\": %d\n", name.c_str(), error);
    return false;
  }

  return true;
}

bool CSystem_Mixer::PauseSound(string name)
{

  return true;
}

bool CSystem_Mixer::RewindSound(string name)
{

  return true;
}

