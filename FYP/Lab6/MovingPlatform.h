#ifndef MOVINGPLATFORM_H
#define MOVINGPLATFORM_H

#include "Sprite.h"
#include "Box2D\Box2D.h"

class MovingPlatform
{
private:
	b2Body* m_body;
	b2BodyDef m_bodyDef;
	b2FixtureDef m_bodyFixtureDef;
	Sprite m_sprite;
	b2PolygonShape m_shape;
	SDL_Texture* m_texture;
	SDL_Rect m_rect;
	SDL_Rect m_startRect;

	int direction;

	b2CircleShape sensorShape;

	std::string type;

	bool leftRight;
public:
	MovingPlatform();
	MovingPlatform(SDL_Texture*, SDL_Rect, b2World*, SDL_Rect src, string type, bool pLeftRight); // Actual moving platform
	MovingPlatform(SDL_Rect, b2World* wWorld, int dir); // SENSOR for moving platform

	static const int RIGHT = 1, LEFT = 2, UP = 3, DOWN = 4;

	void Update();
	void Draw();
	void Destroy();
	void Reset();

	void MoveLeftRightPlatform();
	void MoveUpDownPlatform();

	bool CheckCollision(b2Body* playerBody);

	std::string GetType();
	b2Body* GetBody();

	bool IsLeftRight();
};

#endif