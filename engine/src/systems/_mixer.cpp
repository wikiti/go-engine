#include "systems/_mixer.h"
#include "systems/_debug.h"

CSystem_Mixer gSystem_Mixer;
CSystem_Mixer& gMixer = gSystem_Mixer;

bool CSystem_Mixer::Init()
{
  ALCdevice *device;
  ALCcontext *ctx;

  /* Open and initialize a device with default settings */
  device = alcOpenDevice(NULL);
  if(!device)
  {
    // ¿?
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

  ALenum error = alGetError();
  if (error != AL_NO_ERROR)
    gSystem_Debug.console_error_msg("Error from Mixer: %i", error);

  return true;
}

void CSystem_Mixer::Close()
{
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

