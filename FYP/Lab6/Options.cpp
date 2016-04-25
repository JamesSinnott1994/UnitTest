#include "stdafx.h"
#include "Options.h"

Options::Options(int windowWidth, int windowHeight)
{
	// Timer
	timer = new Timer();

	// Create background image
	m_backGroundImage = new Sprite();
	m_backGroundImage->Init("Images/Backgrounds/space.png",
		SDL_Rect{ windowWidth / 2, windowHeight / 2, windowWidth, windowHeight }, SDL_Rect{ 0, 0, 1280, 960 });
	m_backGroundImage->SetOffset(SDL_Point{ windowWidth / 2, windowHeight / 2 });

	// Create Options image
	m_optionsImage = new Sprite();
	float optionsWidth = 382;
	float optionsHeight = 184;
	m_optionsImage->Init("Images/Options/GameOptions.png",
		SDL_Rect{ windowWidth / 2 - (optionsWidth / 2), windowHeight *0.2f - (optionsHeight/2), optionsWidth, optionsHeight }, 
		SDL_Rect{ 0, 0, 382, 184 });

	// Create back button
	float backBtnWidth = 191/2;
	float backBtnHeight = 92/2;
	float backBtnXPos = windowWidth - backBtnWidth;
	float backBtnYPos = windowHeight - backBtnHeight;
	m_backButton.Init(SDL_Rect{ backBtnXPos, backBtnYPos, backBtnWidth, backBtnHeight },
		SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/BackButtonNotOver.png");

	// Create On button
	float onBtnWidth = 191/2;
	float onBtnHeight = 92/2;
	float onBtnXPos = windowWidth / 2 - (onBtnWidth / 2) + 100;
	float onBtnYPos = (windowHeight * 0.5f) - onBtnHeight;
	m_onButton.Init(SDL_Rect{ onBtnXPos, onBtnYPos, onBtnWidth, onBtnHeight },
		SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/OnBtnNotOver.png");

	// Create Off button
	float offBtnWidth = 191 / 2;
	float offBtnHeight = 92 / 2;
	float offBtnXPos = windowWidth / 2 - (offBtnWidth / 2) + 100;
	float offBtnYPos = (windowHeight * 0.5f) - offBtnHeight;
	m_offButton.Init(SDL_Rect{ offBtnXPos, offBtnYPos, offBtnWidth, offBtnHeight },
		SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/OffBtnNotOver.png");

	// Create Easy button
	float easyBtnWidth = 191 / 2;
	float easyBtnHeight = 92 / 2;
	float easyBtnXPos = windowWidth / 2 - (easyBtnWidth / 2) + 100;
	float easyBtnYPos = (windowHeight * 0.625f) - easyBtnHeight;
	m_easyButton.Init(SDL_Rect{ easyBtnXPos, easyBtnYPos, easyBtnWidth, easyBtnHeight },
		SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/EasyButtonNotOver.png");

	// Create Hard button
	float hardBtnWidth = 191 / 2;
	float hardBtnHeight = 92 / 2;
	float hardBtnXPos = windowWidth / 2 - (hardBtnWidth / 2) + 100;
	float hardBtnYPos = (windowHeight * 0.625f) - hardBtnHeight;
	m_hardButton.Init(SDL_Rect{ hardBtnXPos, hardBtnYPos, hardBtnWidth, hardBtnHeight },
		SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/HardButtonNotOver.png");

	// Booleans
	onBtnSelected = true;
	difficultyOnEasy = true;

	// Prevents constant draw when over buttons
	m_overBackBtn = false;
	m_overOnBtn = false;
	m_overOffBtn = false;
	m_overEasyBtn = false;
	m_overHardBtn = false;

	// Width and Height
	width = windowWidth;
	height = windowHeight;

	initializeTTF();
	loadTTFMedia();
}

bool Options::initializeTTF()
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

bool Options::loadTTFMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("Font/FORTE.ttf", 30);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 255, 165, 0 };
		if (!soundText.loadFromRenderedText("Sound Effects:", textColor, gFont))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
		else if (!difficultyText.loadFromRenderedText("Difficulty:", textColor, gFont))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	return success;
}

void Options::Draw()
{
	Renderer::GetInstance()->ClearRenderer();

	m_backGroundImage->DrawNoCamOffset();
	m_optionsImage->DrawNoCamOffset();
	m_backButton.Draw();

	// Sound on
	if (SoundManager::GetInstance()->SoundOn())
	{
		onBtnSelected = true;
		m_onButton.Draw();
	}
	else
	{
		onBtnSelected = false;
		m_offButton.Draw();
	}

	// Difficulty easy/hard
	if (difficultyOnEasy)
	{
		m_easyButton.Draw();
	}
	else
	{
		m_hardButton.Draw();
	}
	soundText.render(width * 0.15f + 100, height * 0.5f - soundText.getHeight() - 5);
	difficultyText.render(width * 0.15f + 100, height * 0.5f - soundText.getHeight() + 65);

	Renderer::GetInstance()->RenderScreen();
}

int Options::Update(SDL_Event e)
{
	SoundManager::GetInstance()->play(SoundManager::GetInstance()->MENU_MUSIC);

	// Start timer if not already started
	if (!timer->isStarted())
	{
		timer->start();
	}

	// Checks if mouse is over Back button
	#pragma region BackButton
	if (m_backButton.IsOver(e.button.x, e.button.y))
	{
		if (!m_overBackBtn)
		{
			m_backButton.Init(SDL_Rect{ m_backButton.GetRectangle().x, m_backButton.GetRectangle().y, m_backButton.GetRectangle().w, m_backButton.GetRectangle().h },
				SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/BackButtonOver.png");
		}
		m_overBackBtn = true;
	}
	else
	{
		if (m_overBackBtn)
		{
			m_backButton.Init(SDL_Rect{ m_backButton.GetRectangle().x, m_backButton.GetRectangle().y, m_backButton.GetRectangle().w, m_backButton.GetRectangle().h },
				SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/BackButtonNotOver.png");
		}
		m_overBackBtn = false;
	}
	#pragma endregion

	#pragma region On/Off Button
	// On
	if (onBtnSelected)
	{
		if (m_onButton.IsOver(e.button.x, e.button.y))
		{
			if (!m_overOnBtn)
			{
				m_onButton.Init(SDL_Rect{ m_onButton.GetRectangle().x, m_onButton.GetRectangle().y, m_onButton.GetRectangle().w, m_onButton.GetRectangle().h },
					SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/OnBtnOver.png");
			}
			m_overOnBtn = true;
		}
		else
		{
			if (m_overOnBtn)
			{
				m_onButton.Init(SDL_Rect{ m_onButton.GetRectangle().x, m_onButton.GetRectangle().y, m_onButton.GetRectangle().w, m_onButton.GetRectangle().h },
					SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/OnBtnNotOver.png");
			}
			m_overOnBtn = false;
		}
	}
	// Off
	if (!onBtnSelected)
	{
		if (m_offButton.IsOver(e.button.x, e.button.y))
		{
			if (!m_overOffBtn)
			{
				m_offButton.Init(SDL_Rect{ m_offButton.GetRectangle().x, m_offButton.GetRectangle().y, m_offButton.GetRectangle().w, m_offButton.GetRectangle().h },
					SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/OffBtnOver.png");
			}
			m_overOffBtn = true;
		}
		else
		{
			if (m_overOffBtn)
			{
				m_offButton.Init(SDL_Rect{ m_offButton.GetRectangle().x, m_offButton.GetRectangle().y, m_offButton.GetRectangle().w, m_offButton.GetRectangle().h },
					SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/OffBtnNotOver.png");
			}
			m_overOffBtn = false;
		}
	}
	#pragma endregion

	#pragma region Easy/Hard Button
	// Easy
	if (difficultyOnEasy)
	{
		if (m_easyButton.IsOver(e.button.x, e.button.y))
		{
			if (!m_overEasyBtn)
			{
				m_easyButton.Init(SDL_Rect{ m_easyButton.GetRectangle().x, m_easyButton.GetRectangle().y, m_easyButton.GetRectangle().w, m_easyButton.GetRectangle().h },
					SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/EasyButtonOver.png");
			}
			m_overEasyBtn = true;
		}
		else
		{
			if (m_overEasyBtn)
			{
				m_easyButton.Init(SDL_Rect{ m_easyButton.GetRectangle().x, m_easyButton.GetRectangle().y, m_easyButton.GetRectangle().w, m_easyButton.GetRectangle().h },
					SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/EasyButtonNotOver.png");
			}
			m_overEasyBtn = false;
		}
	}
	// Hard
	if (!difficultyOnEasy)
	{
		if (m_hardButton.IsOver(e.button.x, e.button.y))
		{
			if (!m_overHardBtn)
			{
				m_hardButton.Init(SDL_Rect{ m_hardButton.GetRectangle().x, m_hardButton.GetRectangle().y, m_hardButton.GetRectangle().w, m_hardButton.GetRectangle().h },
					SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/HardButtonOver.png");
			}
			m_overHardBtn = true;
		}
		else
		{
			if (m_overHardBtn)
			{
				m_hardButton.Init(SDL_Rect{ m_hardButton.GetRectangle().x, m_hardButton.GetRectangle().y, m_hardButton.GetRectangle().w, m_hardButton.GetRectangle().h },
					SDL_Rect{ 0, 0, 191, 92 }, "Images/Options/HardButtonNotOver.png");
			}
			m_overHardBtn = false;
		}
	}
	#pragma endregion

	#pragma region Mousedown
	// Mousedown
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		//If the left mouse button was pressed
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			//Get the mouse offsets
			int mouse_x = e.button.x;
			int mouse_y = e.button.y;


			// Check if button clicked
			if (m_backButton.IsClicked(mouse_x, mouse_y))
			{
				return MENU;
			}

			// Check if ON/OFF buttons are clicked
			if (onBtnSelected)
			{
				if (m_onButton.IsClicked(mouse_x, mouse_y) && (timer->getTicks()/1000) > 0)
				{
					timer->reset();
					onBtnSelected = false;
					SoundManager::GetInstance()->SetSoundOn(false);
				}
			}
			else
			{
				if (m_offButton.IsClicked(mouse_x, mouse_y) && (timer->getTicks() / 1000) > 0)
				{
					timer->reset();
					onBtnSelected = true;
					SoundManager::GetInstance()->SetSoundOn(true);
				}
			}

			// Check if EASY/HARD buttons are clicked
			if (difficultyOnEasy)
			{
				if (m_easyButton.IsClicked(mouse_x, mouse_y) && (timer->getTicks() / 1000) > 0)
				{
					timer->reset();
					difficultyOnEasy = false;
				}
			}
			else
			{
				if (m_hardButton.IsClicked(mouse_x, mouse_y) && (timer->getTicks() / 1000) > 0)
				{
					timer->reset();
					difficultyOnEasy = true;
				}
			}
		}
	}
	#pragma endregion

	return OPTIONS;
}