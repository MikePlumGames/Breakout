#pragma once
#include "miniaudio.h"

// globals from Audio.cpp
extern ma_engine g_mainSoundEngine;
extern ma_sound g_backgroundMusic;

// local group for playing sfx with a specific separate volume
extern ma_sound_group g_sfxGroup;
// same thing for music
extern ma_sound_group g_musicGroup;

extern float g_masterVolume;
extern float g_sfxVolume;
extern float g_musicVolume;
extern float g_masterVolumeCurrent;

// all sound effects
struct SoundEffects
{
	inline static const char* bleepHigh{"assets/audio/sfx/bleepHigh.wav"};
	inline static const char* bleepLow{"assets/audio/sfx/bleepLow.wav"};
	inline static const char* metalImpact{"assets/audio/sfx/metalImpact.wav"};
	inline static const char* powerup{"assets/audio/sfx/powerup.wav"};
};


// default volume is 1 (100%)

// functions from Audio.cpp

bool audioInit();									// initialize the engine and music
void startMusic(ma_sound& music);					// start the background music
void pauseMusic(ma_sound& music);					// pause the background music
void toggleMusic(ma_sound& music);					// switch the music on/off
void setMasterVolume(float volume);					// set the master volume
float getMasterVolume();							// return the value of the master volume
void setMusicVolume(float volume);					// set the global music volume
float getMusicVolume();								// return the value of the music volume
void setSFXvolume(float volume);					// set the global sfx volume
float getSFXvolume();								// return the value of the sfx volume
void playSFX(const char* filePath, float volume);	// play the sfx with a given volume
void audioCleanup();								// cleanup