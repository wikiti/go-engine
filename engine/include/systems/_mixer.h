#ifndef __CSYSTEM_MIXER_H_
#define __CSYSTEM_MIXER_H_

#include "_globals.h"
#include "_system.h"

class CSystem_Mixer: public CSystem
{
  private:
    vector<ALuint> source_list;

  public:
    CSystem_Mixer() {};

    bool Init();
    void Close();

    bool RemoveBuffer(ALuint id);

    // ¿Listener?
    void PlaySound(ALuint id);
    void PauseSound(ALuint id);
    void RewindSound(ALuint id);

    ALuint GetSourceID(uint index)
    {
      return source_list[index-1];
    }

    static const uint NUMBER_SOURCES;
};

extern CSystem_Mixer gSystem_Mixer;
extern CSystem_Mixer& gMixer;

#endif /* __CSYSTEM_MIXER_H_ */
