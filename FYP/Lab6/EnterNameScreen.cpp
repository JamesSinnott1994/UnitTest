#include "stdafx.h"
#include "EnterNameScreen.h"

EnterNameScreen::EnterNameScreen(int windowWidth, int windowHeight)
{
	// Create background image
	m_backGroundImage = new Sprite();
	m_backGroundImage->Init("Images/Backgrounds/space.png",
		SDL_Rect{ windowWidth / 2, windowHeight / 2, windowWidth, windowHeight }, SDL_Rect{ 0, 0, 600, 360 });
	m_backGroundImage->SetOffset(SDL_Point{ windowWidth / 2, windowHeight / 2 });

	initializeTTF();
	loadTTFMedia();
}

bool EnterNameScreen::initializeTTF()
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

bool EnterNameScreen::loadTTFMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("Font/timesbd.ttf", 60);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 255, 165, 0 };
		if (!enterNameText.loadFromRenderedText("Enter Name", textColor, gFont))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	return success;
}

void EnterNameScreen::Draw()
{
	Renderer::GetInstance()->ClearRenderer();
	m_backGroundImage->DrawNoCamOffset();
	enterNameText.render(400 - enterNameText.getWidth()/2, 100);
	Renderer::GetInstance()->RenderScreen();
}

int EnterNameScreen::Update()
{
	cout << "ENTER NAME:" << endl;
	cin >> name;

	if (name != "")
	{
		SoundManager::GetInstance()->stopMusic();
		return 1;
	}
	else
		return 0;
}

string EnterNameScreen::GetName()
{
	return name;
}