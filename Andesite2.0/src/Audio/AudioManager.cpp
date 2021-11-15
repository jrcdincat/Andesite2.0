#include "../pch.h"
#include "AudioManager.h"

AudioManager* AudioManager::audioManagerInstance = nullptr;

AudioManager::AudioManager()
{
	gameMusic = NULL;
}

AudioManager::~AudioManager()
{
	Mix_FreeMusic(gameMusic);
	gameMusic = NULL;
}

bool AudioManager::LoadAudio()
{
	gameMusic = Mix_LoadMUS("src/assets/music/Andesite.wav");
	if (gameMusic == NULL)
	{
		SDL_Log("Failed to load Andesite music: %s", Mix_GetError());
		return false;
	}
	Mix_VolumeMusic(10);

	


	return true;
}

void AudioManager::PlayMusic()
{
	// Play music if no music is already playing
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(gameMusic, -1);
	}
}