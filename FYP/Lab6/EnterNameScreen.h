#ifndef ENTERNAMESCREEN_H
#define ENTERNAMESCREEN_H

#include "Sprite.h"
#include "Text.h"
#include "KeyBoardInput.h"
#include "SoundManager.h"

class EnterNameScreen
{
private:
	Sprite* m_backGroundImage;

	//Globally used font
	TTF_Font *gFont = NULL;

	//Rendered texture
	Text enterNameText;
	string name;
public:
	EnterNameScreen(int, int);

	void Draw();
	int Update();

	// Used for drawing text
	bool initializeTTF();
	bool loadTTFMedia();

	string GetName();
};

#endif