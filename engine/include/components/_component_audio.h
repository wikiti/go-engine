#ifndef __COMPONENT_AUDIO_H_
#define __COMPONENT_AUDIO_H_

#include "_globals.h"
#include "components/_component.h"

#include "systems/_resource.h"


class CComponent_Audio_Source: public CComponent
{
  private:
    friend class CSystem_Mixer;
    friend class CGameObject;

    bool playing, paused;
    ALuint source_attached;

    CResource_Sound* sound;

    void Setup();

  public:
    ALfloat pitch;
    ALfloat volume;

    bool mute;
    bool music;
    bool loop;
    bool start_playing;
    bool affected_by_time;
    bool everywhere;

    ALfloat max_distance;
    ALfloat min_distance;
    /* That'll be alDistanceModel(AL_LINEAR_DISTANCE) or better alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED), which will turn distant sources off entirely.
    Read chapters 3.4.3 and 3.4.4 here. You also have to set the parameters in the formulas listed there (AL_MAX_DISTANCE and AL_REFERENCE_DISTANCE) for each source (alSourcef).
    If the parameters don't make sense, attenuation is turned off. If the sound is stereo, likewise (stereo is just that... stereo -- only mono sounds are 3D). */

  private:
    static int GetID() { return Components::audio_source; }

    void parseDebug(string command);
    void printDebug();

  public:
    CComponent_Audio_Source(){};
    CComponent_Audio_Source(CGameObject* gameObject);
    ~CComponent_Audio_Source();

    void SetSound(string name)
    {
      sound = gSystem_Resources.GetSound(name);
    }

    void OnLoop();
    void OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix);

    void Play();
    void PlayOneShot();
    void PlayAt(vector3f pos);
    void PlayAt(ALfloat x, ALfloat y, ALfloat z);

    void Stop();
    void Pause();
    void Rewind();

    void Bind();
    void UnBind();
};

#endif /* __COMPONENT_AUDIO_H_ */
