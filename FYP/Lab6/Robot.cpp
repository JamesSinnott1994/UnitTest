#include "stdafx.h"
#include "Robot.h"

Robot::Robot()
{

}

Robot::Robot(SDL_Rect pRect, b2World* wWorld, int direction, string speedType, int width, int height)
{
	world = wWorld;

	// Screen width and height
	m_width = width;
	m_height = height;

	// Position
	m_rect.x = pRect.x;
	m_rect.y = pRect.y;
	m_rect.w = 59;
	m_rect.h = 96;
	m_resetRect = m_rect;

	// Raycasting
	p1.x = m_rect.x;
	p1.y = m_rect.y;
	p2.x = 0;
	p2.y = 0;
	rayLength = 0;
	cantFire = false;
	playerAbove = false;

	// Health
	m_health = 100;

	// Direction
	if (direction == 1)// RIGHT
	{
		m_facingRight = true;
		m_movingRight = true;
		m_movingLeft = false;
	}
	else // LEFT
	{
		m_facingRight = false;
		m_movingRight = false;
		m_movingLeft = true;
	}

	// Box2D stuff
	m_bodyDef.type = b2_dynamicBody;
	m_bodyDef.position.Set(m_rect.x, m_rect.y);
	m_body = wWorld->CreateBody(&m_bodyDef);

	// Define the ground box shape.
	// The extents are the half-widths of the box.
	m_shape.SetAsBox(m_rect.w / 2, m_rect.h / 2);
	m_bodyFixtureDef.shape = &m_shape;

	// Collision Filtering
	short GROUP_PLAYER = -1;
	short GROUP_ROBOT = -2;

	m_bodyFixtureDef.filter.groupIndex = GROUP_ROBOT;

	// Add the ground fixture to the ground body.
	m_body->CreateFixture(&m_bodyFixtureDef);

	// Grunt Initial idle sprite
	m_idleSprite = new Sprite();
	m_source = { 0, 0, m_rect.w, m_rect.h };
	m_idleSprite->Init("Images/Enemy/RobotIdleLeft.png", m_rect, m_source);
	m_idleSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });

	// Running sprite
	m_runningSprite = new Sprite();
	m_source = { 0, 0, 1344, 96 };
	m_runningSprite->Init("Images/Enemy/RobotRunningLeft.png", m_rect, m_source);
	m_runningSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
	
	// Grunt states
	m_idle = true;
	m_running = false;
	m_shooting = false;
	m_inRange = false;
	m_dead = false;
	m_alive = true;
	m_playerToTheLeft = false;
	m_drawn = true;
	m_canCreateBullet = false;

	// For player score
	m_scoreAdded = false;

	// Running Frames/Animation
	gSpriteRunningClipsRight[RUNNING_ANIMATION_FRAMES];
	gSpriteRunningClipsLeft[RUNNING_ANIMATION_FRAMES];
	m_runningFrames = 0;
	m_runningAnimationTime = 0;
	m_runningAnimationLimit = 0;
	m_runningAnimationLimitLab = 60;
	m_runningAnimationLimitLaptop = 5;

	// Shoot timer
	m_shootTimerLaptop = 100;
	m_shootTimerLab = 2000;

	// Bullet
	bulletDirX = 0;
	bulletDirY = 0;
	angle = 0;
	angleOkToShoot = false;

	// Speed
	if (speedType == "labSpeed")
	{
		m_runningAnimationLimit = m_runningAnimationLimitLab;
		m_shootTimerLimit = m_shootTimerLab;
		m_shootTimer = m_shootTimerLimit;
	}
	else
	{
		m_runningAnimationLimit = m_runningAnimationLimitLaptop;
		m_shootTimerLimit = m_shootTimerLaptop;
		m_shootTimer = m_shootTimerLimit;
	}

	SpriteClips();
}

void Robot::SpriteClips()
{
	// Right
	gSpriteRunningClipsRight[0] = { 0, 0, 101, 90 };
	gSpriteRunningClipsRight[1] = { 150, 3, 101, 89 };
	gSpriteRunningClipsRight[2] = { 312, 3, 101, 87 };
	gSpriteRunningClipsRight[3] = { 459, 1, 101, 91 };
	gSpriteRunningClipsRight[4] = { 583, 0, 101, 92 };
	gSpriteRunningClipsRight[5] = { 711, 1, 101, 91 };
	gSpriteRunningClipsRight[6] = { 856, 7, 101, 89 };
	gSpriteRunningClipsRight[7] = { 1016, 9, 101, 87 };
	gSpriteRunningClipsRight[8] = { 1159, 6, 101, 90 };
	gSpriteRunningClipsRight[9] = { 1260, 0, 101, 92 };

	// Left
	gSpriteRunningClipsLeft[0] = { 1255, 0, 101, 90 };
	gSpriteRunningClipsLeft[1] = { 1092, 3, 101, 89 };
	gSpriteRunningClipsLeft[2] = { 951, 3, 101, 87 };
	gSpriteRunningClipsLeft[3] = { 839, 1, 101, 91 };
	gSpriteRunningClipsLeft[4] = { 709, 0, 101, 92 };
	gSpriteRunningClipsLeft[5] = { 553, 1, 101, 91 };
	gSpriteRunningClipsLeft[6] = { 386, 7, 101, 89 };
	gSpriteRunningClipsLeft[7] = { 247, 9, 101, 87 };
	gSpriteRunningClipsLeft[8] = { 124, 6, 101, 90 };
	gSpriteRunningClipsLeft[9] = { 0, 0, 101, 92 };
}

