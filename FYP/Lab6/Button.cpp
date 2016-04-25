#include "stdafx.h"
#include "Button.h"

Button::Button()
{

}

void Button::Init(SDL_Rect pos, SDL_Rect src, std::string path)
{
	m_rect = pos;
	m_sprite = new Sprite();
	m_sprite->Init(path, pos, src);
}

void Button::InitNoImage(SDL_Rect pos)// For the In-Game menu
{
	m_rect = pos;
}

// Checks if you clicked on the button
bool Button::IsClicked(int x , int y)
{
	if (m_rect.x + m_rect.w > x && m_rect.x < x && m_rect.y + m_rect.h > y && m_rect.y < y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Check if the mouse cursor is over the button
bool Button::IsOver(int x, int y)
{
	if (m_rect.x + m_rect.w > x && m_rect.x < x && m_rect.y + m_rect.h > y && m_rect.y < y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Button::Draw()
{
	m_sprite->DrawNoCamOffset();
}

SDL_Rect Button::GetRectangle()
{
	return m_rect;
}
