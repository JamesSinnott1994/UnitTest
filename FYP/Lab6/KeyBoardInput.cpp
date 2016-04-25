#include "stdafx.h"
#include "KeyboardInput.h"


bool KeyBoardInput::instanceFlag = false;
KeyBoardInput* KeyBoardInput::instance = NULL;


KeyBoardInput* KeyBoardInput::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new KeyBoardInput;
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}
void KeyBoardInput::updateKeyboard(SDL_Event e)
{
	//User presses a key
	if (e.type == SDL_KEYDOWN)
	{
		pressedKeys.push_back(e.key.keysym.sym);
	}
	if (e.type == SDL_KEYUP)
	{
		for each (SDL_Keycode released in pressedKeys)
		{
			if (e.key.keysym.sym == released)
			{
				pressedKeys.remove(released);
				break;
			}
		}
	}
}
void KeyBoardInput::clearKeys()
{
	pressedKeys.clear();
}
bool KeyBoardInput::isKeyPressed(SDL_Keycode key)
{
	for each (SDL_Keycode pressed in pressedKeys)
	{
		if (key == pressed)
		{
			return true;
		}
	}
	return false;
}
