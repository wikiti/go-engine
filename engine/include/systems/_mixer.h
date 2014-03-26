#ifndef __CSYSTEM_MIXER_H_
#define __CSYSTEM_MIXER_H_

#include "_globals.h"
#include "_system.h"
#include "_object.h"

// Más sencillo:
// Audio Source y Audio Listener como componentes, no como audio.
class CSystem_Mixer: public CSystem
{
  friend class CComponent_Audio_Source;
  friend class CSystem_Debug;

  private:
    // Buffers reservados "one shots"...
    std::vector< ALuint > oneshot_used;
    std::vector< ALuint > oneshot_unused;

    // Buffers normales
    std::vector< ALuint > sources_used;
    std::vector< ALuint > sources_unused;

    /*static const */uint NUMBER_SOURCES;
    /*static const */uint NUMBER_SOURCES_ONESHOT;

    ALuint GetFreeSource();
    void AddFreeSource(ALuint source);

    ALuint GetFreeOneShot();

  public:
    CGameObject* listener; // Si es inválido o no reconocido, se usará la cámara principal del Render como listener.

  public:
    CSystem_Mixer(): CSystem() {};

    bool Init();
    void Close();
    bool Reset();

    void ResetSources();

    void OnLoop();
};

extern CSystem_Mixer gSystem_Mixer;
extern CSystem_Mixer& gMixer;

#endif /* __CSYSTEM_MIXER_H_ */
