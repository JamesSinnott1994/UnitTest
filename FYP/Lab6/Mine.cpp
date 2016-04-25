#include "stdafx.h"
#include "Mine.h"

Mine::Mine()
{
}

Mine::Mine(SDL_Texture* text, SDL_Rect pRect, b2World *wWorld, SDL_Rect src)
{
	myRect = pRect;

	// Define the ground body.
	myBodyDef.position.Set(pRect.x, pRect.y);
	myBodyDef.type = b2_staticBody;

	// Define the ground box shape.
	// The extents are the half-widths of the box.
	myShape.SetAsBox(pRect.w / 2, pRect.h / 2);

	myBodyFixtureDef.shape = &myShape;

	// The body is also added to the world.
	myBody = wWorld->CreateBody(&myBodyDef);

	// Collision Filtering
	short GROUP_PLAYER = -2;

	myBodyFixtureDef.filter.groupIndex = GROUP_PLAYER;

	// Add the ground fixture to the ground body.
	myBody->CreateFixture(&myBodyFixtureDef);
	sprite.Init(text, pRect, src);
	sprite.SetOffset(SDL_Point{ myRect.w / 2, myRect.h / 2 });

	m_alive = true;
	m_animationPlayed = false;
	m_startAnimation = false;
}

void Mine::Draw()
{
	if (m_alive)
		sprite.Draw(1);
}

void Mine::Update()
{
	if (m_startAnimation && !m_animationPlayed)
	{
		m_animationPlayed = true;
	}
}

void Mine::Reset()
{
	m_alive = true;
	myBody->SetActive(true);
}

void Mine::Destroy()
{
	myBody->GetWorld()->DestroyBody(myBody);
}

bool Mine::CheckCollision(b2Body* playerBody)
{
	bool collided = (b2TestOverlap(myBody->GetFixtureList()->GetAABB(0), playerBody->GetFixtureList()->GetAABB(0)));
	if (collided)
	{
		m_alive = false;
		myBody->SetActive(false);
		if (SoundManager::GetInstance()->SoundOn())
			SoundManager::GetInstance()->play(SoundManager::MINE);
		m_startAnimation = true;
	}

	return collided;
}

bool Mine::GetAlive()
{
	return m_alive;
}

void Mine::SetAlive(bool myAlive)
{
	m_alive = myAlive;
}