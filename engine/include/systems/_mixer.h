#ifndef __CSYSTEM_MIXER_H_
#define __CSYSTEM_MIXER_H_

#include "_globals.h"
#include "_system.h"
#include "_object.h"

class CSystem_Mixer: public CSystem
{
  private:
    vector<ALuint> source_list;
    // sería conveniente usar un vector de pilas para los conflictos...

  public:
    string listener; // Si es inválido o no reconocido, se usará la cámara principal del Render como listener.

  public:
    CSystem_Mixer() {};

    bool Init();
    void Close();

    bool RemoveBuffer(ALuint id);

    // ¿Listener?
    bool PlaySound(string name, CGameObject* source = NULL);
    bool PauseSound(string name);
    bool RewindSound(string name);

    ALuint GetSourceID(uint index)
    {
      return source_list[index-1];
    }

    void OnLoop();

    static const uint NUMBER_SOURCES;
};

extern CSystem_Mixer gSystem_Mixer;
extern CSystem_Mixer& gMixer;

#endif /* __CSYSTEM_MIXER_H_ */
