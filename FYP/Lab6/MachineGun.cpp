#include "stdafx.h"
#include "MachineGun.h"

MachineGun::MachineGun()
{
	
}

MachineGun::MachineGun(SDL_Texture* text, SDL_Rect pRect, b2World *wWorld, SDL_Rect src)
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
	m_sprite = new Sprite();
	m_sprite->Init(text, pRect, src);
	m_sprite->SetOffset(SDL_Point{ pRect.w / 2, pRect.h / 2 });

	m_pickedUp = false;
}

void MachineGun::Draw()
{
	if (!m_pickedUp)
	{
		m_sprite->Draw(1);
	}
}

void MachineGun::Reset()
{
	m_pickedUp = false;
}

void MachineGun::Destroy()
{
	//if(myBody->GetNext() != NULL)
	myBody->GetWorld()->DestroyBody(myBody);
}

bool MachineGun::CheckCollision(b2Body* playerBody)
{
	bool collided = (b2TestOverlap(myBody->GetFixtureList()->GetAABB(0), playerBody->GetFixtureList()->GetAABB(0)));
	if (collided)
	{
		m_pickedUp = true;
		if (SoundManager::GetInstance()->SoundOn())
			SoundManager::GetInstance()->play(SoundManager::GUN_PICKUP);
	}

	return collided;
}

bool MachineGun::PickedUp()
{
	return m_pickedUp;
}

void MachineGun::SetPickedUp(bool myPickedUp)
{
	m_pickedUp = myPickedUp;
}