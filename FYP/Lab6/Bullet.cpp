#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet(SDL_Texture* text, SDL_Rect pRect, b2World* wWorld, SDL_Rect src, bool movingRight)
{
	myRect = pRect;

	// Define the ground body.
	myBodyDef.position.Set(pRect.x, pRect.y);
	myBodyDef.type = b2_kinematicBody;
	myBodyDef.bullet = true;

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
	sprite.SetOffset(SDL_Point{ 16, 16 });

	m_alive = true;

	if (movingRight)
	{
		m_movingRight = true;
		m_movingLeft = false;
	}
	else
	{
		m_movingRight = false;
		m_movingLeft = true;
	}
}

void Bullet::Draw()
{
	if (m_alive)
		sprite.Draw(1);
}

void Bullet::Update()
{
	if (m_movingRight)
		myBody->SetLinearVelocity(b2Vec2(4, myBody->GetLinearVelocity().y));
	else
		myBody->SetLinearVelocity(b2Vec2(-4, myBody->GetLinearVelocity().y));

	sprite.SetPosition(myBody->GetPosition().x, myBody->GetPosition().y);
}

void Bullet::Destroy()
{
	myBody->GetWorld()->DestroyBody(myBody);
}

bool Bullet::OutOfBounds(SDL_Rect playerPos)
{
	float distance = playerPos.x - sprite.GetPosition().x;

	if (distance <= -800 || distance >= 800)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Bullet::CheckBulletGruntCollision(bool hasMachineGun)
{
	bool collided = EnemyManager::GetInstance()->CheckBulletGruntCollision(myBody, hasMachineGun);
	return collided;
}

bool Bullet::CheckBulletRobotCollision()
{
	bool collided = EnemyManager::GetInstance()->CheckBulletRobotCollision(myBody);
	return collided;
}

b2Body* Bullet::getBody()
{
	return myBody;
}

bool Bullet::GetAlive()
{
	return m_alive;
}

void Bullet::SetAlive(bool myAlive)
{
	m_alive = myAlive;
}