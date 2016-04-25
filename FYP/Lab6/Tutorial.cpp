#include "stdafx.h"
#include "Tutorial.h"

Tutorial::Tutorial()
{

}

Tutorial::Tutorial(int windowWidth, int windowHeight)
{
	// Width and Height
	width = windowWidth;
	height = windowHeight;

	// Create background image
	m_backGroundImage = new Sprite();
	m_backGroundImage->Init("Images/Backgrounds/space4.png",
		SDL_Rect{ windowWidth / 2, windowHeight / 2, windowWidth, windowHeight }, SDL_Rect{ 0, 0, 3840, 2400 });
	m_backGroundImage->SetOffset(SDL_Point{ windowWidth / 2, windowHeight / 2 });

	// Right arrow
	m_rightArrow = new Sprite();
	m_rightArrow->Init("Images/Tutorial/RightArrow.png",
		SDL_Rect{ windowWidth - 60, (windowHeight / 2) - 34, 52, 69 }, SDL_Rect{ 0, 0, 52, 69 });

	// Left arrow
	m_leftArrow = new Sprite();
	m_leftArrow->Init("Images/Tutorial/LeftArrow.png",
		SDL_Rect{ 10, (windowHeight / 2) - 34, 52, 69 }, SDL_Rect{ 0, 0, 52, 69 });

	// Screen Number
	screenNo = 1;
	noOfScreens = 7;

	// Screen Image
	m_screenImage = new Sprite();
	m_screenImage->Init("Images/Tutorial/Screen" + to_string(screenNo) + ".png",
		SDL_Rect{ windowWidth *0.1f, windowHeight * 0.1f, windowWidth * 0.8f, windowHeight * 0.8f }, SDL_Rect{ 0, 0, 800, 600 });

	// Timer
	timer = new Timer();

	// Bool
	m_drawLeftArrow = false;
	m_drawRightArrow = true;

	// Outer rect
	outerRect = new SDL_Rect{ 80 - 5, 60 - 5, 640 + 10, 480 + 10 };

	// Sqaures
	CreateSquares();

	initializeTTF();
	loadTTFMedia();
}

void Tutorial::CreateSquares()
{
	int x = 0;
	int x2 = 0;
	int allSquaresWidth = noOfScreens * 40;
	rectNo = 1;
	for (int i = 0; i < noOfScreens; i++)
	{
		SDL_Rect* temp = new SDL_Rect{ (outerRect->x + outerRect->w / 2) + x - (allSquaresWidth/2), 20, 20, 20 };
		x += 40;
		m_squares.push_back(temp);

		SDL_Rect* temp2 = new SDL_Rect{ (outerRect->x + outerRect->w / 2) + x2 - 5 - (allSquaresWidth/2), 15, 30, 30 };
		x2 += 40;
		m_squaresOutline.push_back(temp2);
	}
}

bool Tutorial::initializeTTF()
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

bool Tutorial::loadTTFMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("Font/timesbd.ttf", 20);
	gFont2 = TTF_OpenFont("Font/FORTE.ttf", 30);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 255, 135, 0 };
		if (!returnText.loadFromRenderedText("Press 'Enter' to return", textColor, gFont2))
		{
			printf("Failed to render text texture!\n");
			success = false;
		}
	}

	return success;
}

void Tutorial::Draw()
{
	Renderer::GetInstance()->ClearRenderer();

	m_backGroundImage->DrawNoCamOffset();

	// Sqaure outlines
	for each(SDL_Rect* rect in m_squaresOutline)
	{
		SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 0, 0, 0, 0);// Black
		SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), rect);
	}

	// Draw sqaures which tell you what screen you're on
	for each(SDL_Rect* rect in m_squares)
	{
		if (rectNo == screenNo)
		{
			SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 255, 127, 39, 0);// Orange
			SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), rect);
		}
		else
		{
			SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 255, 255, 255, 0);// White
			SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), rect);
		}
		rectNo++;
	}
	rectNo = 1;

	// Draw arrows
	if (m_drawRightArrow)
		m_rightArrow->DrawNoCamOffset();
	if (m_drawLeftArrow)
		m_leftArrow->DrawNoCamOffset();

	// Draws blue rectangle outside screenshot
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), 0, 162, 232, 255);// LightBlue
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), outerRect);

	// Screenshot image
	m_screenImage->DrawNoCamOffset();

	// Return text
	returnText.render(400 - (returnText.getWidth() / 2), 593 - (returnText.getHeight()));

	Renderer::GetInstance()->RenderScreen();
}

