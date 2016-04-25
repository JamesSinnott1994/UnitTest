#pragma once

#include "Sprite.h"
#include "Box2D\Box2D.h"

class NoRayPlatform
{
public:
	NoRayPlatform();
	NoRayPlatform(SDL_Texture*, SDL_Rect, b2World*, SDL_Rect src);

	void Draw();
	void Destroy();

private:
	b2Body* m_body;
	b2BodyDef m_bodyDef;
	b2FixtureDef m_bodyFixtureDef;
	Sprite* m_sprite;
	b2PolygonShape m_shape;
	SDL_Texture* m_texture;
	SDL_Rect* m_rect;
};