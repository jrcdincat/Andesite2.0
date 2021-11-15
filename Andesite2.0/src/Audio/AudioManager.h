#pragma once
#include <SDL_mixer.h>

class AudioManager
{
public:
	static AudioManager* GetInstance() {
		return audioManagerInstance = (audioManagerInstance != nullptr) ? audioManagerInstance : new AudioManager();
	}
	~AudioManager();
	bool LoadAudio();
	void PlayMusic();
private:
	AudioManager();
	static AudioManager* audioManagerInstance;
	Mix_Music* gameMusic;
};