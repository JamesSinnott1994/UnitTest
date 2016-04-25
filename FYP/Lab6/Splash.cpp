#include "stdafx.h"
#include "Splash.h"

Splash::Splash(int width, int height)
{
	// Create splash image
	m_image = new Sprite();
	m_image->Init("Images/Backgrounds/space.png",
		SDL_Rect{ width / 2, height / 2, width, height }, SDL_Rect{ 0, 0, 600, 360 });
	m_image->SetOffset(SDL_Point{ width / 2, height / 2 });

	// Set can draw
	m_canDraw = true;
}

void Splash::Draw()
{
	Renderer::GetInstance()->ClearRenderer();
	m_image->DrawNoCamOffset();
	Renderer::GetInstance()->RenderScreen();
}

bool Splash::CanDraw()
{
	return m_canDraw;
}

void Splash::SetCanDraw(bool myCanDraw)
{
	m_canDraw = myCanDraw;
}