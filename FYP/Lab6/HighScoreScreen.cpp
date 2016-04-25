#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include "stdafx.h"
#include "HighScoreScreen.h"
#include <stdio.h>
#include "curl\curl.h"
#include <iostream>
#include "string.h"

#include <stdlib.h>
#include <sstream>
#include <map>
#include <list>
#include <vector>
#include <algorithm>

HighScoreScreen::HighScoreScreen(int windowWidth, int windowHeight)
{
	// Timer
	timer = new Timer();

	// Create background image
	m_backGroundImage = new Sprite();
	m_backGroundImage->Init("Images/Backgrounds/space.png",
		SDL_Rect{ windowWidth / 2, windowHeight / 2, windowWidth, windowHeight }, SDL_Rect{ 0, 0, 2560, 1024 });
	m_backGroundImage->SetOffset(SDL_Point{ windowWidth / 2, windowHeight / 2 });

	haveScores = false;
	tableLoaded = false;

	y = 200;

	initializeTTF();
	loadTTFMedia();
}

bool HighScoreScreen::initializeTTF()
{
	//Loading success flag
	bool success = true;

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		success = false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	return success;
}

bool HighScoreScreen::loadTTFMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("Font/timesbd.ttf", 40);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	gFont2 = TTF_OpenFont("Font/timesbd.ttf", 25);
	if (gFont2 == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else 
	{
		//Render text
		SDL_Color textColor = { 255, 165, 0 };
		if (!highscoreText.loadFromRenderedText("High Scores", textColor, gFont))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}

		LoadTable(textColor, gFont2);
	}

	return success;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

map<string, int> HighScoreScreen::HandleData(std::string data)
{
	// Create
	map<string, int> highscores;
	string name = "";
	string score = "";
	bool addToName = false;
	bool addToScore = false;

	// Iterate through all characters in string
	for (int x = 0; data.at(x) != '}'; x++)
	{
		if (isalpha(data.at(x))) // Letter
		{
			if (addToName)
			{
				name += data.at(x);
			}
		}
		else if (isdigit(data.at(x))) // Number
		{
			score += data.at(x);
		}
		else // Not letter or number
		{
			// Adding to name
			if (data.at(x) == '"' && !addToName)// Getting name
			{
				addToName = true;
			}
			else if (data.at(x) == '"' && addToName)// Got name
			{
				addToName = false;
			}

			// Adding to score
			if (data.at(x) == ',')
			{
				highscores[name] = atoi(score.c_str());
				name = "";
				score = "";
			}
		}
	}
	highscores[name] = atoi(score.c_str());
	name = "";
	score = "";
	haveScores = true;
	return highscores;
}

bool myFunction(std::pair<std::string, int> first, std::pair<std::string, int> second)
{
	return first.second > second.second;
}

void HighScoreScreen::GetServerData()
{
	curl_global_init(CURL_GLOBAL_ALL);
	CURL * myHandle = curl_easy_init();

	CURLcode result;

	std::string readBuffer;

	// URL
	string url = "http://52.16.238.168:443/score";

	// Sets URL
	curl_easy_setopt(myHandle, CURLOPT_URL, url.c_str());

	// Callback
	curl_easy_setopt(myHandle, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(myHandle, CURLOPT_WRITEDATA, &readBuffer);

	// Will perform an action.
	result = curl_easy_perform(myHandle);

	// Will clean it, so it can be used again.
	curl_easy_cleanup(myHandle);

	highscores = HandleData(readBuffer);

	std::vector<std::pair<std::string, int> > myVec(highscores.begin(), highscores.end());

	std::sort(myVec.begin(), myVec.end(), &myFunction);

	highscoreVector = myVec;
}

void HighScoreScreen::PostServerData(string pName, int pScore)
{
	curl_global_init(CURL_GLOBAL_ALL);
	CURL * myHandle = curl_easy_init();

	CURLcode result;

	string postMessage = "";
	string name = pName;
	string score = to_string(pScore);

	postMessage = "name=Test&name=" + name + "&score=" + score;

	// URL
	string url = "http://52.16.238.168:443/score";

	// Sets URL
	curl_easy_setopt(myHandle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(myHandle, CURLOPT_POSTFIELDS, postMessage.c_str());

	// Will perform an action.
	result = curl_easy_perform(myHandle);

	// Will clean it, so it can be used again.
	curl_easy_cleanup(myHandle);
}

void HighScoreScreen::Draw()
{
	Renderer::GetInstance()->ClearRenderer();
	m_backGroundImage->DrawNoCamOffset();
	highscoreText.render(400 - highscoreText.getWidth() / 2, 50);

	name0.render(450 - highscoreText.getWidth() / 2, 150);
	name1.render(450 - highscoreText.getWidth() / 2, 200);
	name2.render(450 - highscoreText.getWidth() / 2, 250);
	name3.render(450 - highscoreText.getWidth() / 2, 300);
	name4.render(450 - highscoreText.getWidth() / 2, 350);
	name5.render(450 - highscoreText.getWidth() / 2, 400);
	name6.render(450 - highscoreText.getWidth() / 2, 450);
	name7.render(450 - highscoreText.getWidth() / 2, 500);
	name8.render(450 - highscoreText.getWidth() / 2, 550);
	name9.render(450 - highscoreText.getWidth() / 2, 600);

	Renderer::GetInstance()->RenderScreen();
}

int HighScoreScreen::Update(SDL_Event e)
{
	// Start timer if not already started
	if (!timer->isStarted())
	{
		timer->start();
	}

	if (!haveScores)
		GetServerData();
	if (haveScores && !tableLoaded)
		loadTTFMedia();

	// Check if pressed enter
	if (KeyBoardInput::GetInstance()->isKeyPressed(SDLK_RETURN))
	{
		haveScores = false;
		tableLoaded = false;
		SoundManager::GetInstance()->stopMusic();
		return 1;
	}
	return 0;
}

void HighScoreScreen::LoadTable(SDL_Color textColor, TTF_Font* gFont2)
{
	// Iterate through the pairs of names and scores
	bool success;
	int count = 0;
	for (std::vector<std::pair<std::string, int>>::iterator it = highscoreVector.begin(); it != highscoreVector.end(); ++it)
	{
		string temp = it->first + ": " + to_string(it->second);
		if (count == 0)
		{
			if (!name0.loadFromRenderedText(temp, textColor, gFont2))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
		}
		else if (count == 1)
		{
			if (!name1.loadFromRenderedText(temp, textColor, gFont2))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
		}
		else if (count == 2)
		{
			if (!name2.loadFromRenderedText(temp, textColor, gFont2))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
		}
		else if (count == 3)
		{
			if (!name3.loadFromRenderedText(temp, textColor, gFont2))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
		}
		else if (count == 4)
		{
			if (!name4.loadFromRenderedText(temp, textColor, gFont2))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
		}
		else if (count == 5)
		{
			if (!name5.loadFromRenderedText(temp, textColor, gFont2))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
		}
		else if (count == 6)
		{
			if (!name6.loadFromRenderedText(temp, textColor, gFont2))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
		}
		else if (count == 7)
		{
			if (!name7.loadFromRenderedText(temp, textColor, gFont2))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
		}
		else if (count == 8)
		{
			if (!name8.loadFromRenderedText(temp, textColor, gFont2))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
		}
		else if (count == 9)
		{
			if (!name9.loadFromRenderedText(temp, textColor, gFont2))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
		}
		count++;

		if (count == highscoreVector.size())
		{
			tableLoaded = true;
		}
	}
}