void Robot::Draw()
{
	if (m_alive)
	{
		//Render current frame
		if (m_movingRight)
			currentRunnerClip = &gSpriteRunningClipsRight[m_runningFrames / RUNNING_ANIMATION_FRAMES];
		else if (m_movingLeft)
			currentRunnerClip = &gSpriteRunningClipsLeft[m_runningFrames / RUNNING_ANIMATION_FRAMES];

		// Draw
		if (m_idle)
		{
			m_idleSprite->Draw(1);
		}
		if (m_running)
		{
			m_runningSprite->SetSourceRect(*currentRunnerClip);
			m_runningSprite->Draw(1);
		}

		// Update sprite position
		m_rect.x = m_body->GetPosition().x;
		m_rect.y = m_body->GetPosition().y;
		p1.x = m_body->GetPosition().x;
		p1.y = m_body->GetPosition().y;
		m_idleSprite->SetPosition(m_body->GetPosition().x, m_body->GetPosition().y);
		m_runningSprite->SetPosition(m_body->GetPosition().x, m_body->GetPosition().y);
	}
}

void Robot::Update(SDL_Rect &playerRect, int noOfBullets, int maxBullets, b2Body* playerBody)
{
	// Update time to shoot
	if (m_shootTimer <= m_shootTimerLimit && m_inRange)
		m_shootTimer++;
	else
	{
		m_canCreateBullet = false;
		m_shootTimer = m_shootTimerLimit + 2;
	}

	State s = FiniteStateMachine();

	switch (s)
	{
	case IDLE:
		break;
	case RUNNING:
		Running();
		break;
	case SHOOTING:
		Shoot(noOfBullets, maxBullets);
		break;
	case DEAD:
		break;
	}

	// Check if player is above or below grunt
	if (playerBody->GetPosition().y >= m_body->GetPosition().y)
	{
		playerAbove = false;
	}
	else
	{
		playerAbove = true;
	}

	// Check if player is left or right of grunt
	if (playerBody->GetPosition().x >= m_body->GetPosition().x)
	{
		m_playerToTheLeft = false;
	}
	else
	{
		m_playerToTheLeft = true;
	}

	ChangeDirection(playerRect.x);
	GetDirectionToPlayer(&playerRect);

	// Cast our ray to determine if object is in front of player which prevents firing
	if (angleOkToShoot && m_inRange)
	{
		CastRay();
	}
	else
	{
		cantFire = false;
	}

	m_inRange = InRangeOfPlayer(playerRect);

	//Animation();
}

Robot::State Robot::FiniteStateMachine()
{
	State s;
	if (m_idle)
		s = IDLE;
	if (m_running)
		s = RUNNING;
	if (m_inRange && cantFire)
		s = SHOOTING;
	if (m_dead)
		s = DEAD;

	return s;
}

void Robot::CastRay()
{
	// Raycasting
	input.p1 = p1;
	input.p2 = p2;
	input.maxFraction = 1;

	b2Vec2* vec = PlatformManager::GetInstance()->getRayIntersectionPoint(output, input, playerAbove, m_playerToTheLeft, m_body->GetPosition().x, m_body->GetPosition().y);
	intersectionPoint.x = vec->x;
	intersectionPoint.y = vec->y;

	// Check if intersection point is within confines of player
	if (intersectionPoint.x == p2.x && intersectionPoint.y == p2.y)
	{
		cantFire = true;
	}
	else
	{
		cantFire = false;
	}
}

void Robot::Reset()
{
	m_body->SetTransform(b2Vec2(m_resetRect.x, m_resetRect.y), 0);
	m_alive = true;
	m_body->SetActive(true);
	m_idle = true;
	m_running = false;
	m_shooting = false;
	m_inRange = false;
	m_dead = false;
	m_playerToTheLeft = false;
	m_drawn = true;
	m_canCreateBullet = false;
	m_shootTimer = 0;
	m_health = 100;
	m_scoreAdded = false;

	// Bullet
	bulletDirX = 0;
	bulletDirY = 0;
	angle = 0;
	angleOkToShoot = false;

	// Raycasting
	p1.x = m_resetRect.x;
	p1.y = m_resetRect.y;
	p2.x = 0;
	p2.y = 0;
	rayLength = 0;
	cantFire = false;
	playerAbove = false;


	m_body->SetLinearVelocity(b2Vec2(0, m_body->GetLinearVelocity().y - 0.000001f));
}

