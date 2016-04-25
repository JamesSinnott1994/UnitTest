#include "stdafx.h"
#include "RobotBullet.h"

RobotBullet::RobotBullet() {}

RobotBullet::RobotBullet(SDL_Texture* text, int width, int height, SDL_Rect pRect, b2World* wWorld, SDL_Rect src, bool facingRight, SDL_Rect gruntRect, float bulDirX, float bulDirY)
{
	m_rect = pRect;
	m_gruntRect = gruntRect;

	m_width = width;
	m_height = height;

	bulletDirX = bulDirX;
	bulletDirY = bulDirY;

	// Define the ground body.
	myBodyDef.position.Set(m_rect.x, m_rect.y);
	myBodyDef.type = b2_kinematicBody;
	myBodyDef.bullet = true;

	// Define the ground box shape.
	// The extents are the half-widths of the box.
	myShape.SetAsBox(m_rect.w / 2, m_rect.h / 2);

	myBodyFixtureDef.shape = &myShape;

	// The body is also added to the world.
	myBody = wWorld->CreateBody(&myBodyDef);

	// Collision Filtering
	short GROUP_PLAYER = -1;
	short GROUP_GRUNT = -2;

	myBodyFixtureDef.filter.groupIndex = GROUP_GRUNT;


	// Add the ground fixture to the ground body.
	myBody->CreateFixture(&myBodyFixtureDef);

	// Sprite
	sprite = new Sprite();
	sprite->Init(text, pRect, src);
	sprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });

	// Direction
	m_facingRight = facingRight;

	m_alive = true;
	m_collided = false;
}

void RobotBullet::Draw()
{
	sprite->Draw(1);
}

bool RobotBullet::Update(b2Body* playerBody)
{
	if (m_facingRight)
		myBody->SetLinearVelocity(b2Vec2(bulletDirX * 4, bulletDirY * 4));
	else
		myBody->SetLinearVelocity(b2Vec2(bulletDirX * 4, bulletDirY * 4));

	m_rect.x = myBody->GetPosition().x;
	m_rect.y = myBody->GetPosition().y;
	sprite->SetPosition(myBody->GetPosition().x, myBody->GetPosition().y);

	if (CheckBulletPlayerCollision(playerBody))
	{
		SetCollided(true);
		return true;
	}
	else
	{
		SetCollided(false);
		return false;
	}
}

bool RobotBullet::OutOfBounds(SDL_Rect &playerRect)
{
	float distance;

	distance = sqrt(((m_rect.x - m_gruntRect.x)*(m_rect.x - m_gruntRect.x)) + ((m_rect.y - m_gruntRect.y)*(m_rect.y - m_gruntRect.y)));

	if (distance > m_width || m_rect.x < (playerRect.x - 300))// 300 is the startMovingCameraPosition
	{
		myBody->GetWorld()->DestroyBody(myBody);
		return true;
	}
	else
		return false;
}

bool RobotBullet::CheckBulletPlayerCollision(b2Body* playerBody)
{
	bool collided = (b2TestOverlap(myBody->GetFixtureList()->GetAABB(0), playerBody->GetFixtureList()->GetAABB(0)));

	if (collided)
	{
		myBody->SetLinearVelocity(b2Vec2(0, myBody->GetLinearVelocity().y));
		myBody->GetWorld()->DestroyBody(myBody);
		m_alive = false;
	}

	return collided;

}

void RobotBullet::Destroy()
{
	myBody->GetWorld()->DestroyBody(myBody);
}

bool RobotBullet::GetAlive()
{
	return m_alive;
}
void RobotBullet::SetAlive(bool myAlive)
{
	m_alive = myAlive;
}

bool RobotBullet::Collided()
{
	return m_collided;
}
void RobotBullet::SetCollided(bool myCollided)
{
	m_collided = myCollided;
}