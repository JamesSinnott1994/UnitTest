#ifndef INGAMEMENU_H
#define INGAMEMENU_H

#include "Text.h"
#include "Button.h"
#include "SoundManager.h"

class InGameMenu
{
private:
	InGameMenu()
	{}

	static bool instanceFlag;
	static InGameMenu* instance;

	//Globally used font
	TTF_Font *gFont = NULL;

	//Rendered texture
	Text menuTitle;
	bool menuTitleLoaded;
	Text resumeText;
	bool resumeTextLoaded;
	Text quitText;
	bool quitTextLoaded;
	Text optionsText;
	bool optionsTextLoaded;
	Text soundText;
	bool soundTextLoaded;
	Text returnText;
	bool returnTextLoaded;

	bool soundOn;

	int textCouter;

	bool options;

	// Buttons
	Button m_resumeButton;
	Button m_quitButton;
	Button m_optionsButton;

	Button m_soundButton;
	Button m_returnButton;

	int time;
	int soundOnTimer;
	int soundOffTimer;

	// Menu states for buttons
	const int RESUME = 0;
	const int INGAMEMENU = 1;
	const int QUIT = 2;
	const int OPTIONS = 3;

public:
	static InGameMenu* GetInstance();
	~InGameMenu()
	{
		instanceFlag = false;
		menuTitleLoaded = false;
		resumeTextLoaded = false;
		quitTextLoaded = false;
		optionsTextLoaded = false;
		soundTextLoaded = false;
		returnTextLoaded = false;

		soundOn = false;
		textCouter = 0;

		time = 0;
		soundOnTimer = 0;
		soundOffTimer = 0;
	}

	void Draw(int width, int height);
	void DrawMenu(int menuTitleXPos, int innerRectHeight);
	void DrawOptions(int innerRectXPos, int innerRectYPos, int innerRectWidth, int innerRectHeight);
	int Update(SDL_Event e);

	// Used for drawing text
	bool initializeTTF();
	bool loadTTFMedia(int width, int textNum);
};

#endif