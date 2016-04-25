#include "stdafx.h"
#include "InGameMenu.h"

bool InGameMenu::instanceFlag = false;
InGameMenu* InGameMenu::instance = NULL;

InGameMenu* InGameMenu::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new InGameMenu;
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}

bool InGameMenu::initializeTTF()
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

bool InGameMenu::loadTTFMedia(int width, int textNum)
{
	//Loading success flag
	bool success = true;

	gFont = TTF_OpenFont("Font/timesbd.ttf", width);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render options text
		if (!optionsTextLoaded && quitTextLoaded && menuTitleLoaded && resumeTextLoaded)
		{
			SDL_Color textColor = { 255, 255, 255 };
			if (!optionsText.loadFromRenderedText("Options", textColor, gFont))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
			else
			{
				optionsTextLoaded = true;
			}
		}

		//Render quit text
		if (!quitTextLoaded && menuTitleLoaded && resumeTextLoaded)
		{
			SDL_Color textColor = { 255, 255, 255 };
			if (!quitText.loadFromRenderedText("Quit Game", textColor, gFont))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
			else
			{
				quitTextLoaded = true;
			}
		}

		//Render resume text
		if (!resumeTextLoaded && menuTitleLoaded)
		{
			SDL_Color textColor = { 255, 255, 255 };
			if (!resumeText.loadFromRenderedText("Resume Game", textColor, gFont))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
			else
			{
				resumeTextLoaded = true;
			}
		}

		//Render paused text
		if (!menuTitleLoaded)
		{
			SDL_Color textColor = { 255, 255, 255 };
			if (!menuTitle.loadFromRenderedText("Game Paused", textColor, gFont))
			{
				printf("Failed to render text texture!\n");
				success = false;
			}
			else
			{
				menuTitleLoaded = true;
			}
		}

		if (options)
		{
			if (soundTextLoaded && !returnTextLoaded)
			{
				SDL_Color textColor = { 255, 255, 255 };
				if (!returnText.loadFromRenderedText("Return", textColor, gFont))
				{
					printf("Failed to render text texture!\n");
					success = false;
				}
				else
				{
					returnTextLoaded = true;
				}
			}

			if (!soundTextLoaded)
			{
				SDL_Color textColor = { 255, 255, 255 };
				if (!soundText.loadFromRenderedText("Sound:", textColor, gFont))
				{
					printf("Failed to render text texture!\n");
					success = false;
				}
				else
				{
					soundTextLoaded = true;
				}
			}
		}
	}

	return success;
}

void InGameMenu::Draw(int width, int height)
{
	// Outer Rectangle
	int outerRectWidth = width / 2;
	int outerRectHeight = height * 0.75f;

	int outerRectXPos = (width / 2) - (outerRectWidth / 2);
	int outerRectYPos = (height / 2) - (outerRectHeight / 2);

	SDL_Rect outerRect = { outerRectXPos, outerRectYPos, outerRectWidth, outerRectHeight };
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 0, 162, 232, 255);// LightBlue
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), &outerRect);

	// Inner Rectangle
	int innerRectWidth = outerRectWidth * 0.95f;
	int innerRectHeight = outerRectHeight * 0.95f;

	int innerRectXPos = outerRectXPos + (outerRectWidth * 0.025f);
	int innerRectYPos = outerRectYPos + (outerRectHeight * 0.025f);

	SDL_Rect innerRect = { innerRectXPos, innerRectYPos, innerRectWidth, innerRectHeight };
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 0, 0, 0, 0);
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), &innerRect);

	// Menu title text
	initializeTTF();
	loadTTFMedia(innerRectWidth/10, textCouter);
	textCouter++;

	int menuTitleXPos = innerRectXPos + (innerRectWidth / 2) - (menuTitle.getWidth() / 2);
	int menuTitleYPos = innerRectYPos + (innerRectHeight*0.1f) - (menuTitle.getHeight() / 2);

	menuTitle.render(menuTitleXPos, menuTitleYPos);

	if (!options)
		DrawMenu(menuTitleXPos, innerRectHeight);
	else
		DrawOptions(innerRectXPos, innerRectYPos, innerRectWidth, innerRectHeight);
}

