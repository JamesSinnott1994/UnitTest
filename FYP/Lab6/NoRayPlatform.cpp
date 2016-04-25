#include "stdafx.h"
#include "NoRayPlatform.h"

NoRayPlatform::NoRayPlatform()
{

}

NoRayPlatform::NoRayPlatform(SDL_Texture* text, SDL_Rect pRect, b2World* wWorld, SDL_Rect src)
{
	m_rect = &pRect;

	// Define the ground body.
	m_bodyDef.position.Set(pRect.x, pRect.y);
	m_bodyDef.type = b2_staticBody;

	// Define the ground box shape.
	// The extents are the half-widths of the box.
	m_shape.SetAsBox(pRect.w / 2, pRect.h / 2);
	m_bodyFixtureDef.shape = &m_shape;

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	m_body = wWorld->CreateBody(&m_bodyDef);

	// Collision Filtering
	short GROUP_PLATFORM = 3;

	m_bodyFixtureDef.filter.groupIndex = GROUP_PLATFORM;
	m_bodyFixtureDef.friction = 0;

	// Add the ground fixture to the ground body.
	m_body->CreateFixture(&m_bodyFixtureDef);
	m_sprite = new Sprite();
	m_sprite->Init(text, pRect, src);
	m_sprite->SetOffset(SDL_Point{ m_rect->w / 2, m_rect->h / 2 });
}

void NoRayPlatform::Destroy()
{
	m_body->GetWorld()->DestroyBody(m_body);
}

void NoRayPlatform::Draw()
{
	m_sprite->Draw(1);
}