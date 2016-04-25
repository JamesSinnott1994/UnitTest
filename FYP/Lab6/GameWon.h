#ifndef GAMEWON_H
#define GAMEWON_H

#include "Sprite.h"
#include "Button.h"
#include "KeyBoardInput.h"
#include "SoundManager.h"
#include "HighScoreScreen.h"
#include "Text.h"
class GameWon
{
private:
	Sprite* m_backGroundImage;

	// Buttons
	Button m_continueButton;

	// Prevents constant draw when over buttons
	bool m_overContinueBtn;

	bool scoreAddedToDatabase;

	TTF_Font *gFont = NULL;
	Text winTextTexture;

	string name2;
public:
	GameWon(int windowWidth, int windowHeight);

	void Draw();
	int Update(SDL_Event e, bool highscoreEnabled, string name, HighScoreScreen* highscore, int score);

	const int MENU = 1;

	void SetScoreAddedToDatabase(bool myScoreAddedToDatabase);

	bool initializeTTF();
	bool loadTTFMedia();
};

#endif