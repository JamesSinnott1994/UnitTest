#ifndef PLATFORM_H
#define PLATFORM_H

#include "Sprite.h"

class Platform
{
public:
	Platform();
	Platform(SDL_Texture*, SDL_Rect, b2World*, SDL_Rect src);

	void Draw();
	void Destroy();

	bool CheckStaticPlatCollision(b2Body* playerBody);

	SDL_Rect* GetRect();

	b2Body* getBody();
	int id;


private:
	b2Body* m_body;
	b2BodyDef m_bodyDef;
	b2FixtureDef m_bodyFixtureDef;
	Sprite* m_sprite;
	b2PolygonShape m_shape;
	SDL_Texture* m_texture;
	SDL_Rect* m_rect;
};

#endif