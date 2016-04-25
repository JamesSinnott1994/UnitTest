#ifndef MINE_H
#define MINE_h

#include <iostream> 
#include "sdl.h"
#include "Box2D\Box2D.h"
#include "Sprite.h"
#include "Renderer.h"
#include "SoundManager.h"

class Mine
{
public:
	Mine();
	Mine(SDL_Texture*, SDL_Rect, b2World*, SDL_Rect src);

	void Draw();
	void Update();
	void Reset();
	void Destroy();

	bool CheckCollision(b2Body* playerBody);

	bool GetAlive();
	void SetAlive(bool);

private:
	b2Body* myBody;
	b2BodyDef myBodyDef;
	b2FixtureDef myBodyFixtureDef;
	Sprite sprite;
	b2PolygonShape myShape;
	SDL_Texture* myTexture;

	SDL_Rect myRect;

	bool m_alive;

	bool m_animationPlayed;
	bool m_startAnimation;
};

#endif