#include "stdafx.h"
#include "HealthBar.h"
#include "Renderer.h"

HealthBar::HealthBar()
{
	SDL_Rect m_rect = { 725, 25, 28, 38 };
	SDL_Rect m_source = { 0, 0, 52, 111 };

	m_playerLivesSprite = new Sprite();
	m_playerLivesSprite->Init("Images/Player/lifeImage.png", m_rect, m_source);
	m_playerLivesSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
}

/*
Render a Horizontal Percentage Bar
*/
void HealthBar::RenderHPBar(int x, int y, int w, int h, float percent, SDL_Color FGColor, SDL_Color BGColor)
{
	// Sort out if percent > 1 or < 0
	if (percent > 1.0f)
	{
		percent = 1.0f;
	}
	else if (percent < 0)
	{
		percent = 0;
	}

	// Background rectangle
	SDL_Rect bgrect = { x, y, w, h };
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), BGColor.r, BGColor.g, BGColor.b, BGColor.a);
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), &bgrect);

	// Get width of foreground rectangle based on percentage of health bar
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), FGColor.r, FGColor.g, FGColor.b, FGColor.a);
	int pw = (int)((float)w * percent);// Determines width of foreground rectangle

	// Foreground rectangle
	SDL_Rect fgrect = { x, y, pw, h };
	SDL_RenderFillRect(Renderer::GetInstance()->Get_SDL_RENDERER(), &fgrect);
	SDL_SetRenderDrawColor(Renderer::GetInstance()->Get_SDL_RENDERER(), FGColor.r, FGColor.g, FGColor.b, FGColor.a);
}

void HealthBar::RenderPlayerLives()
{
	m_playerLivesSprite->DrawNoCamOffset();
}