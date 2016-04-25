#include "stdafx.h"
#include "Score.h"

Score::Score()
{
}

Score::Score(SDL_Texture* text, SDL_Rect pRect, b2World *wWorld, SDL_Rect src, string speedType)
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
	short GROUP_PLAYER = -2;

	myBodyFixtureDef.filter.groupIndex = GROUP_PLAYER;

	// Add the ground fixture to the ground body.
	myBody->CreateFixture(&myBodyFixtureDef);

	m_alive = true;

	m_animationSprite = new Sprite();
	m_animationSprite->Init(text, pRect, src);
	m_animationSprite->SetOffset(SDL_Point{ pRect.w / 2, pRect.h / 2 });
	gSpriteClips[ANIMATION_FRAMES];

	// Speed
	m_animationTimeLaptop = 10;
	m_animationTimeLab = 35;

	if (speedType == "labSpeed")
	{
		m_limit = m_animationTimeLab;
	}
	else
	{
		m_limit = m_animationTimeLaptop;
	}

	SpriteClips();
}

void Score::SpriteClips()
{
	gSpriteClips[0] = { 0, 0, 62, 61 };
	gSpriteClips[1] = { 64, 0, 62, 61 };
	gSpriteClips[2] = { 128, 0, 62, 61 };
	gSpriteClips[3] = { 192, 0, 62, 61 };
	gSpriteClips[4] = { 256, 0, 62, 61 };
	gSpriteClips[5] = { 320, 0, 62, 61 };
}

void Score::Draw()
{
	if (m_alive)
	{
		currentClip = &gSpriteClips[m_animationFrames / ANIMATION_FRAMES];
		m_animationSprite->SetSourceRect(*currentClip);
		m_animationSprite->Draw(1);
	}
}

void Score::Update()
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

void Score::Reset()
{
	m_alive = true;
	myBody->SetActive(true);
}

void Score::Destroy()
{
	myBody->GetWorld()->DestroyBody(myBody);
}

bool Score::CheckCollision(b2Body* playerBody)
{
	bool collided = (b2TestOverlap(myBody->GetFixtureList()->GetAABB(0), playerBody->GetFixtureList()->GetAABB(0)));
	if (collided)
	{
		m_alive = false;
		myBody->SetActive(false);
		if (SoundManager::GetInstance()->SoundOn())
			SoundManager::GetInstance()->play(SoundManager::SCORE_PICKUP);
	}

	return collided;
}

bool Score::GetAlive()
{
	return m_alive;
}

void Score::SetAlive(bool myAlive)
{
	m_alive = myAlive;
}