#include "stdafx.h"
#include "PlatformManager.h"

bool PlatformManager::instanceFlag = false;
PlatformManager* PlatformManager::instance = NULL;

PlatformManager* PlatformManager::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new PlatformManager;
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}

void PlatformManager::addPlatform(SDL_Rect pRect, b2World* world, string type, int upsideDown)
{
	if (type == "topPlatform")
	{
		if (upsideDown == 1)
		{
			m_platformTexture = Sprite::loadTexture("Images/LevelEntities/platform.png", Renderer::GetInstance()->Get_SDL_RENDERER());
			m_platformSource = { 0, 0, 106, 29 };
		}
		if (upsideDown == 2)
		{
			m_platformTexture = Sprite::loadTexture("Images/LevelEntities/platform2.png", Renderer::GetInstance()->Get_SDL_RENDERER());
			m_platformSource = { 0, 0, 106, 29 };
		}
	}
	else
	{
		m_platformTexture = Sprite::loadTexture("Images/LevelEntities/bottomPlatform.png", Renderer::GetInstance()->Get_SDL_RENDERER());
		m_platformSource = { 0, 0, 106, 29 };
	}

	Platform* temp = new Platform(m_platformTexture, pRect, world, m_platformSource);

	m_platforms.push_back(temp);
}

void PlatformManager::addNoRayPlatform(SDL_Rect pRect, b2World* world, string type, int upsideDown)
{
	if (type == "topPlatform")
	{
		if (upsideDown == 1)
		{
			m_platformTexture = Sprite::loadTexture("Images/LevelEntities/platform.png", Renderer::GetInstance()->Get_SDL_RENDERER());
			m_platformSource = { 0, 0, 106, 29 };
		}
		if (upsideDown == 2)
		{
			m_platformTexture = Sprite::loadTexture("Images/LevelEntities/platform2.png", Renderer::GetInstance()->Get_SDL_RENDERER());
			m_platformSource = { 0, 0, 106, 29 };
		}
	}
	else
	{
		m_platformTexture = Sprite::loadTexture("Images/LevelEntities/bottomPlatform.png", Renderer::GetInstance()->Get_SDL_RENDERER());
		m_platformSource = { 0, 0, 106, 29 };
	}

	NoRayPlatform* temp = new NoRayPlatform(m_platformTexture, pRect, world, m_platformSource);

	m_noRayPlatforms.push_back(temp);
}

void PlatformManager::addMovingPlatform(SDL_Rect pRect, b2World* world, string type, bool pLeftRight)
{
	m_movingPlatformTexture = Sprite::loadTexture("Images/LevelEntities/platform.png", Renderer::GetInstance()->Get_SDL_RENDERER());
	m_movingPlatformSource = { 0, 0, 106, 29 };

	MovingPlatform* temp = new MovingPlatform(m_movingPlatformTexture, pRect, world, m_movingPlatformSource, type, pLeftRight);

	m_movingPlatforms.push_back(temp);
}

void PlatformManager::addMovePlatSensor(SDL_Rect pRect, b2World  *world, int src)
{
	MovingPlatform* temp = new MovingPlatform(pRect, world, src);

	movingPlatformsSensors.push_back(temp);
}

void PlatformManager::Draw()
{
	for each(Platform* plat in m_platforms)
	{
		plat->Draw();
	}
	for each(NoRayPlatform* plat in m_noRayPlatforms)
	{
		plat->Draw();
	}
	for each(MovingPlatform* mPlat in m_movingPlatforms)
	{
		mPlat->Draw();
	}
}

void PlatformManager::Update()
{
	for each(MovingPlatform* mPlat in m_movingPlatforms)
	{
		mPlat->Update();
	}
}

std::vector<int>PlatformManager::CheckStaticPlatCollision(b2Body*playerBody)
{
	for each (Platform* plat in m_platforms)
	{
		if (plat->CheckStaticPlatCollision(playerBody))
		{
			int x = plat->GetRect()->x;
			int y = plat->GetRect()->y;
			int w = plat->GetRect()->w;
			int h = plat->GetRect()->h;
			platformOn.push_back(x);
			platformOn.push_back(y);
			platformOn.push_back(w);
			platformOn.push_back(h);
			return platformOn;
		}
	}
	platformOn.clear();
	return vector<int>();;
}

