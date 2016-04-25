#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "Sprite.h"
#include "Renderer.h"

static class Button
{
private:
	SDL_Rect m_rect;
	Sprite* m_sprite;
	
public:

	Button();
	~Button()
	{
		delete m_sprite;
	}
	void Init(SDL_Rect pos,  SDL_Rect src , std::string path);
	void InitNoImage(SDL_Rect pos);
	bool IsClicked(int x, int y);
	bool IsOver(int x, int y);
	void Draw();
	SDL_Rect GetRectangle();
};
#endif