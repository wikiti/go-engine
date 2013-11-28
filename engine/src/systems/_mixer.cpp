#include "systems/_mixer.h"
#include "systems/_debug.h"

CSystem_Mixer gSystem_Mixer;
CSystem_Mixer& gMixer = gSystem_Mixer;

const uint CSystem_Mixer::NUMBER_SOURCES = 31;

bool CSystem_Mixer::Init()
{
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


