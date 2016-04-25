#ifndef WALLMANAGER_H
#define WALLMANAGER_H

#include "Wall.h"
#include <list>

class WallManager
{
public:
	static WallManager* GetInstance();
	~WallManager()
	{
		instanceFlag = false;
	}

	void Destroy();

	void addWallParts(SDL_Rect pRect, b2World* world);

private:
	WallManager()
	{
		m_walls = list<Wall*>();
	}

	static bool instanceFlag;
	static WallManager* instance;

	list<Wall*> m_walls;
	list<Wall*>::iterator m_wallIterator;
};

#endif