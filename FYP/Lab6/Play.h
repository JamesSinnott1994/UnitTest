#ifndef _PLAY_H_
#define _PLAY_H_

#include "Player.h"
#include "Level.h"
#include "PickupManager.h"
#include "HealthBar.h"
#include "InGameMenu.h"
#include "Splash.h"

class Play
{
private:
	b2World* world;
	Player* m_player;
	bool levelComplete;

	Sprite* m_backGroundImage;
	Sprite* m_ammoHUD;

	// Splash screen object
	Splash* splash;

	// Levels
	int noOfLevels;

	HealthBar* m_healthBar;

	//Globally used font
	TTF_Font *gFont = NULL;
	bool fontLoaded;

	//Rendered texture
	Text gScoreTextTexture;
	Text gLevelTextTexture;
	Text gLivesTextTexture;
	Text gMGAmmoTextTexture;
	int mgAmmo;

	// Different speeds
	float labComputerSpeed;
	float laptopSpeed;
	float speedToUse;

	// Screen width and height
	int m_width;
	int m_height;

	Level* level;

	// Timer
	Timer* timer;

	string whichSpeed;

	// Menu
	bool menuOpen;
	bool paused;

public:
	Play(b2World*, int w, int h, Splash* pSplash);
	~Play(){}

	void Init(b2World*, int w, int h, Splash* pSplash);
	void AddAssetsToRenderer();
	int Update(SDL_Event e);
	void Reset();
	void Quit();
	void Draw();
	int LevelComplete();
	void UpdateCameraPos();
	void HandleSplash();

	bool initializeTTF();
	bool loadTTFMedia();

	int GetScore();
};
#endif
