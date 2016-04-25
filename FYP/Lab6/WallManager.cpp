#include "stdafx.h"
#include "WallManager.h"

bool WallManager::instanceFlag = false;
WallManager* WallManager::instance = NULL;

WallManager* WallManager::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new WallManager;
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}

void WallManager::Destroy()
{
	for each(Wall* wall in m_walls)
	{
		wall->Destroy();
	}

	// Iterate through list of bullets
	if (m_walls.size() > 0)
	{
		m_walls.clear();
	}
}

void WallManager::addWallParts(SDL_Rect pRect, b2World* world)
{
	Wall* temp = new Wall(pRect, world);

	m_walls.push_back(temp);
}