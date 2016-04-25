#ifndef KEYBOARDINPUT_H
#define KEYBOARDINPUT_H

#include <iostream> 
#include <SDL.h>			//SDL
#include <stdio.h>
#include <list>

class KeyBoardInput
{
public:
	void updateKeyboard(SDL_Event e);
	bool isKeyPressed(SDL_Keycode key);
	void clearKeys();
	static KeyBoardInput* GetInstance();
	~KeyBoardInput()
	{
		instanceFlag = false;
	}
private:
	KeyBoardInput()
	{
		pressedKeys = std::list<SDL_Keycode>();
	}
	std::list<SDL_Keycode> pressedKeys;
	static bool instanceFlag;
	static KeyBoardInput* instance;
};
#endif

