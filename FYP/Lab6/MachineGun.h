#ifndef MACHINEGUN_H
#define MACHINEGUN_H

#include "Box2D\Box2D.h"
#include "Sprite.h"
#include "SoundManager.h"

class MachineGun
{
private:
	b2Body* myBody;
	b2BodyDef myBodyDef;
	b2FixtureDef myBodyFixtureDef;
	b2PolygonShape myShape;

	SDL_Rect myRect;

	Sprite* m_sprite;

	bool m_pickedUp;

public:
	MachineGun();
	MachineGun(SDL_Texture*, SDL_Rect, b2World*, SDL_Rect src);

	void Draw();
	void Reset();
	void Destroy();

	bool CheckCollision(b2Body* playerBody);

	bool PickedUp();
	void SetPickedUp(bool myPickedUp);
};

#endif