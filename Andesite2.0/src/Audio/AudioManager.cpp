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
	std::map<std::string, Mix_Chunk*>::iterator it;
	for (it = sfx.begin(); it != sfx.end(); it++)
	{
		Mix_FreeChunk(it->second);
	}
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

	Mix_Chunk* jumpSfx = Mix_LoadWAV("src/assets/sfx/jump.mp3");
	if (jumpSfx == NULL)
	{
		SDL_Log("Failed to load jump sfx: %s", Mix_GetError());
		return false;
	}
	sfx.insert(std::pair<std::string, Mix_Chunk*>("jump", jumpSfx));
	Mix_VolumeChunk(jumpSfx, 14);

	Mix_Chunk* explosionSfx = Mix_LoadWAV("src/assets/sfx/explosion.wav");
	if (jumpSfx == NULL)
	{
		SDL_Log("Failed to load explosion sfx: %s", Mix_GetError());
		return false;
	}
	sfx.insert(std::pair<std::string, Mix_Chunk*>("explosion", explosionSfx));
	Mix_VolumeChunk(explosionSfx, 14);

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

void AudioManager::PlaySfx(std::string sfxKey)
{
	Mix_PlayChannel(-1, sfx[sfxKey], 0);
}