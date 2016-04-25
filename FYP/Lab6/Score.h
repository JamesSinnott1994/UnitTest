#ifndef SCORE_H
#define SCORE_H

#include <iostream> 
#include "sdl.h"
#include "Box2D\Box2D.h"
#include "Sprite.h"
#include "Renderer.h"
#include "SoundManager.h"

class Score
{
public:
	Score();
	Score(SDL_Texture*, SDL_Rect, b2World*, SDL_Rect src, string speedType);

	void Draw();
	void Update();
	void Reset();
	void Destroy();

	bool CheckCollision(b2Body* playerBody);

	bool GetAlive();
	void SetAlive(bool);

	void SpriteClips();

private:
	b2Body* myBody;
	b2BodyDef myBodyDef;
	b2FixtureDef myBodyFixtureDef;
	b2PolygonShape myShape;

	bool m_alive;

	// Animation
	Sprite* m_animationSprite;
	const int ANIMATION_FRAMES = 6;
	SDL_Rect gSpriteClips[6];
	SDL_Rect* currentClip;
	int m_animationFrames;

	int m_animationTime;
	int m_limit;
	int m_animationTimeLaptop;
	int m_animationTimeLab;
};

#endif