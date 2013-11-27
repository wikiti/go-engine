#ifndef __CSYSTEM_MIXER_H_
#define __CSYSTEM_MIXER_H_

#include "_globals.h"
#include "_system.h"

class CSystem_Mixer: public CSystem
{
  private:

  public:
    CSystem_Mixer() {};

    bool Init();
    void Close();
};

extern CSystem_Mixer gSystem_Mixer;
extern CSystem_Mixer& gMixer;

#endif /* __CSYSTEM_MIXER_H_ */
