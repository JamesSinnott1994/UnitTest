#include "stdafx.h"
#include "PickupManager.h"

bool PickupManager::instanceFlag = false;
PickupManager* PickupManager::instance = NULL;

PickupManager* PickupManager::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new PickupManager;
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}

void PickupManager::Draw()
{
	for each(Score* score in m_scores)
	{
		score->Draw();
	}

	for each(Health* health in m_healthPickups)
	{
		health->Draw();
	}
	for each(MachineGun* mg in m_machineGunPickups)
	{
		mg->Draw();
	}
	for each(Ammo* ammo in m_ammoPickups)
	{
		ammo->Draw();
	}
	for each(Ammo* ammo in m_ammoGeneratedPickups)
	{
		ammo->Draw();
	}
}

void PickupManager::Update()
{
	for each(Score* score in m_scores)
	{
		score->Update();
	}
}

void PickupManager::Reset()
{
	for each(Score* score in m_scores)
	{
		score->Reset();
	}
	for each(Health* health in m_healthPickups)
	{
		health->Reset();
	}
	for each(MachineGun* mg in m_machineGunPickups)
	{
		mg->Reset();
	}
	for each(Ammo* ammo in m_ammoPickups)
	{
		ammo->Reset();
	}
	for each(Ammo* ammo in m_ammoGeneratedPickups)
	{
		ammo->Destroy();
	}
	if (m_ammoGeneratedPickups.size() > 0)
	{
		m_ammoGeneratedPickups.clear();
	}
}

void PickupManager::Destroy()
{
	for each(Score* score in m_scores)
	{
		score->Destroy();
	}
	for each(Health* health in m_healthPickups)
	{
		health->Destroy();
	}
	for each(MachineGun* mg in m_machineGunPickups)
	{
		mg->Destroy();
	}
	for each(Ammo* ammo in m_ammoPickups)
	{
		ammo->Destroy();
	}
	for each(Ammo* ammo in m_ammoGeneratedPickups)
	{
		ammo->Destroy();
	}

	if (m_scores.size() > 0)
	{
		m_scores.clear();
	}
	if (m_healthPickups.size() > 0)
	{
		m_healthPickups.clear();
	}
	if (m_machineGunPickups.size() > 0)
	{
		m_machineGunPickups.clear();
	}
	if (m_ammoPickups.size() > 0)
	{
		m_ammoPickups.clear();
	}
	if (m_ammoGeneratedPickups.size() > 0)
	{
		m_ammoGeneratedPickups.clear();
	}
}

void PickupManager::addScorePickups(SDL_Rect pRect, b2World* world, string speedType)
{
	Score* temp = new Score(m_scoreTexture, pRect, world, m_scoreSource, speedType);

	m_scores.push_back(temp);
}

void PickupManager::addHealthPickups(SDL_Rect pRect, b2World* world)
{
	Health* temp = new Health(m_healthTexture, pRect, world, m_healthSource);

	m_healthPickups.push_back(temp);
}

void PickupManager::addMachineGun(SDL_Rect pRect, b2World* world)
{
	MachineGun* temp = new MachineGun(m_machineGunTexture, pRect, world, m_machineGunSource);

	m_machineGunPickups.push_back(temp);
}

void PickupManager::addAmmoPickups(SDL_Rect pRect, b2World* world)
{
	Ammo* temp = new Ammo(m_ammoTexture, pRect, world, m_ammoSource);

	m_ammoPickups.push_back(temp);
}

bool PickupManager::CheckScoreCollision(b2Body*playerBody)
{
	// Iterate through list of bullets
	for (m_scoreIterator = m_scores.begin(); m_scoreIterator != m_scores.end(); ++m_scoreIterator)
	{
		if ((*m_scoreIterator)->GetAlive())
		{
			if ((*m_scoreIterator)->CheckCollision(playerBody))
			{
				return true;
			}
		}
	}
	return false;
}

bool PickupManager::CheckHealthCollision(b2Body*playerBody)
{
	// Iterate through list of bullets
	for (m_healthIterator = m_healthPickups.begin(); m_healthIterator != m_healthPickups.end(); ++m_healthIterator)
	{
		if ((*m_healthIterator)->GetAlive())
		{
			if ((*m_healthIterator)->CheckCollision(playerBody))
			{
				return true;
			}
		}
	}
	return false;
}

bool PickupManager::CheckMachineGunCollision(b2Body*playerBody)
{
	for (m_machineGunIterator = m_machineGunPickups.begin(); m_machineGunIterator != m_machineGunPickups.end(); ++m_machineGunIterator)
	{
		if (!(*m_machineGunIterator)->PickedUp())
		{
			if ((*m_machineGunIterator)->CheckCollision(playerBody))
			{
				return true;
			}
		}
	}
	return false;
}

bool PickupManager::CheckAmmoCollision(b2Body*playerBody)
{
	// Iterate through list of GENERATED ammo pickups
	if (m_ammoGeneratedPickups.size() > 0)
	{
		for (m_ammoIterator = m_ammoGeneratedPickups.begin(); m_ammoIterator != m_ammoGeneratedPickups.end(); ++m_ammoIterator)
		{
			if ((*m_ammoIterator)->GetAlive())
			{
				if ((*m_ammoIterator)->CheckCollision(playerBody))
				{
					(*m_ammoIterator)->Destroy();
					m_ammoGeneratedPickups.erase(m_ammoIterator);
					return true;
				}
			}
		}
	}
	// Iterate through list of ammo pickups
	if (m_ammoPickups.size() > 0)
	{
		for (m_ammoIterator = m_ammoPickups.begin(); m_ammoIterator != m_ammoPickups.end(); ++m_ammoIterator)
		{
			if ((*m_ammoIterator)->GetAlive())
			{
				if ((*m_ammoIterator)->CheckCollision(playerBody))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void PickupManager::GenerateAmmo(SDL_Rect pRect, b2World* world)
{
	Ammo* temp = new Ammo(m_ammoTexture, pRect, world, m_ammoSource, 2);

	m_ammoGeneratedPickups.push_back(temp);
}
