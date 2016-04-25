#include "stdafx.h"
#include "ObstacleManager.h"

bool ObstacleManager::instanceFlag = false;
ObstacleManager* ObstacleManager::instance = NULL;

ObstacleManager* ObstacleManager::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new ObstacleManager;
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}

void ObstacleManager::Draw()
{
	for each(Mine* mine in m_mines)
	{
		mine->Draw();
	}
	for each(ElectricBarrier* barrier in m_barriers)
	{
		barrier->Draw();
	}
	for each(ElectricSwitch* m_switch in m_switches)
	{
		m_switch->Draw();
	}
}

void ObstacleManager::Update()
{
	for each(Mine* mine in m_mines)
	{
		mine->Update();
	}
	for each(ElectricBarrier* barrier in m_barriers)
	{
		if (barrier->GetOn())
		{
			barrier->Update();
		}
	}
	for each(ElectricSwitch* m_switch in m_switches)
	{
		if (!m_switch->GetOn())// If switch is turned off
		{
			// Turn off barrier
			for each(ElectricBarrier* barrier in m_barriers)
			{
				if (barrier->GetBarrierID() == m_switch->GetSwitchID() && barrier->GetOn())
				{
					barrier->SetOn(false);
				}
			}
		}
	}
}

void ObstacleManager::Reset()
{
	for each(Mine* mine in m_mines)
	{
		mine->Reset();
	}
	for each(ElectricBarrier* barrier in m_barriers)
	{
		barrier->Reset();
	}
	for each(ElectricSwitch* m_switch in m_switches)
	{
		m_switch->Reset();
	}
}

void ObstacleManager::Destroy()
{
	for each(Mine* mine in m_mines)
	{
		mine->Destroy();
	}
	for each(ElectricBarrier* barrier in m_barriers)
	{
		barrier->Destroy();
	}
	for each(ElectricSwitch* m_switch in m_switches)
	{
		m_switch->Destroy();
	}

	// Iterate through list of mines
	if (m_mines.size() > 0)
	{
		m_mines.clear();
	}
	if (m_barriers.size() > 0)
	{
		m_barriers.clear();
	}
	if (m_switches.size() > 0)
	{
		m_switches.clear();
	}
}

void ObstacleManager::addMineObstacles(SDL_Rect pRect, b2World* world)
{
	Mine* temp = new Mine(m_mineTexture, pRect, world, m_mineSource);

	m_mines.push_back(temp);
}

void ObstacleManager::addElectricBarriers(SDL_Rect pRect, b2World* world, string speedType, int barrierID)
{
	ElectricBarrier* barrier = new ElectricBarrier(m_barrierTexture, pRect, world, m_barrierSource, speedType, barrierID);

	m_barriers.push_back(barrier);
}

void ObstacleManager::addElectricSwitches(SDL_Rect pRect, b2World* world, int switchID)
{
	ElectricSwitch* temp = new ElectricSwitch(m_switchTexture, pRect, world, m_switchSource, switchID);

	m_switches.push_back(temp);
}

bool ObstacleManager::CheckMineCollision(b2Body*playerBody)
{
	// Iterate through list of bullets
	for (m_mineIterator = m_mines.begin(); m_mineIterator != m_mines.end(); ++m_mineIterator)
	{
		if ((*m_mineIterator)->GetAlive())
		{
			if ((*m_mineIterator)->CheckCollision(playerBody))
			{
				return true;
			}
		}
	}
	return false;
}

bool ObstacleManager::CheckBarrierCollision(b2Body*playerBody)
{
	// Iterate through list of bullets
	for (m_barrierIterator = m_barriers.begin(); m_barrierIterator != m_barriers.end(); ++m_barrierIterator)
	{
		if ((*m_barrierIterator)->GetOn())
		{
			if ((*m_barrierIterator)->CheckCollision(playerBody))
			{
				return true;
			}
		}
	}
	return false;
}

bool ObstacleManager::CheckSwitchCollision(b2Body*playerBody)
{
	// Iterate through list of bullets
	for (m_switchIterator = m_switches.begin(); m_switchIterator != m_switches.end(); ++m_switchIterator)
	{
		if ((*m_switchIterator)->GetOn())
		{
			if ((*m_switchIterator)->CheckCollision(playerBody))
			{
				return true;
			}
		}
	}
	return false;
}