void Robot::Running()
{
	if (m_facingRight)// RIGHT
	{
		//m_body->SetLinearVelocity(b2Vec2(2, m_body->GetLinearVelocity().y - 0.000001f));

		// Change sprite image if not already moving right
		if (!m_movingRight)
		{
			m_source = { 0, 0, 1344, 96 };
			m_runningSprite->Init("Images/Enemy/RobotRunningRight.png", m_rect, m_source);
			m_runningSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
		}

		// Change booleans
		m_movingLeft = false;
		m_movingRight = true;

		// Increase running frames
		m_runningAnimationTime++;
		if (m_runningAnimationTime >= m_runningAnimationLimit)
		{
			++m_runningFrames;
			m_runningAnimationTime = 0;
		}
	}
	else // LEFT
	{
		//m_body->SetLinearVelocity(b2Vec2(-2, m_body->GetLinearVelocity().y - 0.000001f));

		// Change sprite image if not already moving left
		if (!m_movingLeft)
		{
			m_source = { 0, 0, 1344, 96 };
			m_runningSprite->Init("Images/Enemy/RobotRunningLeft.png", m_rect, m_source);
			m_runningSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
		}

		// Change booleans
		m_movingLeft = true;
		m_movingRight = false;

		// Increase running frames
		m_runningAnimationTime++;
		if (m_runningAnimationTime >= m_runningAnimationLimit)
		{
			++m_runningFrames;
			m_runningAnimationTime = 0;
		}

	}
}

void Robot::Animation()
{
	//Cycle animation
	if (m_runningFrames / RUNNING_ANIMATION_FRAMES >= RUNNING_ANIMATION_FRAMES)
	{
		m_runningFrames = 0;
	}
}

void Robot::Shoot(int noOfBullets, int maxBullets)
{
	if (m_shootTimer >= m_shootTimerLimit && noOfBullets <= maxBullets && angleOkToShoot)
	{
		//CreateBullet(m_gruntBullets);
		m_canCreateBullet = true;
		m_shootTimer = 0;
	}
	else
	{
		m_canCreateBullet = false;
	}
}

list<RobotBullet*> Robot::CreateBullet(list<RobotBullet*>m_robotBullets)
{
	SDL_Texture* m_bulletTexture;
	SDL_Rect m_bulletSource = { 0, 0, 9, 6 };
	SDL_Rect m_bulletPos;

	if (m_facingRight)
	{
		m_rect.w = 90;
		m_source = { 0, 0, 90, 96 };
		m_idleSprite->Init("Images/Enemy/RobotShootRight.png", m_rect, m_source);
		m_idleSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });

		m_bulletTexture = Sprite::loadTexture("Images/Enemy/RobotBulletLeft.png", Renderer::GetInstance()->Get_SDL_RENDERER());
		m_bulletPos = { m_rect.x + 40, m_rect.y - 15, 10, 7 };
	}
	else
	{
		m_rect.w = 90;
		m_source = { 0, 0, 90, 96 };
		m_idleSprite->Init("Images/Enemy/RobotShootLeft.png", m_rect, m_source);
		m_idleSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });

		m_bulletTexture = Sprite::loadTexture("Images/Enemy/RobotBulletLeft.png", Renderer::GetInstance()->Get_SDL_RENDERER());
		m_bulletPos = { m_rect.x - 40, m_rect.y - 15, 10, 7 };
	}

	RobotBullet* bullet = new RobotBullet(m_bulletTexture, m_width, m_height, m_bulletPos, world, m_bulletSource, m_facingRight, m_rect, bulletDirX, bulletDirY);

	m_robotBullets.push_back(bullet);

	return m_robotBullets;
}

void Robot::ResetTimer()
{
	m_shootTimer = 0;
}

void Robot::Destroy()
{
	m_body->GetWorld()->DestroyBody(m_body);
}

void Robot::ChangeDirection(int &playerXPos)
{
	if (playerXPos < m_rect.x && m_facingRight)// Player is to the left of grunt
	{
		m_rect.w = 59;
		m_facingRight = false;
		m_source = { 0, 0, 59, 96 };
		m_idleSprite->Init("Images/Enemy/RobotIdleLeft.png", m_rect, m_source);
		m_idleSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
	}
	else if (playerXPos > m_rect.x && !m_facingRight)// Player is to the right of grunt
	{
		m_rect.w = 59;
		m_facingRight = true;
		m_source = { 0, 0, 59, 96 };
		m_idleSprite->Init("Images/Enemy/RobotIdleRight.png", m_rect, m_source);
		m_idleSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
	}
}

