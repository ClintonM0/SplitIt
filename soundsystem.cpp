#include <soundsystem.h>
#include <fmod.hpp>
#include <fmod_errors.h>

class soundSystem
{
    FMOD::System *fmodSystem;
    typedef FMOD::Sound* sound;
    soundSystem()
    {
        if(FMOD::System_Create(&fmodSystem) != FMOD_OK)
        {
            return;
        }

        int driverCount = 0;
        fmodSystem->getNumDrivers(&driverCount);

        if(driverCount == 0)
        {
            return;
        }

        fmodSystem->init(24, FMOD_INIT_NORMAL, 0);

    }

    void createSound(sound *audioOutput, const char* audioFile)
    {
        fmodSystem->createSound(audioFile, FMOD_CREATESTREAM, 0, audioOutput);
    }

    void playSound(sound *audioOutput, bool bLoop = false)
    {

    }

};
