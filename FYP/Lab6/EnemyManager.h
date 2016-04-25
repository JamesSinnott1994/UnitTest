#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include <iostream> 
#include <SDL.h>			//SDL
#include <stdio.h>
#include "SDL_mixer.h"
#include <list>
#include "Grunt.h"
#include "Robot.h"

using namespace std;

class EnemyManager
{
public:
	static EnemyManager* GetInstance();
	~EnemyManager()
	{
		instanceFlag = false;
	}

	void Draw();
	bool Update(SDL_Rect &playerRect, b2Body* playerBody);
	void UpdateGrunts(SDL_Rect &playerRect, b2Body* playerBody);

	bool UpdateRobotBullets(SDL_Rect &playerRect, b2Body* playerBody);

	bool GruntCheckCollision(b2Body* playerBody);
	bool CheckBulletGruntCollision(b2Body* bulletBody, bool hasMachineGun);
	bool CheckBulletRobotCollision(b2Body* bulletBody);

	void Reset();
	void ResetGruntTimers();
	void Destroy();
	void DestroyBullets();

	void addGrunt(SDL_Rect pRect, b2World* world, int color, int direction, string speedType, int width, int height);
	void addRobot(SDL_Rect pRect, b2World* world, int direction, string speedType, int width, int height);

	bool IncreasePlayerScore();
	void SetIncreasePlayerScore(bool myIncreasePlayerScore);

	int GetPlayerScoreToAdd();
	void SetPlayerScoreToAdd(int myPlayerScoreToAdd);

private:
	EnemyManager()
	{
		m_grunts = list<Grunt*>();
		m_robots = list<Robot*>();

		// Player score
		m_increasePlayerScore = false;
		m_playerScoreToAdd = 0;
	}

	static bool instanceFlag;
	static EnemyManager* instance;

	// Grunt
	list<Grunt*> m_grunts;
	list<GruntBullet*>m_gruntBullets;
	SDL_Texture* m_gruntTexture;
	SDL_Rect m_gruntSource;
	list<Grunt*>::iterator m_gruntIterator;
	list<GruntBullet*>::iterator m_gruntBulletIterator;

	// Robot
	list<Robot*> m_robots;
	list<RobotBullet*>m_robotBullets;
	SDL_Texture* m_robotTexture;
	SDL_Rect m_robotSource;
	list<Robot*>::iterator m_robotIterator;
	list<RobotBullet*>::iterator m_robotBulletIterator;

	// Player score
	bool m_increasePlayerScore;
	int m_playerScoreToAdd;
};
#endif