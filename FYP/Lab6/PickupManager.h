#ifndef PICKUPMANAGER_H
#define PICKUPMANAGER_H

#include <list>
#include "Score.h"
#include "Health.h"
#include "MachineGun.h"
#include "Ammo.h"

class PickupManager
{
public:
	static PickupManager* GetInstance();
	~PickupManager()
	{
		instanceFlag = false;
	}

	void Draw();
	void Update();
	void Reset();
	void Destroy();

	void addScorePickups(SDL_Rect pRect, b2World* world, string speedType);
	void addHealthPickups(SDL_Rect pRect, b2World* world);
	void addMachineGun(SDL_Rect pRect, b2World* world);
	void addAmmoPickups(SDL_Rect pRect, b2World* world);

	bool CheckScoreCollision(b2Body*playerBody);
	bool CheckHealthCollision(b2Body*playerBody);
	bool CheckMachineGunCollision(b2Body*playerBody);
	bool CheckAmmoCollision(b2Body*playerBody);

	void GenerateAmmo(SDL_Rect pRect, b2World* world);

private:
	PickupManager()
	{
		m_scores = list<Score*>();
		m_scoreTexture = Sprite::loadTexture("Images/Pickups/CoinAnimations.png", Renderer::GetInstance()->Get_SDL_RENDERER());
		m_scoreSource = { 0, 0, 412, 64 };

		m_healthPickups = list<Health*>();
		m_healthTexture = Sprite::loadTexture("Images/Pickups/health.png", Renderer::GetInstance()->Get_SDL_RENDERER());
		m_healthSource = { 0, 0, 40, 40 };

		// Machine Gun
		m_machineGunPickups = list<MachineGun*>();
		m_machineGunTexture = Sprite::loadTexture("Images/Pickups/MachineGun.png", Renderer::GetInstance()->Get_SDL_RENDERER());
		m_machineGunSource = { 0, 0, 59, 24 };

		// Ammo
		m_ammoPickups = list<Ammo*>();
		m_ammoGeneratedPickups = list<Ammo*>();
		m_ammoTexture = Sprite::loadTexture("Images/Pickups/Ammo.png", Renderer::GetInstance()->Get_SDL_RENDERER());
		m_ammoSource = { 0, 0, 27, 27 };
	}

	static bool instanceFlag;
	static PickupManager* instance;

	list<Score*> m_scores;
	SDL_Texture* m_scoreTexture;
	SDL_Rect m_scoreSource;
	list<Score*>::iterator m_scoreIterator;

	list<Health*> m_healthPickups;
	SDL_Texture* m_healthTexture;
	SDL_Rect m_healthSource;
	list<Health*>::iterator m_healthIterator;

	// Machine Gun
	list<MachineGun*> m_machineGunPickups;
	SDL_Texture* m_machineGunTexture;
	SDL_Rect m_machineGunSource;
	list<MachineGun*>::iterator m_machineGunIterator;

	// Ammo
	list<Ammo*> m_ammoPickups;
	list<Ammo*> m_ammoGeneratedPickups;
	SDL_Texture* m_ammoTexture;
	SDL_Rect m_ammoSource;
	list<Ammo*>::iterator m_ammoIterator;
};

#endif