void InGameMenu::DrawMenu(int menuTitleXPos, int innerRectHeight)
{
	// Outer Resume Rectangle
	int outerResumeRectWidth = menuTitle.getWidth();
	int outerResumeRectHeight = menuTitle.getHeight();

	int outerResumeRectXPos = menuTitleXPos;
	int outerResumeRectYPos = (innerRectHeight * 0.5f);

	SDL_Rect resumeOuterRect = { outerResumeRectXPos, outerResumeRectYPos, outerResumeRectWidth, outerResumeRectHeight };
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 0, 162, 232, 255);
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), &resumeOuterRect);

	// Inner Resume Rectangle
	int innerResumeRectWidth = outerResumeRectWidth * 0.95f;
	int innerResumeRectHeight = outerResumeRectHeight * 0.95f;

	int innerResumeRectXPos = outerResumeRectXPos + (outerResumeRectWidth * 0.025f);
	int innerResumeRectYPos = outerResumeRectYPos + (outerResumeRectHeight * 0.025f);

	SDL_Rect resumeInnerRect = { innerResumeRectXPos, innerResumeRectYPos, innerResumeRectWidth, innerResumeRectHeight };
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 0, 0, 0, 0);
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), &resumeInnerRect);

	// Resume text
	loadTTFMedia(innerResumeRectWidth/10, textCouter);
	textCouter++;

	int resumeTitleXPos = innerResumeRectXPos + (innerResumeRectWidth / 2) - (resumeText.getWidth() / 2);
	int resumeTitleYPos = innerResumeRectYPos + (innerResumeRectHeight*0.5f) - (resumeText.getHeight() / 2);

	resumeText.render(resumeTitleXPos, resumeTitleYPos);

	// Outer Options Rectangle
	int outerOptionsRectWidth = menuTitle.getWidth();
	int outerOptionsRectHeight = menuTitle.getHeight();

	int outerOptionsRectXPos = menuTitleXPos;
	int outerOptionsRectYPos = (innerRectHeight * 0.7f);

	SDL_Rect optionsOuterRect = { outerOptionsRectXPos, outerOptionsRectYPos, outerOptionsRectWidth, outerOptionsRectHeight };
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 0, 162, 232, 255);
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), &optionsOuterRect);

	// Inner Options Rectangle
	int innerOptionsRectWidth = outerOptionsRectWidth * 0.95f;
	int innerOptionsRectHeight = outerOptionsRectHeight * 0.95f;

	int innerOptionsRectXPos = outerOptionsRectXPos + (outerOptionsRectWidth * 0.025f);
	int innerOptionsRectYPos = outerOptionsRectYPos + (outerOptionsRectHeight * 0.025f);

	SDL_Rect optionsInnerRect = { innerOptionsRectXPos, innerOptionsRectYPos, innerOptionsRectWidth, innerOptionsRectHeight };
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 0, 0, 0, 0);
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), &optionsInnerRect);

	// Options text
	loadTTFMedia(innerOptionsRectWidth/10, textCouter);
	textCouter++;

	int optionsTitleXPos = innerOptionsRectXPos + (innerOptionsRectWidth / 2) - (optionsText.getWidth() / 2);
	int optionsTitleYPos = innerOptionsRectYPos + (innerOptionsRectHeight*0.5f) - (optionsText.getHeight() / 2);

	optionsText.render(optionsTitleXPos, optionsTitleYPos);

	// Outer Quit Rectangle
	int outerQuitRectWidth = menuTitle.getWidth();
	int outerQuitRectHeight = menuTitle.getHeight();

	int outerQuitRectXPos = menuTitleXPos;
	int outerQuitRectYPos = (innerRectHeight * 0.9f);

	SDL_Rect quitOuterRect = { outerQuitRectXPos, outerQuitRectYPos, outerQuitRectWidth, outerQuitRectHeight };
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 0, 162, 232, 255);
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), &quitOuterRect);

	// Inner Quit Rectangle
	int innerQuitRectWidth = outerQuitRectWidth * 0.95f;
	int innerQuitRectHeight = outerQuitRectHeight * 0.95f;

	int innerQuitRectXPos = outerQuitRectXPos + (outerQuitRectWidth * 0.025f);
	int innerQuitRectYPos = outerQuitRectYPos + (outerQuitRectHeight * 0.025f);

	SDL_Rect quitInnerRect = { innerQuitRectXPos, innerQuitRectYPos, innerQuitRectWidth, innerQuitRectHeight };
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 0, 0, 0, 0);
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), &quitInnerRect);

	// Quit text
	loadTTFMedia(innerQuitRectWidth/10, textCouter);
	textCouter++;

	int quitTitleXPos = innerQuitRectXPos + (innerQuitRectWidth / 2) - (quitText.getWidth() / 2);
	int quitTitleYPos = innerQuitRectYPos + (innerQuitRectHeight*0.5f) - (quitText.getHeight() / 2);

	quitText.render(quitTitleXPos, quitTitleYPos);

	// Button
	m_resumeButton.InitNoImage(SDL_Rect{ innerResumeRectXPos, innerResumeRectYPos, innerResumeRectWidth, innerResumeRectHeight });
	m_optionsButton.InitNoImage(SDL_Rect{ innerOptionsRectXPos, innerOptionsRectYPos, innerOptionsRectWidth, innerOptionsRectHeight });
	m_quitButton.InitNoImage(SDL_Rect{ innerQuitRectXPos, innerQuitRectYPos, innerQuitRectWidth, innerQuitRectHeight });
}

