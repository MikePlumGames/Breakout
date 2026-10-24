//#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <iostream>

// global handles for audio
ma_engine g_mainSoundEngine;

// music
ma_sound g_backgroundMusic;

// local group for playing sfx with a specific separate volume
ma_sound_group g_sfxGroup;
// same for music
ma_sound_group g_musicGroup;

float g_masterVolume{ 1.0f };
float g_sfxVolume{ 1.0f };
float g_musicVolume{ 1.0f };
// used for muting audio
float g_masterVolumeCurrent{ g_masterVolume };

// initialize the engine and music
bool audioInit()
{
    // initialize the sound engine, return false if failed
    if (ma_engine_init(NULL, &g_mainSoundEngine) != MA_SUCCESS)
    {
        return false;
    }
    // initialize the global sfx group for sfx's and return if it failed
    if (ma_sound_group_init(&g_mainSoundEngine, 0, NULL, &g_sfxGroup) != MA_SUCCESS)
    {
        ma_engine_uninit(&g_mainSoundEngine);
        return false;
    }
    // initialize the global music group for sfx's and return if it failed
    if (ma_sound_group_init(&g_mainSoundEngine, 0, NULL, &g_musicGroup) != MA_SUCCESS)
    {
        ma_sound_group_uninit(&g_sfxGroup);
        ma_engine_uninit(&g_mainSoundEngine);
        return false;
    }

    // initialize music
    ma_result result = ma_sound_init_from_file(&g_mainSoundEngine, "assets/audio/audio01.mp3", 0, &g_musicGroup, NULL, &g_backgroundMusic);
    // if successfully initialized set it to looping mode, so the music loops
    if (result == MA_SUCCESS) 
    {
        ma_sound_set_looping(&g_backgroundMusic, MA_TRUE);
    }

    return true;
}

// start/resume music
void startMusic(ma_sound& music)
{
    if (music.engineNode.pEngine == nullptr)
    {
        std::cout << "startMusic error: uninitialized ma_sound& object" << std::endl;
        return;
    }
    ma_sound_start(&music);
}

// pause music
void pauseMusic(ma_sound& music)
{
    if (music.engineNode.pEngine == nullptr)
    {
        std::cout << "pauseMusic error: uninitialized ma_sound& object" << std::endl;
        return;
    }
    ma_sound_stop(&music);
}

// switch the music on/off
void toggleMusic(ma_sound& music)
{
    if (music.engineNode.pEngine == nullptr)
    {
        std::cout << "toggleMusic error: uninitialized ma_sound& object" << std::endl;
        return;
    }
    if (ma_sound_is_playing(&music)) ma_sound_stop(&music);
    
    else ma_sound_start(&music);
}

// set the global volume
void setMasterVolume(float volume)
{
    g_masterVolume = volume;
    ma_engine_set_volume(&g_mainSoundEngine, g_masterVolume);
}

// return the value of the master volume
float getMasterVolume()
{
    float volume{ ma_engine_get_volume(&g_mainSoundEngine) };
    if (volume != g_masterVolume)
    {
        std::cout << "getMasterVolume warning: g_masterVolume contained wrong value" << std::endl;
        g_masterVolume = volume;
    }
    return g_masterVolume;
}

// set the global music volume
void setMusicVolume(float volume)
{
    // check if the music is initialized
    if (g_musicGroup.engineNode.pEngine == nullptr)
    {
        std::cout << "setMusicVolume error: uninitialized g_musicGroup object" << std::endl;
        return;
    }
    g_musicVolume = volume;
    ma_sound_group_set_volume(&g_musicGroup, g_musicVolume);
}

// return the value of the music volume
float getMusicVolume()
{
    float volume{ ma_sound_group_get_volume(&g_musicGroup) };
    if (volume != g_musicVolume)
    {
        std::cout << "getMusicVolume warning: g_musicVolume contained wrong value" << std::endl;
        g_musicVolume = volume;
    }
    return g_musicVolume;
}

// set the global sfx volume
void setSFXvolume(float volume)
{
    if (g_sfxGroup.engineNode.pEngine == nullptr)
    {
        std::cout << "setSFXvolume error: uninitialized g_sfxGroup object" << std::endl;
        return;
    }
    g_sfxVolume = volume;
    ma_sound_group_set_volume(&g_sfxGroup, g_sfxVolume);
}

// return the value of the sfx volume
float getSFXvolume()
{
    float volume{ ma_sound_group_get_volume(&g_sfxGroup) };
    if (volume != g_sfxVolume)
    {
        std::cout << "getSFXvolume warning: g_sfxVolume contained wrong value" << std::endl;
        g_sfxVolume = volume;
    }
    return g_sfxVolume;
}

// play the sfx with a given volume
void playSFX(const char* filePath, float volume)
{
    // set the volume for the sfx group
    ma_sound_group_set_volume(&g_sfxGroup, volume * g_sfxVolume);
    
    // play the sfx (on a internal thread of miniaudio)
    ma_result result = ma_engine_play_sound(&g_mainSoundEngine, filePath, &g_sfxGroup);
    
    if (result != MA_SUCCESS)
    {
        std::cout << "playSFX error: invalid sound path: " << filePath << std::endl;
    }
}

// cleanup
void audioCleanup()
{
    ma_sound_uninit(&g_backgroundMusic);
    ma_sound_group_uninit(&g_musicGroup);
    ma_sound_group_uninit(&g_sfxGroup);
    ma_engine_uninit(&g_mainSoundEngine);
}