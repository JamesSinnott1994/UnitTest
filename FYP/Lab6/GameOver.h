#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "Sprite.h"
#include "SoundManager.h"
#include "Timer.h"

class HighScoreScreen;

class GameOver
{
private:
	Sprite* m_backGroundImage;

	Timer* timer;

	bool scoreAddedToDatabase;
public:
	GameOver(int windowWidth, int windowHeight);

	void Draw();
	int Update(string name, HighScoreScreen* highscore, int score, bool highscoreEnabled);

	const int MENU = 1;

	void SetScoreAddedToDatabase(bool myScoreAddedToDatabase);
};

#endif