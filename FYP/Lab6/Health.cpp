#include "stdafx.h"
#include "Health.h"

Health::Health(){}

Health::Health(SDL_Texture* text, SDL_Rect pRect, b2World *wWorld, SDL_Rect src)
{
	myRect = pRect;

	// Define the ground body.
	myBodyDef.position.Set(pRect.x, pRect.y);
	myBodyDef.type = b2_dynamicBody;

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
	sprite = new Sprite();
	sprite->Init(text, pRect, src);
	sprite->SetOffset(SDL_Point{ pRect.w / 2, pRect.h / 2 });

	m_alive = true;
}

void Health::Draw()
{
	if (m_alive)
	{
		sprite->SetPosition(myBody->GetPosition().x, myBody->GetPosition().y);
		sprite->Draw(1);
	}
}

void Health::Reset()
{
	m_alive = true;
	myBody->SetActive(true);
}

void Health::Destroy()
{
	myBody->GetWorld()->DestroyBody(myBody);
}

bool Health::CheckCollision(b2Body* playerBody)
{
	bool collided = (b2TestOverlap(myBody->GetFixtureList()->GetAABB(0), playerBody->GetFixtureList()->GetAABB(0)));
	if (collided)
	{
		m_alive = false;
		myBody->SetActive(false);
		if (SoundManager::GetInstance()->SoundOn())
			SoundManager::GetInstance()->play(SoundManager::HEALTH);
	}

	return collided;
}

bool Health::GetAlive()
{
	return m_alive;
}

void Health::SetAlive(bool myAlive)
{
	m_alive = myAlive;
}