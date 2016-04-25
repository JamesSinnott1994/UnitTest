#include "stdafx.h"
#include "Ammo.h"

Ammo::Ammo(){}

Ammo::Ammo(SDL_Texture* text, SDL_Rect pRect, b2World *wWorld, SDL_Rect src)
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
	sprite.SetOffset(SDL_Point{ pRect.w / 2, pRect.h / 2 });

	m_alive = true;
	m_generated = false;
}

Ammo::Ammo(SDL_Texture* text, SDL_Rect pRect, b2World *wWorld, SDL_Rect src, int type)
{
	myRect = pRect;
	myRect.w = 32;
	myRect.h = 32;

	// Define the ground body.
	myBodyDef.position.Set(myRect.x, myRect.y);
	myBodyDef.type = b2_dynamicBody;

	// Define the ground box shape.
	// The extents are the half-widths of the box.
	myShape.SetAsBox(myRect.w / 2, myRect.h / 2);

	myBodyFixtureDef.shape = &myShape;

	// The body is also added to the world.
	myBody = wWorld->CreateBody(&myBodyDef);

	// Collision Filtering
	short GROUP_PLAYER = -2;

	myBodyFixtureDef.filter.groupIndex = GROUP_PLAYER;

	// Add the ground fixture to the ground body.
	myBody->CreateFixture(&myBodyFixtureDef);
	sprite.Init(text, myRect, src);
	sprite.SetOffset(SDL_Point{ myRect.w / 2, myRect.h / 2 });

	m_alive = true;
	m_generated = true;

	myBody->SetLinearVelocity(b2Vec2(0, myBody->GetLinearVelocity().y - 0.000001f));
}

void Ammo::Draw()
{
	if (m_alive)
	{
		sprite.SetPosition(myBody->GetPosition().x, myBody->GetPosition().y);
		sprite.Draw(1);
	}
}

void Ammo::Reset()
{
	m_alive = true;
	myBody->SetActive(true);
}

void Ammo::Destroy()
{
	myBody->GetWorld()->DestroyBody(myBody);
}

bool Ammo::CheckCollision(b2Body* playerBody)
{
	bool collided = (b2TestOverlap(myBody->GetFixtureList()->GetAABB(0), playerBody->GetFixtureList()->GetAABB(0)));
	if (collided)
	{
		m_alive = false;
		myBody->SetActive(false);
		if (SoundManager::GetInstance()->SoundOn())
			SoundManager::GetInstance()->play(SoundManager::GUN_PICKUP);
	}

	return collided;
}

bool Ammo::GetAlive()
{
	return m_alive;
}

void Ammo::SetAlive(bool myAlive)
{
	m_alive = myAlive;
}