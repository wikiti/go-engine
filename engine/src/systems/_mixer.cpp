#include "systems/_mixer.h"
#include "systems/_debug.h"
#include "systems/_resource.h"
#include "systems/_render.h"
#include "systems/_manager.h"

CSystem_Mixer gSystem_Mixer;
CSystem_Mixer& gMixer = gSystem_Mixer;

const uint CSystem_Mixer::NUMBER_SOURCES = 225;
const uint CSystem_Mixer::NUMBER_SOURCES_ONESHOT = 30;

bool CSystem_Mixer::Init()
{
  CSystem::Init();

  /* Init SDL_Mixer */
  int audio_rate = 44100;
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
  ALuint source_oneshot[NUMBER_SOURCES_ONESHOT];

  // Generate the sources
  alGenSources(NUMBER_SOURCES, source);
  if ((error = alGetError()) != AL_NO_ERROR)
  {
    gSystem_Debug.error("Error from Mixer: Could not create OpenAL sources!: %d", error);
    return false;
  }

  alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

  // Generate the sources
  alGenSources(NUMBER_SOURCES_ONESHOT, source_oneshot);
  if ((error = alGetError()) != AL_NO_ERROR)
  {
    gSystem_Debug.error("Error from Mixer: Could not create OpenAL sources!: %d", error);
    return false;
  }

  for(uint i = 0; i < NUMBER_SOURCES; i++)
    sources_unused.push_back(source[i]);
  for(uint i = 0; i < NUMBER_SOURCES_ONESHOT; i++)
    oneshot_unused.push_back(source_oneshot[i]);

  listener = NULL;

  return true;
}

void CSystem_Mixer::Close()
{
  Mix_CloseAudio();

  // Borrar sources
  for(vector<ALuint>::iterator it = sources_used.begin(); it != sources_used.end(); it++)
  {
    alSourceStop((*it));
    alDeleteSources(1, &(*it));
  }
  for(vector<ALuint>::iterator it = sources_unused.begin(); it != sources_unused.end(); it++)
    alDeleteSources(1, &(*it));

  for(vector<ALuint>::iterator it = oneshot_used.begin(); it != oneshot_used.end(); it++)
  {
    alSourceStop((*it));
    alDeleteSources(1, &(*it));
  }
  for(vector<ALuint>::iterator it = oneshot_unused.begin(); it != oneshot_unused.end(); it++)
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

ALuint CSystem_Mixer::GetFreeSource()
{
  if(!sources_unused.size())
  {
    gSystem_Debug.console_error_msg("Error from Mixer: No more sources availables. Unbind some of them! (MAX sources: %d)", NUMBER_SOURCES);
    return 0;
  }

  ALuint out = sources_unused.back();
  sources_unused.pop_back();

  sources_used.push_back(out);

  return out;
}

void CSystem_Mixer::AddFreeSource(ALuint source)
{
  sources_unused.push_back(source);

  for(vector<ALuint>::iterator it = sources_used.begin(); it != sources_used.end(); it++)
  {
    if(source == (*it))
    {
      sources_used.erase(it);
      break;
    }
  }
}

ALuint CSystem_Mixer::GetFreeOneShot()
{
  if(!oneshot_unused.size())
  {
    gSystem_Debug.console_error_msg("Error from Mixer: No more one-shots sources availables. (MAX one-shot sources: %d)", NUMBER_SOURCES_ONESHOT);
    return 0;
  }

  ALuint out = oneshot_unused.back();
  oneshot_unused.pop_back();

  oneshot_used.push_back(out);

  return out;
}

void CSystem_Mixer::OnLoop()
{
  if(!listener) listener = gSystem_Render.GetMainCamera();
  if(!listener) return;

  // Source propertiers
  vector3f vel, pos, euler;
  pos = listener->Transform()->Position();
  euler = listener->Transform()->EulerAngles();
  // FALTA VEL!!!
  alListenerfv(AL_POSITION,    ((float*) &pos)   );
  alListenerfv(AL_VELOCITY,    ((float*) &vel)   );
  //alListenerfv(AL_ORIENTATION, ((float*) &euler) );   // <- Fix this :C

  ALint processed;
  ALuint trash;
  GLenum error;

  for(vector<ALuint>::iterator it = oneshot_used.begin(); it != oneshot_used.end();)
  {
    // Comprobar si ha sido procesado y sacarlo de la cola.
    alGetSourcei((*it), AL_BUFFERS_PROCESSED, &processed);
    if ((error = alGetError()) != AL_NO_ERROR)
      gSystem_Debug.console_error_msg("Error %d from Mixer: Could not get buffers processed from source %d", error, (*it));

    if(processed != 0)
    {
      alSourceUnqueueBuffers((*it), 1, &trash);
      // lolwut con buffer
      //processed--;

      oneshot_unused.push_back((*it));
      oneshot_used.erase(it);
    }
    else
    {
      //alSourcef((*it), AL_PITCH, gSystem_Time.timeScale());

      //float gVolume = gSystem_Data_Storage.GetFloat("__SOUND_VOLUME");
      //alSourcef((*it), AL_GAIN, gVolume);
      it++;
    }
  }
}