bool PlatformManager::CheckCollision(b2Body*playerBody)
{
	for each (MovingPlatform* mPlat in m_movingPlatforms)
	{
		if (mPlat->CheckCollision(playerBody) && mPlat->GetType() == "platform")
		{
			xSpeedOfMovingPlatform = mPlat->GetBody()->GetLinearVelocity().x;
			ySpeedOfMovingPlatform = mPlat->GetBody()->GetLinearVelocity().y;
			return true;
		}
	}
	return false;
}

bool PlatformManager::IsLeftRight()
{
	for each (MovingPlatform* mPlat in m_movingPlatforms)
	{
		return mPlat->IsLeftRight();
	}
}

b2Vec2 *PlatformManager::getRayIntersectionPoint(b2RayCastOutput output, b2RayCastInput input, bool playerAbove, bool playerToLeft, float gruntXPos, float gruntYPos)
{
	float closestFraction = 1;
	b2Vec2 intersectionNormal(0, 0);
	for each(Platform* plat in m_platforms)
	{
		// Only check platforms ABOVE_LEFT
		if (playerAbove && playerToLeft)
		{
			if (plat->getBody()->GetPosition().y <= gruntYPos && plat->getBody()->GetPosition().x <= gruntXPos)
			{
				if (plat->getBody()->GetPosition().x )
				for (b2Fixture* f = plat->getBody()->GetFixtureList(); f; f = f->GetNext())
				{
					if (!f->RayCast(&output, input, 0))
					{
						continue;
					}
					if (output.fraction < closestFraction)
					{
						closestFraction = output.fraction;
						intersectionNormal = output.normal;
					}
				}
			}
		}
		// Only check platforms ABOVE_RIGHT
		else if (playerAbove && !playerToLeft)
		{
			if (plat->getBody()->GetPosition().y <= gruntYPos && plat->getBody()->GetPosition().x >= gruntXPos)
			{
				//return plat->getBody()->GetFixtureList();
				for (b2Fixture* f = plat->getBody()->GetFixtureList(); f; f = f->GetNext())
				{
					if (!f->RayCast(&output, input, 0))
					{
						continue;
					}
					if (output.fraction < closestFraction)
					{
						closestFraction = output.fraction;
						intersectionNormal = output.normal;
					}
				}
			}
		}
		// Only check platforms BELOW_LEFT
		else if (!playerAbove && playerToLeft)
		{
			if (plat->getBody()->GetPosition().y >= gruntYPos && plat->getBody()->GetPosition().x-96 <= gruntXPos)
			{
				//return plat->getBody()->GetFixtureList();
				for (b2Fixture* f = plat->getBody()->GetFixtureList(); f; f = f->GetNext())
				{
					if (!f->RayCast(&output, input, 0))
					{
						continue;
					}
					if (output.fraction < closestFraction)
					{
						closestFraction = output.fraction;
						intersectionNormal = output.normal;
					}
				}
			}
		}
		// Only check platforms BELOW_RIGHT
		else if (!playerAbove && !playerToLeft)
		{
			if (plat->getBody()->GetPosition().y >= gruntYPos && plat->getBody()->GetPosition().x >= gruntXPos)
			{
				//return plat->getBody()->GetFixtureList();
				for (b2Fixture* f = plat->getBody()->GetFixtureList(); f; f = f->GetNext())
				{
					if (!f->RayCast(&output, input, 0))
					{
						continue;
					}
					if (output.fraction < closestFraction)
					{
						closestFraction = output.fraction;
						intersectionNormal = output.normal;
					}
				}
			}
		}
	}// End outer for loop

	b2Vec2 intersectionPoint = input.p1 + closestFraction * (input.p2 - input.p1);
	return &intersectionPoint;
}

void PlatformManager::Destroy()
{
	for each(Platform* plat in m_platforms)
	{
		plat->Destroy();
	}
	for each(NoRayPlatform* plat in m_noRayPlatforms)
	{
		plat->Destroy();
	}
	for each(MovingPlatform* mPlat in m_movingPlatforms)
	{
		mPlat->Destroy();
	}
	for each(MovingPlatform* mPlat in movingPlatformsSensors)
	{
		mPlat->Destroy();
	}

	if (m_platforms.size() > 0)
	{
		m_platforms.clear();
	}
	if (m_noRayPlatforms.size() > 0)
	{
		m_noRayPlatforms.clear();
	}
	if (m_movingPlatforms.size() > 0)
	{
		m_movingPlatforms.clear();
	}
	if (movingPlatformsSensors.size() > 0)
	{
		movingPlatformsSensors.clear();
	}
}