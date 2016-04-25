#ifndef MENU_H
#define MENU_H

#include "Sprite.h"
#include "Button.h"
#include "SoundManager.h"

class Menu
{
public:
	Menu(int, int);

	void Draw();
	int Update(SDL_Event e, bool highscoreEnabled);

	// Menu states for buttons
	const int MENU = 0;
	const int PLAY = 1;
	const int OPTIONS = 2;
	const int EXIT = 3;
	const int TUTORIAL = 4;

	// Prevents constant draw when over buttons
	bool m_playImageOver;
	bool m_optionsImageOver;
	bool m_exitImageOver;
	bool m_tutorialImageOver;
private:
	Sprite* m_backGroundImage;

	Button m_playButton;
	Button m_optionsButton;
	Button m_exitButton;
	Button m_tutorialButton;
};

#endif