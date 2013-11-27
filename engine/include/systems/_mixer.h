#ifndef __CSYSTEM_MIXER_H_
#define __CSYSTEM_MIXER_H_

#include "_globals.h"
#include "_system.h"

class CSystem_Mixer: public CSystem
{
  private:
    map<string, ALuint> source_list;

  public:
    CSystem_Mixer() {};

    bool Init();
    void Close();

    bool LoadFile(string name, string file);

    bool AddEmpty(string name);
    bool Remove(string name);

    ALuint GetSourceID();

    void PlaySound(string name);
    void PlaySound(ALuint id);

    void PauseSound(string name);
    void PauseSound(ALuint id);

    void RewindSound(string name);
    void RewindSound(ALuint id);
};

extern CSystem_Mixer gSystem_Mixer;
extern CSystem_Mixer& gMixer;

#endif /* __CSYSTEM_MIXER_H_ */
