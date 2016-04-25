#ifndef ELECTRICBARRIER_H
#define ELECTRICBARRIER_H

#include <iostream> 
#include "sdl.h"
#include "Box2D\Box2D.h"
#include "Sprite.h"
#include "Renderer.h"
#include "SoundManager.h"

class ElectricBarrier
{
private:
	b2Body* myBody;
	b2BodyDef myBodyDef;
	b2FixtureDef myBodyFixtureDef;
	b2PolygonShape myShape;

	bool m_on;

	// Animation
	Sprite* m_animationSprite;
	const int ANIMATION_FRAMES = 3;
	SDL_Rect gSpriteClips[3];
	SDL_Rect* currentClip;
	int m_animationFrames;

	int m_animationTime;
	int m_limit;
	int m_animationTimeLaptop;
	int m_animationTimeLab;

	int switchID;
	int barrierID;

public:
	ElectricBarrier();
	ElectricBarrier(SDL_Texture*, SDL_Rect, b2World*, SDL_Rect src, string speedType, int id);

	void Draw();
	void Update();
	void Reset();
	void Destroy();

	bool CheckCollision(b2Body* playerBody);

	bool GetOn();
	void SetOn(bool);

	void SpriteClips();

	int GetSwitchID();
	void SetSwitchID(int myID);
	bool hasSwitchID;

	int GetBarrierID();
	void SetBarrierID(int myID);
};

#endif