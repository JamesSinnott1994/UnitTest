#ifndef GRUNT_H
#define GRUNT_H

#include "Sprite.h"
#include "GruntBullet.h"
#include "PickupManager.h"
#include "PlatformManager.h"
#include <list>

class Grunt
{
private:
	// Box2D
	b2World* world;
	b2Body* m_body;
	b2BodyDef m_bodyDef;
	b2FixtureDef m_bodyFixtureDef;
	b2PolygonShape m_shape;

	// Sprite
	Sprite* m_idleSprite;

	// SDL
	SDL_Texture* m_texture;
	SDL_Rect m_rect;
	SDL_Rect m_resetRect;// Used for reset
	SDL_Rect m_source;

	// Running animation
	Sprite* m_runningSprite;
	const int RUNNING_ANIMATION_FRAMES = 8;
	SDL_Rect gSpriteRunningClipsRight[8];
	SDL_Rect gSpriteRunningClipsLeft[8];
	SDL_Rect* currentRunnerClip;
	int m_runningFrames;
	int m_runningAnimationTime;
	int m_runningAnimationLimit;
	int m_runningAnimationLimitLab;
	int m_runningAnimationLimitLaptop;

	// Health
	int m_health;

	// Boolean
	bool m_facingRight;
	bool m_movingRight;
	bool m_movingLeft;
	bool m_idle;
	bool m_running;
	bool m_shooting;
	bool m_inRange;
	bool m_dead;
	bool m_alive;
	bool m_playerToTheLeft;
	bool m_drawn;// For drawing idle image when changing direction
	bool m_canCreateBullet;

	// For player score
	bool m_scoreAdded;

	// Strings used to read in images
	string colorString;
	string directionString;
	string file;

	// Screen width and height
	int m_width;
	int m_height;

	// Shooting
	int m_shootTimer;
	int m_shootTimerLaptop;
	int m_shootTimerLab;
	int m_shootTimerLimit;

	// Bullets
	list<GruntBullet*> m_bullets;
	list<GruntBullet*>::iterator m_bulletIterator;
	float bulletDirX;
	float bulletDirY;
	float angle;
	bool angleOkToShoot;

	// Raycasting
	b2Vec2 p1;
	b2Vec2 p2;
	float rayLength;
	b2RayCastInput input;
	b2RayCastOutput output;
	bool cantFire;
	b2Vec2 intersectionPoint;
	bool playerAbove;

public:
	Grunt();

	Grunt(SDL_Rect pRect, b2World* wWorld, int color, int direction, string speedType, int width, int height);

	void LoadAssets(int color, int direction);
	void SpriteClips();

	// Grunt states
	enum State { IDLE, RUNNING, SHOOTING, DEAD };
	State FiniteStateMachine();

	void Draw();
	void Update(SDL_Rect &playerRect, int noOfBullets, int maxBullets, b2Body* playerBody);
	void Reset();
	void ResetTimer();
	void Destroy();

	void CastRay();

	void Animation();
	void Running();
	void Shoot(int noOfBullets, int maxBullets);
	list<GruntBullet*> CreateBullet(list<GruntBullet*>m_gruntBullets);

	bool InRange();
	void SetInRange(bool);
	bool InRangeOfPlayer(SDL_Rect &playerRect);

	bool CanCreateBullet();
	void SetCanCreateBullet(bool myCanCreateBullet);

	bool GetAlive();
	void SetAlive(bool);

	void ChangeDirection(int &playerXPos);
	bool PlayerToTheLeft();
	void SetPlayerToTheLeft(bool);

	void GetDirectionToPlayer(SDL_Rect* playerRect);

	bool GruntCheckCollision(b2Body* bulletBody, bool hasMachineGun);
	bool CheckBulletPlayerCollision(b2Body* playerBody);

	void Fell();

	bool GetScoreAdded();
	void SetScoreAdded(bool myScoreAdded);

	bool OnScreen(float playerXPos, float playerYPos);
};

#endif