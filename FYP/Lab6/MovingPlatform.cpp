#include "stdafx.h"
#include "MovingPlatform.h"

MovingPlatform::MovingPlatform(){}

// Actual moving platforms
MovingPlatform::MovingPlatform(SDL_Texture* text, SDL_Rect pRect, b2World* wWorld, SDL_Rect src, string speedType, bool pLeftRight)
{
	m_rect = pRect;
	m_startRect = pRect;

	// Define the ground body.
	m_bodyDef.position.Set(pRect.x, pRect.y);
	m_bodyDef.type = b2_kinematicBody;

	// Define the ground box shape.
	// The extents are the half-widths of the box.
	m_shape.SetAsBox(pRect.w / 2, pRect.h / 2);
	m_bodyFixtureDef.shape = &m_shape;

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	m_bodyFixtureDef.friction = 100;
	m_body = wWorld->CreateBody(&m_bodyDef);

	// Collision Filtering
	short GROUP_PLATFORM = 3;

	m_bodyFixtureDef.filter.groupIndex = GROUP_PLATFORM;

	// Add the ground fixture to the ground body.
	m_body->CreateFixture(&m_bodyFixtureDef);
	m_sprite.Init(text, pRect, src);
	m_sprite.SetOffset(SDL_Point{ 48, 16 });

	m_body->SetLinearVelocity(b2Vec2{ -2.0f, 0.0f });
	type = "platform";

	leftRight = pLeftRight;
}

// SENSOR for moving platforms
MovingPlatform::MovingPlatform(SDL_Rect pRect, b2World* wWorld, int dir)
{
	m_rect = pRect;
	m_bodyDef.position.Set(pRect.x, pRect.y);
	m_bodyDef.type = b2_dynamicBody;
	m_bodyDef.gravityScale = 0;

	m_shape.SetAsBox(pRect.w / 2, pRect.h / 2);
	sensorShape.m_radius = 2;

	direction = dir;

	m_bodyFixtureDef.isSensor = true;
	m_bodyFixtureDef.shape = &sensorShape;
	if (direction == RIGHT)
		m_bodyFixtureDef.userData = "moveRight";
	else if (direction == LEFT)
		m_bodyFixtureDef.userData = "moveLeft";
	else if (direction == UP)
		m_bodyFixtureDef.userData = "moveUp";
	else if (direction == DOWN)
		m_bodyFixtureDef.userData = "moveDown";

	m_body = wWorld->CreateBody(&m_bodyDef);
	m_body->CreateFixture(&m_bodyFixtureDef);

	type = "sensor";
}

void MovingPlatform::Update()
{
	if (leftRight)
		MoveLeftRightPlatform();
	else
		MoveUpDownPlatform();
}

void MovingPlatform::MoveLeftRightPlatform()
{
	//
	if (m_body->GetLinearVelocity().x != 2 || m_body->GetLinearVelocity().x != -2)
	{
		int direction = 0;
		if (m_body->GetLinearVelocity().x < 0)
			direction = -1;
		else
			direction = 1;

		m_body->SetLinearVelocity(b2Vec2(2 * direction, 0));
	}

	//
	for (b2ContactEdge* edge = m_body->GetContactList(); edge; edge = edge->next)
	{
		b2Fixture* a = edge->contact->GetFixtureA();
		b2Fixture* b = edge->contact->GetFixtureB();

		if (a->IsSensor() || b->IsSensor())
		{
			if (((a->GetUserData() == "moveLeft") ||
				(b->GetUserData() == "moveLeft")))
			{
				m_body->SetLinearVelocity(b2Vec2(5, 0));
			}
		}
		if (a->IsSensor() || b->IsSensor())
		{
			if (((a->GetUserData() == "moveRight") ||
				(b->GetUserData() == "moveRight")))
			{
				m_body->SetLinearVelocity(b2Vec2(-5, 0));
			}
		}
	}

	m_sprite.SetPosition(m_body->GetPosition().x, m_body->GetPosition().y);
}

void MovingPlatform::MoveUpDownPlatform()
{
	//
	if (m_body->GetLinearVelocity().y != 1 || m_body->GetLinearVelocity().y != -1)
	{
		int direction = 0;
		if (m_body->GetLinearVelocity().y < 0)
			direction = -1;
		else
			direction = 1;

		m_body->SetLinearVelocity(b2Vec2(0, 1 * direction));
	}

	//
	for (b2ContactEdge* edge = m_body->GetContactList(); edge; edge = edge->next)
	{
		b2Fixture* a = edge->contact->GetFixtureA();
		b2Fixture* b = edge->contact->GetFixtureB();

		if (a->IsSensor() || b->IsSensor())
		{
			if (((a->GetUserData() == "moveDown") ||
				(b->GetUserData() == "moveDown")))
			{
				m_body->SetLinearVelocity(b2Vec2(0, 1));
			}
		}
		if (a->IsSensor() || b->IsSensor())
		{
			if (((a->GetUserData() == "moveUp") ||
				(b->GetUserData() == "moveUp")))
			{
				m_body->SetLinearVelocity(b2Vec2(0, -1));
			}
		}
	}

	m_sprite.SetPosition(m_body->GetPosition().x, m_body->GetPosition().y);
}

void MovingPlatform::Draw()
{
	m_sprite.Draw(1);
}

void MovingPlatform::Destroy()
{
	m_body->GetWorld()->DestroyBody(m_body);
}

void MovingPlatform::Reset()
{
	m_rect = m_startRect;
}

bool MovingPlatform::CheckCollision(b2Body* playerBody)
{
	return(b2TestOverlap(m_body->GetFixtureList()->GetAABB(0), playerBody->GetFixtureList()->GetAABB(0)));
}

std::string MovingPlatform::GetType()
{
	return type;
}

b2Body* MovingPlatform::GetBody()
{
	return m_body;
}

bool MovingPlatform::IsLeftRight()
{
	return leftRight;
}