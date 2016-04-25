#pragma once

#include "Sprite.h"
#include "Box2D\Box2D.h"

class RobotBullet
{
private:
	SDL_Rect m_rect;
	SDL_Rect m_gruntRect;

	int m_width;
	int m_height;

	float bulletDirX;
	float bulletDirY;

	b2Body* myBody;
	b2BodyDef myBodyDef;
	b2FixtureDef myBodyFixtureDef;
	Sprite* sprite;
	b2PolygonShape myShape;
	SDL_Texture* myTexture;

	bool m_facingRight;

	bool m_collided;

public:
	RobotBullet();
	RobotBullet(SDL_Texture* text, int width, int height, SDL_Rect pRect, b2World* wWorld, SDL_Rect src, bool facingRight, SDL_Rect gruntRect, float bulDirX, float bulDirY);

	void Draw();
	bool Update(b2Body* playerBody);
	void Destroy();

	bool OutOfBounds(SDL_Rect &playerRect);

	bool CheckBulletPlayerCollision(b2Body* playerBody);

	bool m_alive;

	bool GetAlive();
	void SetAlive(bool myAlive);

	bool Collided();
	void SetCollided(bool myCollided);
};
