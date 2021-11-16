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

	// Player jumpSfx
	Mix_Chunk* jumpSfx = Mix_LoadWAV("src/assets/sfx/jump.mp3");
	if (jumpSfx == NULL)
	{
		SDL_Log("Failed to load jump sfx: %s", Mix_GetError());
		return false;
	}
	sfx.insert(std::pair<std::string, Mix_Chunk*>("jump", jumpSfx));
	Mix_VolumeChunk(jumpSfx, 14);

	// Player death sfx
	Mix_Chunk* playerDeathSfx = Mix_LoadWAV("src/assets/sfx/player_death.mp3");
	if (playerDeathSfx == NULL)
	{
		SDL_Log("Failed to load player death sfx: %s", Mix_GetError());
		return false;
	}
	sfx.insert(std::pair<std::string, Mix_Chunk*>("player_death", playerDeathSfx));
	Mix_VolumeChunk(playerDeathSfx, 14);

	// Player win sfx
	Mix_Chunk* playerWinSfx = Mix_LoadWAV("src/assets/sfx/player_win.mp3");
	if (playerWinSfx == NULL)
	{
		SDL_Log("Failed to load player win sfx: %s", Mix_GetError());
		return false;
	}
	sfx.insert(std::pair<std::string, Mix_Chunk*>("player_win", playerWinSfx));
	Mix_VolumeChunk(playerWinSfx, 15);

	// Rock explosion sfx
	Mix_Chunk* explosionSfx = Mix_LoadWAV("src/assets/sfx/explosion.wav");
	if (explosionSfx == NULL)
	{
		SDL_Log("Failed to load explosion sfx: %s", Mix_GetError());
		return false;
	}
	sfx.insert(std::pair<std::string, Mix_Chunk*>("explosion", explosionSfx));
	Mix_VolumeChunk(explosionSfx, 14);

	// Golem death sfx
	Mix_Chunk* golemDeathSfx = Mix_LoadWAV("src/assets/sfx/golem_death.mp3");
	if (golemDeathSfx == NULL)
	{
		SDL_Log("Failed to load golem death sfx: %s", Mix_GetError());
		return false;
	}
	sfx.insert(std::pair<std::string, Mix_Chunk*>("golem_death", golemDeathSfx));
	Mix_VolumeChunk(golemDeathSfx, 14);

	// Bat death sfx
	Mix_Chunk* batDeathSfx = Mix_LoadWAV("src/assets/sfx/bat_death.mp3");
	if (batDeathSfx == NULL)
	{
		SDL_Log("Failed to load bat death sfx: %s", Mix_GetError());
		return false;
	}
	sfx.insert(std::pair<std::string, Mix_Chunk*>("bat_death", batDeathSfx));
	Mix_VolumeChunk(batDeathSfx, 14);

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