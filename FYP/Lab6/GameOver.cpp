#include "stdafx.h"
#include "GameOver.h"
#include "HighScoreScreen.h"

GameOver::GameOver(int windowWidth, int windowHeight)
{
	// Create background image
	m_backGroundImage = new Sprite();
	m_backGroundImage->Init("Images/Menu/GameOverScreen.png",
		SDL_Rect{ windowWidth / 2, windowHeight / 2, windowWidth, windowHeight }, SDL_Rect{ 0, 0, 800, 600 });
	m_backGroundImage->SetOffset(SDL_Point{ windowWidth / 2, windowHeight / 2 });

	// Timer
	timer = new Timer();

	// Boolean to prevent constantly adding to database
	scoreAddedToDatabase = false;
}

void GameOver::Draw()
{
	Renderer::GetInstance()->ClearRenderer();
	m_backGroundImage->DrawNoCamOffset();
	Renderer::GetInstance()->RenderScreen();
}

int GameOver::Update(string name, HighScoreScreen* highscore, int score, bool highscoreEnabled)
{
	SoundManager::GetInstance()->play(SoundManager::GetInstance()->MENU_MUSIC);

	if (highscoreEnabled && !scoreAddedToDatabase)
	{
		highscore->PostServerData(name, score);
		scoreAddedToDatabase = true;
	}

	// Start timer if not already started
	if (!timer->isStarted())
	{
		timer->start();
	}
	// Reset after 2 seconds
	if (timer->getTicks() / 1000 >= 4)
	{
		timer->reset();
		return MENU;
	}
	return 0;
}

void GameOver::SetScoreAddedToDatabase(bool myScoreAddedToDatabase)
{
	scoreAddedToDatabase = myScoreAddedToDatabase;
}