void InGameMenu::DrawOptions(int innerRectXPos, int innerRectYPos, int innerRectWidth, int innerRectHeight)
{
	textCouter = 4;

	// Sound text
	initializeTTF();
	loadTTFMedia(innerRectWidth/20, textCouter);
	//textCouter++;

	int soundTextXPos = innerRectXPos + (innerRectWidth *0.25f) - (soundText.getWidth() / 2);
	int soundTextYPos = innerRectYPos + (innerRectHeight*0.5f) - (soundText.getHeight() / 2);

	soundText.render(soundTextXPos, soundTextYPos);

	// On button
	int onBtnSelectedWidth = soundText.getWidth() / 2;
	int onBtnSelectedHeight = soundText.getHeight();

	int onBtnSelectedXPos = soundTextXPos + (innerRectWidth *0.45f) - (onBtnSelectedWidth / 2);
	int onBtnSelectedYPos = soundTextYPos;

	SDL_Rect onBtnSelected = { onBtnSelectedXPos, onBtnSelectedYPos, onBtnSelectedWidth, onBtnSelectedHeight };
	if (SoundManager::GetInstance()->SoundOn())
		SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 0, 255, 0, 0);
	else
		SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 255, 0, 0, 0);
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), &onBtnSelected);

	// Button
	m_soundButton.InitNoImage(SDL_Rect{ onBtnSelectedXPos, onBtnSelectedYPos, onBtnSelectedWidth, onBtnSelectedHeight });

	// Return button
	int returnBtnWidth = soundText.getWidth();
	int returnBtnHeight = soundText.getHeight();

	int returnBtnXPos = (innerRectWidth)-(returnBtnWidth/2);
	int returnBtnYPos = innerRectHeight * 1.f;

	SDL_Rect returnBtnRect = { returnBtnXPos, returnBtnYPos, returnBtnWidth, returnBtnHeight };
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 0, 162, 232, 255);
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), &returnBtnRect);

	// Button
	m_returnButton.InitNoImage(SDL_Rect{ returnBtnXPos, returnBtnYPos, returnBtnWidth, returnBtnHeight });

	// Return text
	initializeTTF();
	loadTTFMedia(returnBtnWidth / 20, textCouter);

	int returnTextXPos = returnBtnXPos + (returnBtnWidth *0.5f) - (returnText.getWidth() / 2);
	int returnTextYPos = returnBtnYPos + (returnBtnHeight*0.5f) - (returnText.getHeight() / 2);

	returnText.render(returnTextXPos, returnTextYPos);
}

int InGameMenu::Update(SDL_Event e)
{
	if(options)
		time++;
	soundOnTimer++;
	soundOffTimer++;

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
			if (m_resumeButton.IsClicked(mouse_x, mouse_y) && !options)
			{
				return RESUME;
			}
			// Check if button clicked
			if (m_optionsButton.IsClicked(mouse_x, mouse_y) && !options)
			{
				options = true;
				return OPTIONS;
			}
			// Check if button clicked
			if (m_quitButton.IsClicked(mouse_x, mouse_y) && !options)
			{
				SoundManager::GetInstance()->stopMusic();
				return QUIT;
			}
			if (m_soundButton.IsClicked(mouse_x, mouse_y) && time > 400)// Lab speed 400, Laptop speed 60
			{
				bool clicked = false;
				bool clicked2 = false;

				if (SoundManager::GetInstance()->SoundOn() && !clicked && !clicked2)
				{
					SoundManager::GetInstance()->SetSoundOn(false);
					clicked = true;
				}
				if (!SoundManager::GetInstance()->SoundOn() && !clicked && !clicked2)
				{
					SoundManager::GetInstance()->SetSoundOn(true);
				}

				time = 0;
			}
			if (m_returnButton.IsClicked(mouse_x, mouse_y) && options)
			{
				time = 0;
				options = false;
			}
		}
	}

	return INGAMEMENU;
}

