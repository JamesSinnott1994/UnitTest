#ifndef BULLET_H
#define BULLET_H

#include "Sprite.h"
#include "EnemyManager.h"

class Bullet
{
public:
	Bullet(SDL_Texture*, SDL_Rect, b2World*, SDL_Rect src, bool);

	void Draw();
	void Update();

	bool GetAlive();
	void SetAlive(bool);

	bool CheckBulletGruntCollision(bool hasMachineGun);
	bool CheckBulletRobotCollision();
	bool OutOfBounds(SDL_Rect);
	void Destroy();

	b2Body* getBody();
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
	bool m_movingRight;
	bool m_movingLeft;
};

#endif