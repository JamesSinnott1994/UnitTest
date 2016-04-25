#ifndef OPTIONS_H
#define OPTIONS_H

#include "Sprite.h"
#include "Button.h"
#include "Text.h"
#include "SoundManager.h"
#include "Timer.h"

class Options
{
public:
	Options(int, int);

	void Draw();
	int Update(SDL_Event e);

	// Options states for buttons
	const int OPTIONS = 0, MENU = 1;

	// Used for drawing text
	bool initializeTTF();
	bool loadTTFMedia();

	bool onBtnSelected;
	bool difficultyOnEasy;
private:
	Sprite* m_backGroundImage;
	Sprite* m_optionsImage;

	//Globally used font
	TTF_Font *gFont = NULL;

	//Rendered texture
	Text soundText;
	Text difficultyText;

	// Buttons
	Button m_onButton;
	Button m_offButton;
	Button m_easyButton;
	Button m_hardButton;
	Button m_backButton;

	// Width and Height of screen
	int width;
	int height;

	// Timer
	Timer* timer;

	// Prevents constant draw when over buttons
	bool m_overBackBtn;
	bool m_overOnBtn;
	bool m_overOffBtn;
	bool m_overEasyBtn;
	bool m_overHardBtn;
};

#endif