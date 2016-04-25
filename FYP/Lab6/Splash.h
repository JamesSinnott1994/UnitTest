#pragma once

#include "Sprite.h"

class Splash
{
private:
	Sprite* m_image;
	bool m_canDraw;
public:
	Splash(int width, int height);

	void Draw();

	void SetCanDraw(bool myCanDraw);
	bool CanDraw();
};