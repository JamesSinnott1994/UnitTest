#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <iostream> 
#include <SDL.h>
#include "SDL_mixer.h"

#pragma comment(lib, "SDL2_mixer.lib")

using namespace std;

class SoundManager
{
public:
	static const int SCORE_PICKUP = 0, GUNSHOT = 1, HEALTH = 2, MINE = 3, MENU_MUSIC = 4, LEVEL_ONE_MUSIC = 5, ELECTROCUTED = 6, GUN_PICKUP = 7, LEVEL_TWO_MUSIC = 8, LEVEL_THREE_MUSIC = 9, VICTORY = 10, SWITCH = 11, LEVEL_FOUR_MUSIC = 12;

	static SoundManager* GetInstance();
	void PlayMusic(int levelNo);
	void play(int);
	void stopMusic();
	bool load_files();
	void clean_up();
	~SoundManager()
	{
		instanceFlag = false;
	}

	bool SoundOn();
	void SetSoundOn(bool mySound);

	bool MainMenuMusicOn;
private:
	SoundManager()
	{
		m_soundOn = true;

		MainMenuMusicOn = true;

		//Initialize SDL_mixer
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		}
	}

	Mix_Chunk* scorePickup = NULL;
	Mix_Chunk* gunshot = NULL;
	Mix_Chunk* health = NULL;
	Mix_Chunk* mine = NULL;
	Mix_Chunk* electrocuted = NULL;
	Mix_Chunk* gunPickup = NULL;
	Mix_Chunk* switchSound = NULL;

	Mix_Music* menuMusic = NULL;
	Mix_Music* levelOneMusic = NULL;
	Mix_Music* levelTwoMusic = NULL;
	Mix_Music* levelThreeMusic = NULL;
	Mix_Music* levelFourMusic = NULL;
	Mix_Music* victory = NULL;

	static bool instanceFlag;
	static SoundManager* instance;
	bool m_soundOn;
	
};
#endif
