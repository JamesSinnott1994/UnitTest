#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "Sprite.h"
#include "KeyBoardInput.h"
#include "Text.h"
#include "Timer.h"
#include <vector>

class Tutorial
{
private:
	Sprite* m_backGroundImage;
	Sprite* m_screenImage;
	Sprite* m_leftArrow;
	Sprite* m_rightArrow;

	//Globally used font
	TTF_Font *gFont = NULL;
	TTF_Font *gFont2 = NULL;

	//Rendered texture
	Text returnText;

	// Width and Height
	int width;
	int height;

	// Screens
	int screenNo;
	int noOfScreens;

	// Timer
	Timer* timer;

	// States
	const int MAIN_MENU = 1;

	// Bool
	bool m_drawLeftArrow;
	bool m_drawRightArrow;

	// Outer rect
	SDL_Rect* outerRect;

	// Squares
	list<SDL_Rect*> m_squares;
	int rectNo;
	list<SDL_Rect*>::iterator m_squareIterator;
	list<SDL_Rect*> m_squaresOutline;
	list<SDL_Rect*>::iterator m_squareOutlineIterator;
public:
	Tutorial();
	Tutorial(int windowWidth, int windowHeight);

	// Sqaures
	void CreateSquares();

	void Draw();
	int Update(SDL_Event* e);
	void MouseDown(SDL_Event* e);

	// Used for drawing text
	bool initializeTTF();
	bool loadTTFMedia();
};

#endif