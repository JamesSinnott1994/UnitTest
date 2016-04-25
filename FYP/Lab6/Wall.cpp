#include "stdafx.h"
#include "Wall.h"

Wall::Wall()
{
}

Wall::Wall(SDL_Rect pRect, b2World *wWorld)
{
	m_rect = pRect;

	// Define the ground body.
	m_bodyDef.position.Set(pRect.x, pRect.y);
	m_bodyDef.type = b2_staticBody;

	// Define the ground box shape.
	// The extents are the half-widths of the box.
	m_shape.SetAsBox(pRect.w / 2, pRect.h / 2);

	m_bodyFixtureDef.shape = &m_shape;

	// The body is also added to the world.
	m_body = wWorld->CreateBody(&m_bodyDef);

	// Add the ground fixture to the ground body.
	m_body->CreateFixture(&m_bodyFixtureDef);
}

void Wall::Destroy()
{
	m_body->GetWorld()->DestroyBody(m_body);
}