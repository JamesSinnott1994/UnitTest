#ifndef ELECTRICSWITCH_H
#define ELECTRICSWITCH_H

#include <iostream> 
#include "sdl.h"
#include "Box2D\Box2D.h"
#include "Sprite.h"
#include "Renderer.h"
#include "SoundManager.h"

class ElectricSwitch
{
private:
	b2Body* myBody;
	b2BodyDef myBodyDef;
	b2FixtureDef myBodyFixtureDef;
	Sprite sprite;
	b2PolygonShape myShape;
	SDL_Texture* onTexture;
	SDL_Texture* offTexture;

	SDL_Rect myRect;

	bool m_on;

	int m_id;
public:
	ElectricSwitch();
	ElectricSwitch(SDL_Texture*, SDL_Rect, b2World*, SDL_Rect src, int id);

	void Draw();
	void Update();
	void Reset();
	void Destroy();

	bool CheckCollision(b2Body* playerBody);

	bool GetOn();
	void SetOn(bool);

	int GetSwitchID();
	void SetSwitchID(int myID);
	bool hasBarrier;
};

#endif