bool Robot::RobotCheckCollision(b2Body* bulletBody)
{
	bool collided = (b2TestOverlap(m_body->GetFixtureList()->GetAABB(0), bulletBody->GetFixtureList()->GetAABB(0)));
	if (collided)
	{
		m_health -= 20;

		if (m_health <= 0)
		{
			m_alive = false;
			m_body->SetActive(false);
		}
	}

	return collided;
}

void Robot::GetDirectionToPlayer(SDL_Rect* playerRect)
{
	float playerXPos = playerRect->x;// +(playerRect->w / 2);
	float playerYPos = playerRect->y;// +(playerRect->h / 2);

	bulletDirX = playerXPos - m_body->GetPosition().x;
	bulletDirY = playerYPos - m_body->GetPosition().y;

	angle = atan2(bulletDirY, bulletDirX) * 180 / 3.141;

	angleOkToShoot = false;
	if (m_facingRight)// RIGHT
	{
		if ((angle >= -75 && angle <= 0)
			|| (angle >= 0 && angle <= 75))
		{
			angleOkToShoot = true;
		}
	}
	else // LEFT
	{
		if ((angle <= -105 && angle >= -180)
			|| (angle >= 105 && angle <= 180))
		{
			angleOkToShoot = true;
		}
	}

	// Normalize
	float length = sqrt((bulletDirX*bulletDirX) + (bulletDirY*bulletDirY));
	rayLength = length;


	if (length > 0)
	{
		bulletDirX = bulletDirX / length;
		bulletDirY = bulletDirY / length;

		p2.x = p1.x + rayLength * (float)cos(angle* 3.14159265 / 180);
		p2.y = p1.y + rayLength * (float)sin(angle* 3.14159265 / 180);
	}
}

bool Robot::InRangeOfPlayer(SDL_Rect &playerRect)
{
	float distance;

	SDL_Point gruntCentre;
	gruntCentre.x = m_rect.x + (m_rect.w / 2);
	gruntCentre.y = m_rect.y + (m_rect.h / 2);

	SDL_Point playerCentre;
	playerCentre.x = playerRect.x + (playerRect.w / 2);
	playerCentre.y = playerRect.y + (playerRect.h / 2);

	// Get distance
	float distanceX;
	float distanceY;

	distanceX = gruntCentre.x - playerCentre.x;
	distanceY = gruntCentre.y - playerCentre.y;

	float length = sqrt((distanceX*distanceX) + (distanceY*distanceY));

	// Check if in range
	if (length <= 600)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Robot::CheckBulletPlayerCollision(b2Body* playerBody)
{
	if (m_bullets.size() > 0)
	{
		// Iterate through list of bullets
		for (m_bulletIterator = m_bullets.begin(); m_bulletIterator != m_bullets.end(); ++m_bulletIterator)
		{
			bool collided = (*m_bulletIterator)->CheckBulletPlayerCollision(playerBody);

			if (collided)
			{
				m_bullets.erase(m_bulletIterator);
			}

			return collided;
		}
	}
	else
	{
		return false;
	}
}

void Robot::Fell()
{
	// Out of bounds from falling
	if (m_body->GetPosition().y >= m_height)
	{
		m_alive = false;
		cout << "Enemy killed" << endl;
	}
}

#pragma region Getters/Setters

bool Robot::GetAlive()
{
	return m_alive;
}

void Robot::SetAlive(bool myAlive)
{
	m_alive = myAlive;
}

bool Robot::InRange()
{
	return m_inRange;
}

void Robot::SetInRange(bool myInRange)
{
	m_inRange = myInRange;
}

bool Robot::PlayerToTheLeft()
{
	return m_playerToTheLeft;
}

void Robot::SetPlayerToTheLeft(bool myPlayerToTheLeft)
{
	m_playerToTheLeft = myPlayerToTheLeft;
}

bool Robot::CanCreateBullet()
{
	return m_canCreateBullet;
}

void Robot::SetCanCreateBullet(bool myCanCreateBullet)
{
	m_canCreateBullet = myCanCreateBullet;
}

bool Robot::GetScoreAdded()
{
	return m_scoreAdded;
}

void Robot::SetScoreAdded(bool myScoreAdded)
{
	m_scoreAdded = myScoreAdded;
}

bool Robot::OnScreen(float playerXPos, float playerYPos)
{
	// Get distance
	float distanceX;
	float distanceY;

	distanceX = m_body->GetPosition().x - playerXPos;
	distanceY = m_body->GetPosition().y - playerYPos;

	float length = sqrt((distanceX*distanceX) + (distanceY*distanceY));

	// Check if in range
	if (length <= 500)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#pragma endregion