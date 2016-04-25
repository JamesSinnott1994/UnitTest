#include "stdafx.h"
#include "Teleporter.h"

bool Teleporter::instanceFlag = false;
Teleporter* Teleporter::instance = NULL;

Teleporter* Teleporter::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new Teleporter;
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance;
	}
}

Teleporter::Teleporter()
{

}

void Teleporter::Init(SDL_Rect pRect, b2World *pWorld, string speedType)
{
	m_rect = pRect;

	// Define the ground body.
	m_bodyDef.position.Set(pRect.x, pRect.y);
	m_bodyDef.type = b2_staticBody;

	// Define the ground box shape.
	// The extents are the half-widths of the box.
	m_shape.SetAsBox(pRect.w / 2, pRect.h / 2);

	m_bodyFixtureDef.shape = &m_shape;
	m_bodyFixtureDef.isSensor = true;

	// The body is also added to the world.
	m_body = pWorld->CreateBody(&m_bodyDef);

	m_texture = Sprite::loadTexture("Images/LevelEntities/TeleporterAnimation.png", Renderer::GetInstance()->Get_SDL_RENDERER());
	m_source = { 0, 0, 1491, 236 };

	// Add the ground fixture to the ground body.
	m_body->CreateFixture(&m_bodyFixtureDef);
	m_animationSprite = new Sprite();
	m_sprite = new Sprite();
	m_animationSprite->Init(m_texture, pRect, m_source);
	m_sprite->Init(m_texture, pRect, m_source);
	m_animationSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
	m_sprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });

	gSpriteClips[ANIMATION_FRAMES];

	// Speed
	m_animationTimeLaptop = 5;
	m_animationTimeLab = 30;

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

void Teleporter::SpriteClips()
{
	gSpriteClips[0] = { 26, 2, 151, 233 };
	gSpriteClips[1] = { 252, 2, 151, 233 };
	gSpriteClips[2] = { 449, 2, 151, 233 };
	gSpriteClips[3] = { 690, 2, 151, 233 };
	gSpriteClips[4] = { 942, 2, 151, 233 };
	gSpriteClips[5] = { 1208, 2, 151, 233 };
	gSpriteClips[6] = { 1477, 2, 151, 233 };
	gSpriteClips[7] = { 1751, 2, 151, 233 };
	gSpriteClips[8] = { 2002, 2, 151, 233 };
	gSpriteClips[9] = { 2246, 2, 151, 233 };
}

void Teleporter::Draw()
{
	currentClip = &gSpriteClips[m_animationFrames / ANIMATION_FRAMES];
	m_animationSprite->SetSourceRect(*currentClip);
	m_animationSprite->Draw(1);

	currentClip2 = &gSpriteClips[0];
	m_sprite->SetSourceRect(*currentClip2);
	m_sprite->Draw(1);

}

void Teleporter::Destroy()
{
	m_body->GetWorld()->DestroyBody(m_body);
}

void Teleporter::Update()
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

	m_animationSprite->SetPosition(m_body->GetPosition().x, m_body->GetPosition().y);
}

bool Teleporter::CheckCollision(int playerXPos, int playerYPos, int width, int height)
{
	if (playerXPos + width >= m_body->GetPosition().x + 65
		&& playerXPos <= m_body->GetPosition().x + m_rect.w - 65
		&& playerYPos + height >= m_body->GetPosition().y
		&& playerYPos <= m_body->GetPosition().y + m_rect.h)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Teleporter::SetPosition(SDL_Rect position)
{
	m_body->SetTransform(b2Vec2(position.x, position.y), 0);
	m_animationSprite->SetPosition(position.x, position.y);
	m_sprite->SetPosition(position.x, position.y);
}

SDL_Rect Teleporter::GetRect()
{
	return m_rect;
}