int Tutorial::Update(SDL_Event* e)
{
	// Start timer if not already started
	if (!timer->isStarted())
	{
		timer->start();
	}

	// Left arrow key
	if ((KeyBoardInput::GetInstance()->isKeyPressed(SDLK_LEFT) || KeyBoardInput::GetInstance()->isKeyPressed(SDLK_a)) && timer->getTicks() / 1000 > 0)
	{
		timer->reset();
		if (screenNo > 1)
		{
			screenNo -= 1;
			m_screenImage->Init("Images/Tutorial/Screen" + to_string(screenNo) + ".png",
				SDL_Rect{ width *0.1f, height * 0.1f, width * 0.8f, height * 0.8f }, SDL_Rect{ 0, 0, 800, 600 });
			loadTTFMedia();
		}
	}

	// Right arrow key
	if ((KeyBoardInput::GetInstance()->isKeyPressed(SDLK_RIGHT) || KeyBoardInput::GetInstance()->isKeyPressed(SDLK_d)) && timer->getTicks() / 1000 > 0)
	{
		timer->reset();
		if (screenNo < noOfScreens)
		{
			screenNo += 1;
			m_screenImage->Init("Images/Tutorial/Screen" + to_string(screenNo) + ".png",
				SDL_Rect{ width *0.1f, height * 0.1f, width * 0.8f, height * 0.8f }, SDL_Rect{ 0, 0, 800, 600 });
			loadTTFMedia();
		}
	}

	// Mouse down
	MouseDown(e);

	// Go back to main menu
	if (KeyBoardInput::GetInstance()->isKeyPressed(SDLK_RETURN) || KeyBoardInput::GetInstance()->isKeyPressed(SDLK_ESCAPE))
	{
		screenNo = 1;
		m_screenImage->Init("Images/Tutorial/Screen" + to_string(screenNo) + ".png",
			SDL_Rect{ width *0.1f, height * 0.1f, width * 0.8f, height * 0.8f }, SDL_Rect{ 0, 0, 800, 600 });
		return MAIN_MENU;
	}

	// Left Arrow
	if (screenNo > 1)
	{
		m_drawLeftArrow = true;
	}
	else
	{
		m_drawLeftArrow = false;
	}

	// Right Arrow
	if (screenNo < noOfScreens)
	{
		m_drawRightArrow = true;
	}
	else
	{
		m_drawRightArrow = false;
	}
	return 0;
}

void Tutorial::MouseDown(SDL_Event* e)
{
	// Mouse down
	if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		//If the left mouse button was pressed
		if (e->button.button == SDL_BUTTON_LEFT)
		{
			//Get the mouse offsets
			int mouse_x = e->button.x;
			int mouse_y = e->button.y;

			// Check over left or right arrow
			// LEFT
			if (m_drawLeftArrow)
			{
				if (mouse_x > m_leftArrow->getDestRect().x
					&& mouse_x < m_leftArrow->getDestRect().x + m_leftArrow->getDestRect().w
					&& mouse_y > m_leftArrow->getDestRect().y
					&& mouse_y < m_leftArrow->getDestRect().y + m_leftArrow->getDestRect().h
					&& timer->getTicks() / 1000 > 0)
				{
					timer->reset();
					if (screenNo > 1)
					{
						screenNo -= 1;
						m_screenImage->Init("Images/Tutorial/Screen" + to_string(screenNo) + ".png",
							SDL_Rect{ width *0.1f, height * 0.1f, width * 0.8f, height * 0.8f }, SDL_Rect{ 0, 0, 800, 600 });
						loadTTFMedia();
					}
				}
			}

			// RIGHT
			if (m_drawRightArrow)
			{
				if (mouse_x > m_rightArrow->getDestRect().x
					&& mouse_x < m_rightArrow->getDestRect().x + m_rightArrow->getDestRect().w
					&& mouse_y > m_rightArrow->getDestRect().y
					&& mouse_y < m_rightArrow->getDestRect().y + m_rightArrow->getDestRect().h
					&& timer->getTicks() / 1000 > 0)
				{
					timer->reset();
					if (screenNo < noOfScreens)
					{
						screenNo += 1;
						m_screenImage->Init("Images/Tutorial/Screen" + to_string(screenNo) + ".png",
							SDL_Rect{ width *0.1f, height * 0.1f, width * 0.8f, height * 0.8f }, SDL_Rect{ 0, 0, 800, 600 });
						loadTTFMedia();
					}
				}
			}
		}
	}
}