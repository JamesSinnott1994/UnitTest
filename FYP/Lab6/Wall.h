#ifndef WALL_H
#define WALL_H

#include <iostream> 
#include "sdl.h"
#include "Box2D\Box2D.h"
#include "Sprite.h"
#include "Renderer.h"
#include "SoundManager.h"

class Wall
{
public:
	Wall();
	Wall(SDL_Rect, b2World*);

	void Destroy();

private:
	b2Body* m_body;
	b2BodyDef m_bodyDef;
	b2FixtureDef m_bodyFixtureDef;
	b2PolygonShape m_shape;
	SDL_Texture* m_texture;
	SDL_Rect m_rect;
};

#endif