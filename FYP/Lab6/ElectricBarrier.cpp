#include "stdafx.h"
#include "ElectricBarrier.h"

ElectricBarrier::ElectricBarrier()
{
}

ElectricBarrier::ElectricBarrier(SDL_Texture* text, SDL_Rect pRect, b2World *wWorld, SDL_Rect src, string speedType, int id)
{
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
	short GROUP_PLAYER = -1;
	short GROUP_BARRIER = -2;

	myBodyFixtureDef.filter.groupIndex = GROUP_BARRIER;


	// Add the ground fixture to the ground body.
	myBody->CreateFixture(&myBodyFixtureDef);

	m_on = true;

	m_animationSprite = new Sprite();
	m_animationSprite->Init(text, pRect, src);
	m_animationSprite->SetOffset(SDL_Point{ pRect.w / 2, pRect.h / 2 });
	gSpriteClips[ANIMATION_FRAMES];

	// Speed
	m_animationTimeLaptop = 10;
	m_animationTimeLab = 60;

	if (speedType == "labSpeed")
	{
		m_limit = m_animationTimeLab;
	}
	else
	{
		m_limit = m_animationTimeLaptop;
	}

	SpriteClips();

	barrierID = id;
	switchID = 0;
	hasSwitchID = false;
}

void ElectricBarrier::SpriteClips()
{
	gSpriteClips[0] = { 43, 4, 21, 193 };
	gSpriteClips[1] = { 199, 4, 31, 193 };
	gSpriteClips[2] = { 357, 4, 28, 193 };
}

void ElectricBarrier::Draw()
{
	if (m_on)
	{
		currentClip = &gSpriteClips[m_animationFrames / ANIMATION_FRAMES];
		m_animationSprite->SetSourceRect(*currentClip);
		m_animationSprite->Draw(1);
	}
}

void ElectricBarrier::Update()
{
	// Cycle animation
	if (m_animationFrames / ANIMATION_FRAMES >= ANIMATION_FRAMES)
	{
		m_animationFrames = 0;
	}

	// Go through frames
	m_animationTime++;
	if (m_animationTime > m_limit)
	{
		++m_animationFrames;
		m_animationTime = 0;
	}

	m_animationSprite->SetPosition(myBody->GetPosition().x, myBody->GetPosition().y);
}

void ElectricBarrier::Reset()
{
	m_on = true;
	myBody->SetActive(true);
}

void ElectricBarrier::Destroy()
{
	myBody->GetWorld()->DestroyBody(myBody);
}

bool ElectricBarrier::CheckCollision(b2Body* playerBody)
{
	bool collided = (b2TestOverlap(myBody->GetFixtureList()->GetAABB(0), playerBody->GetFixtureList()->GetAABB(0)));
	if (collided)
	{
		if (SoundManager::GetInstance()->SoundOn())
			SoundManager::GetInstance()->play(SoundManager::ELECTROCUTED);
	}
	return collided;
}

bool ElectricBarrier::GetOn()
{
	return m_on;
}

void ElectricBarrier::SetOn(bool myAlive)
{
	m_on = myAlive;
}

int ElectricBarrier::GetSwitchID()
{
	return switchID;
}

void ElectricBarrier::SetSwitchID(int myID)
{
	switchID = myID;
}

int ElectricBarrier::GetBarrierID()
{
	return barrierID;
}

void ElectricBarrier::SetBarrierID(int myID)
{
	barrierID = myID;
}