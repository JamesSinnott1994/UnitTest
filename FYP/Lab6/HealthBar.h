#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "Sprite.h"

class HealthBar
{
public:
	HealthBar();

	void RenderHPBar(int x, int y, int w, int h, float percent, SDL_Color FGColor, SDL_Color BGColor);
	void RenderPlayerLives();
private:
	Sprite* m_playerLivesSprite;
};

#endif