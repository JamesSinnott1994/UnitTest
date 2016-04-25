#ifndef AMMO_H
#define AMMO_H

#include <iostream> 
#include "sdl.h"
#include "Box2D\Box2D.h"
#include "Sprite.h"
#include "Renderer.h"
#include "SoundManager.h"

class Ammo
{
private:
	b2Body* myBody;
	b2BodyDef myBodyDef;
	b2FixtureDef myBodyFixtureDef;
	Sprite sprite;
	b2PolygonShape myShape;
	SDL_Texture* myTexture;

	SDL_Rect gSpriteClips;
	SDL_Rect myRect;

	bool m_alive;
	bool m_generated;
public:
	Ammo();
	Ammo(SDL_Texture*, SDL_Rect, b2World*, SDL_Rect src);
	Ammo(SDL_Texture*, SDL_Rect, b2World*, SDL_Rect src, int type);
	// Public member functions
	void Draw();
	void Reset();
	void Destroy();

	bool CheckCollision(b2Body* playerBody);

	bool GetAlive();
	void SetAlive(bool);
};

#endif