#pragma once
#include <SDL_mixer.h>
#include <map>

class AudioManager
{
	public:
		static AudioManager* GetInstance() {
			return audioManagerInstance = (audioManagerInstance != nullptr) ? audioManagerInstance : new AudioManager();
		}
		~AudioManager();

		bool LoadAudio();
		void PlayMusic();
		void PlaySfx(std::string sfxKey);
	private:
		AudioManager();
		static AudioManager* audioManagerInstance;

		Mix_Music* gameMusic;
		std::map<std::string, Mix_Chunk*> sfx;
};