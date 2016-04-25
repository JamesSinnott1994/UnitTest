#ifndef PLAYER_H
#define PLAYER_H

#include "sdl.h"
#include "Box2D\Box2D.h"
#include "Sprite.h"
#include "KeyBoardInput.h"
#include "PickupManager.h"
#include "ObstacleManager.h"
#include "Bullet.h"
#include "Teleporter.h"
#include "Timer.h"
#include "EnemyManager.h"
#include "PlatformManager.h"

class Player
{
public:
	Player();

	int OldScore;
	int OldHealth;
	int OldMachineGunAmmo;

	void Init(SDL_Rect, b2World*, string speedType, float scale);
	void Draw();
	void Update();
	void UpdateAnimations();
	void UpdateBullets();
	void Move();
	void MoveLeft();
	void MoveRight();
	void Jump();
	void Shoot();
	void Reset();
	void LevelComplete();
	void FinishedLevel();

	void SpriteClips();

	b2Body* getBody();

	bool m_movingLeft;
	bool m_movingRight;

	// Score
	int GetScore();
	void SetScore(int);

	// Health
	int GetHealth();
	void SetHealth(int);

	// Machine gun
	bool MachineGunPickedUp();
	void SetMachineGunPickedUp(bool myMachineGun);

	int GetMachineGunAmmo();
	void SetMachineGunAmmo(int myMachineGunAmmo);

	bool MachineGunEquipped();

	void ToggleWeapons();

	// Alive
	bool GetAlive();
	void SetAlive(bool);
	int GetLives();
	void SetLives(int);

	// Position
	SDL_Rect GetPosition();

	// Get Platform On
	std::vector<int> GetPlatformOn();

	// ReachedTeleporter
	bool GetReachedTeleporter();
	void SetReachedTeleporter(bool);

	// Collision Checks
	#pragma region Collision Checks
	void CheckCollisions();
	bool CheckScoreCollision();
	bool CheckHealthCollision();
	bool CheckAmmoCollision();
	bool CheckMineCollision();
	bool CheckBarrierCollision();
	bool CheckSwitchCollision();
	bool CheckTeleporterCollision();
	bool CheckMovingPlatformCollision();
	bool CheckMachineGunCollision();
	SDL_Rect* CheckStaticPlatCollision();
	#pragma endregion

private:
	SDL_Rect m_rect;
	SDL_Rect m_startRect;
	SDL_Rect m_source;

	// Box2D stuff
	b2BodyDef m_bodyDef;
	b2Body* m_body;
	b2PolygonShape m_shape;
	b2FixtureDef m_bodyFixtureDef;

	// Idle sprite
	Sprite* m_playerIdleSprite;
	Sprite* m_playerIdleMachineGunSprite;
	bool m_idle;
	bool m_drawn;

	// Running animation
	Sprite* m_playerRunningSprite;
	const int RUNNING_ANIMATION_FRAMES = 10;
	SDL_Rect gSpriteRunningClipsRight[10];
	SDL_Rect gSpriteRunningClipsLeft[10];
	SDL_Rect* currentRunnerClip;
	int m_runningFrames;
	int m_runningAnimationTime;
	int m_runningAnimationLimit;
	int m_runningAnimationLimitLab;
	int m_runningAnimationLimitLaptop;
	bool m_running;

	// Machine gun running animation
	Sprite* m_playerRunningMGSprite;
	const int RUNNING_MG_ANIMATION_FRAMES = 5;
	SDL_Rect gSpriteRunningMGClipsRight[5];
	SDL_Rect gSpriteRunningMGClipsLeft[5];
	SDL_Rect* currentRunnerMGClip;
	int m_runningMGFrames;
	int m_runningMGAnimationTime;
	int m_runningMGAnimationLimit;
	int m_runningMGAnimationLimitLab;
	int m_runningMGAnimationLimitLaptop;
	int m_machineGunAmmo;

	// Blood splatter
	Sprite* m_bloodSprite;
	const int BLOOD_ANIMATION_FRAMES = 6;
	SDL_Rect bloodSpriteClips[10];
	SDL_Rect* currentBloodClip;
	int m_bloodFrames;
	int m_bloodAnimationTime;
	int m_bloodAnimationLimit;
	int m_bloodAnimationLimitLab;
	int m_bloodAnimationLimitLaptop;
	bool m_stopBloodAnimation;

	// For game
	bool m_reachedTeleporter;

	// Jump
	bool m_canJump;

	// Score
	int m_score;

	// Health
	int m_health;
	int m_lives;
	bool m_alive;

	// Machine gun
	bool m_hasMachineGun;
	bool m_machineGunEquipped;
	bool oldPickedUp;
	int m_shootTimerMGLaptop;
	int m_shootTimerMGLab;
	int m_shootTimerMGLimit;

	// Platform currently on
	std::vector<int> platformOn;

	// Bullets
	list<Bullet*> m_bullets;
	int m_timeToShoot;
	int m_shootTimerLaptop;
	int m_shootTimerLab;
	int m_shootTimerLimit;
	std::list<Bullet*>::iterator m_bulletIterator;

	// Box2D
	b2World* world;

	// Scale
	float playerScale;

	// Collision with platform
	bool collidingWithMovingPlat;
};

#endif