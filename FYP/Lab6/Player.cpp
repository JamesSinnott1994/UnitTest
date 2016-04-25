#include "stdafx.h"
#include "Player.h"

Player::Player(){}

void Player::Init(SDL_Rect pRect, b2World *pWorld, string speedType, float scale)
{
	world = pWorld;

	playerScale = scale;

	// Position
	m_rect = pRect;
	m_startRect = pRect;

	// Box2D stuff
	m_bodyDef.type = b2_dynamicBody;
	m_bodyDef.position.Set(m_rect.x, m_rect.y);
	m_body = pWorld->CreateBody(&m_bodyDef);

	b2Vec2 vertices[8];

	// Bottom Left (-,+)
	vertices[0].Set(-14, 46.0);
	vertices[1].Set(-14.5, 45.5);

	// Bottom Right (+,+)
	vertices[6].Set(14.5, 45.5);
	vertices[7].Set(14, 46.0);

	// Top Left (-,-)
	vertices[2].Set(-14.5, -47.5);
	vertices[3].Set(-14, -48);

	// Top Right (+,-)
	vertices[4].Set(14, -48);
	vertices[5].Set(14.5, -47.5);

	//m_shape.SetAsBox(m_rect.w / 2 - 20, (m_rect.h / 2)-2);
	m_shape.Set(vertices, 8);
	m_bodyFixtureDef.shape = &m_shape;

	// Collision Filtering
	short GROUP_PLAYER = -2;
	short GROUP_GRUNT = -2;
	short GROUP_BARRIER = -2;

	m_bodyFixtureDef.filter.groupIndex = GROUP_BARRIER;
	m_bodyFixtureDef.friction = 0;

	m_body->CreateFixture(&m_bodyFixtureDef);

	// Player Initial idle sprite
	m_playerIdleSprite = new Sprite();
	m_source = { 0, 0, 77, 107 };
	m_playerIdleSprite->Init("Images/Player/PlayerIdleRight.png", m_rect, m_source);
	m_playerIdleSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h/2 });
	m_idle = true;
	m_drawn = true;

	// Player machine gun idle sprite
	m_playerIdleMachineGunSprite = new Sprite();
	m_source = { 0, 0, 83, 108 };
	m_playerIdleMachineGunSprite->Init("Images/Player/PlayerMachineGunIdleSpriteRight.png", pRect, m_source);
	m_playerIdleMachineGunSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });

	// Initial direction
	m_movingLeft = false;
	m_movingRight = true;

	// Player running animation
	m_playerRunningSprite = new Sprite();
	m_source = { 0, 0, 912, 107 };
	m_playerRunningSprite->Init("Images/Player/PlayerRunningRight.png", pRect, m_source);
	m_playerRunningSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
	m_runningFrames = 0;
	m_runningAnimationTime = 0;
	m_runningAnimationLimit = 0;
	m_runningAnimationLimitLab = 20;
	m_runningAnimationLimitLaptop = 3;
	gSpriteRunningClipsRight[RUNNING_ANIMATION_FRAMES];
	gSpriteRunningClipsLeft[RUNNING_ANIMATION_FRAMES];
	m_running = false;

	// Player running animation with machine gun
	m_machineGunAmmo = 50;
	OldMachineGunAmmo = 50;
	m_playerRunningMGSprite = new Sprite();
	m_source = { 0, 0, 414, 108 };
	m_playerRunningMGSprite->Init("Images/Player/PlayerMachineGunRunningSpriteRight.png", pRect, m_source);
	m_playerRunningMGSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
	m_runningMGFrames = 0;
	m_runningMGAnimationTime = 0;
	m_runningMGAnimationLimit = 0;
	m_runningMGAnimationLimitLab = 60;
	m_runningMGAnimationLimitLaptop = 10;
	gSpriteRunningMGClipsRight[RUNNING_MG_ANIMATION_FRAMES];
	gSpriteRunningMGClipsLeft[RUNNING_MG_ANIMATION_FRAMES];

	// Jump
	m_canJump = false;

	// Score
	m_score = 0;
	OldScore = 0;

	// Health
	m_health = 100;
	OldHealth = 100;
	m_lives = 1;
	m_alive = true;

	// Machine gun
	m_hasMachineGun = false;
	m_machineGunEquipped = false;
	oldPickedUp = false;
	m_shootTimerMGLab = 300;
	m_shootTimerMGLaptop = 30;

	// Bullets
	m_timeToShoot = 500;
	m_shootTimerLab = 600;
	m_shootTimerLaptop = 120;

	// Blood
	m_bloodSprite = new Sprite();
	m_source = { 0, 0, 3072, 512 };
	SDL_Rect bloodRect;
	m_bloodSprite->Init("Images/Player/BloodSplatter.png", pRect, m_source);
	m_bloodSprite->SetOffset(SDL_Point{ 38.5f, 53.5f });
	m_bloodFrames = 0;
	m_bloodAnimationTime = 0;
	m_bloodAnimationLimit = 0;
	m_bloodAnimationLimitLab = 30;
	m_bloodAnimationLimitLaptop = 3;
	bloodSpriteClips[BLOOD_ANIMATION_FRAMES];
	m_stopBloodAnimation = false;

	// Speed
	if (speedType == "labSpeed")
	{
		m_shootTimerLimit = m_shootTimerLab;
		m_shootTimerMGLimit = m_shootTimerMGLab;
		m_bloodAnimationLimit = m_bloodAnimationLimitLab;
		m_runningAnimationLimit = m_runningAnimationLimitLab;
		m_runningMGAnimationLimit = m_runningMGAnimationLimitLab;
	}
	else
	{
		m_shootTimerLimit = m_shootTimerLaptop;
		m_shootTimerMGLimit = m_shootTimerMGLaptop;
		m_bloodAnimationLimit = m_bloodAnimationLimitLaptop;
		m_runningAnimationLimit = m_runningAnimationLimitLaptop;
		m_runningMGAnimationLimit = m_runningMGAnimationLimitLaptop;
	}

	SpriteClips();

	m_reachedTeleporter = false;
	collidingWithMovingPlat = false;
}

void Player::SpriteClips()
{
	// Running sprite clips
	gSpriteRunningClipsRight[0] = { 0, 3, 74, 103 };
	gSpriteRunningClipsRight[1] = { 91, 1, 68, 105 };
	gSpriteRunningClipsRight[2] = { 176, 0, 74, 106 };
	gSpriteRunningClipsRight[3] = { 266, 2, 82, 104 };
	gSpriteRunningClipsRight[4] = { 364, 4, 84, 102 };
	gSpriteRunningClipsRight[5] = { 466, 3, 72, 103 };
	gSpriteRunningClipsRight[6] = { 557, 1, 68, 105 };
	gSpriteRunningClipsRight[7] = { 639, 1, 73, 105 };
	gSpriteRunningClipsRight[8] = { 720, 2, 82, 104 };
	gSpriteRunningClipsRight[9] = { 827, 4, 83, 102 };

	gSpriteRunningClipsLeft[0] = { 837, 3, 74, 103 };
	gSpriteRunningClipsLeft[1] = { 752, 1, 68, 105 };
	gSpriteRunningClipsLeft[2] = { 662, 0, 73, 106 };
	gSpriteRunningClipsLeft[3] = { 563, 2, 82, 104 };
	gSpriteRunningClipsLeft[4] = { 463, 4, 84, 102 };
	gSpriteRunningClipsLeft[5] = { 373, 3, 72, 103 };
	gSpriteRunningClipsLeft[6] = { 286, 1, 68, 105 };
	gSpriteRunningClipsLeft[7] = { 199, 1, 73, 105 };
	gSpriteRunningClipsLeft[8] = { 109, 2, 82, 104 };
	gSpriteRunningClipsLeft[9] = { 1, 4, 83, 102 };

	// Blood
	bloodSpriteClips[0] = { 153, 129, 209, 182 };
	bloodSpriteClips[1] = { 654, 91, 245, 215 };
	bloodSpriteClips[2] = { 1150, 65, 282, 241 };
	bloodSpriteClips[3] = { 1659, 66, 294, 252 };
	bloodSpriteClips[4] = { 2172, 64, 298, 269 };
	bloodSpriteClips[5] = { 2684, 65, 306, 286 };

	// Running with machine gun sprite clips
	gSpriteRunningMGClipsRight[0] = { 1, 6, 81, 102 };
	gSpriteRunningMGClipsRight[1] = { 108, 3, 67, 105 };
	gSpriteRunningMGClipsRight[2] = { 200, 5, 83, 103 };
	gSpriteRunningMGClipsRight[3] = { 307, 3, 66, 105 };
	gSpriteRunningMGClipsRight[4] = { 398, 0, 74, 108 };

	gSpriteRunningMGClipsLeft[0] = { 401, 6, 81, 102 };
	gSpriteRunningMGClipsLeft[1] = { 308, 3, 67, 105 };
	gSpriteRunningMGClipsLeft[2] = { 200, 5, 83, 103 };
	gSpriteRunningMGClipsLeft[3] = { 108, 3, 68, 105 };
	gSpriteRunningMGClipsLeft[4] = { 11, 0, 74, 108 };
}

void Player::Draw()
{
	if (m_alive)
	{
		//Render current frame
		if (m_movingRight)
		{
			if (!m_machineGunEquipped)
				currentRunnerClip = &gSpriteRunningClipsRight[m_runningFrames / 10];
			else
				currentRunnerMGClip = &gSpriteRunningMGClipsRight[m_runningMGFrames / 5];
		}
		else if (m_movingLeft)
		{
			if (!m_machineGunEquipped)
				currentRunnerClip = &gSpriteRunningClipsLeft[m_runningFrames / 10];
			else
				currentRunnerMGClip = &gSpriteRunningMGClipsLeft[m_runningMGFrames / 5];
		}

		// Draw IDLE sprite
		if (m_idle)
		{
			if (!m_machineGunEquipped)
				m_playerIdleSprite->Draw(1);
			else
				m_playerIdleMachineGunSprite->Draw(1);
		}
		// Draw RUNNING sprite
		else if (m_running)
		{
			if (!m_machineGunEquipped)
			{
				m_playerRunningSprite->SetSourceRect(*currentRunnerClip);
				m_playerRunningSprite->Draw(1);
			}
			else
			{
				m_playerRunningMGSprite->SetSourceRect(*currentRunnerMGClip);
				m_playerRunningMGSprite->Draw(1);
			}
		}
	}

	// Draw BLOOD Sprite
	if (!m_alive && !m_stopBloodAnimation)
	{
		currentBloodClip = &bloodSpriteClips[m_bloodFrames / 6];
		m_bloodSprite->SetSourceRect(*currentBloodClip);
		m_bloodSprite->Draw(1);
	}

	// Draw bullets
	if (m_bullets.size() > 0)
	{
		for each(Bullet* bullet in m_bullets)
		{
			bullet->Draw();
		}
	}
}

void Player::Update()
{
	// Update time to shoot
	if (m_timeToShoot <= m_shootTimerLimit)
		m_timeToShoot++;

	if (m_health > 0)
		CheckCollisions();

	// Checks if killed
	if (m_health <= 0)
	{
		if (m_alive)
			m_bloodSprite->SetPosition(m_body->GetPosition().x, m_body->GetPosition().y);
		m_alive = false;
	}

	// Can jump if y-velocity is 0
	if (m_body->GetLinearVelocity().y == 0)
	{
		m_canJump = true;
	}

	else
	{
		// Cannot jump
		if (!collidingWithMovingPlat)
		{
			m_canJump = false;
			m_bodyFixtureDef.friction = 0;
		}
	}

	// Out of bounds from falling
	if (m_body->GetPosition().y >= 800)
	{
		m_health = 0;
	}

	// Call move
	if (m_alive && !m_reachedTeleporter)
	{
		Move();
	}

	// Toggle Weapons
	ToggleWeapons();

	// Draw player when idle
	if (m_idle)
	{
		if (m_movingLeft && !m_drawn)
		{
			//if (!m_machineGunEquipped)// NO MACHINE GUN
			//{
				m_source = { 0, 0, 77, 107 };
				m_playerIdleSprite->Init("Images/Player/PlayerIdleLeft.png", m_rect, m_source);
				m_playerIdleSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
				m_drawn = true;
			//}
			//else// HAS MACHINE GUN
			//{
				m_source = { 0, 0, 83, 108 };
				m_playerIdleMachineGunSprite->Init("Images/Player/PlayerMachineGunIdleSpriteLeft.png", m_rect, m_source);
				m_playerIdleMachineGunSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
				m_drawn = true;
		}
		else if (m_movingRight && !m_drawn)
		{
			//if (!m_machineGunEquipped)// NO MACHINE GUN
			//{
				m_source = { 0, 0, 77, 107 };
				m_playerIdleSprite->Init("Images/Player/PlayerIdleRight.png", m_rect, m_source);
				m_playerIdleSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
				m_drawn = true;
			//}
			//else// HAS MACHINE GUN
			//{
				m_source = { 0, 0, 83, 108 };
				m_playerIdleMachineGunSprite->Init("Images/Player/PlayerMachineGunIdleSpriteRight.png", m_rect, m_source);
				m_playerIdleMachineGunSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
				m_drawn = true;
		}
	}
	
	// Update sprite positions
	if (m_alive)
	{
		m_rect.x = m_body->GetPosition().x;
		m_rect.y = m_body->GetPosition().y;

		if (!m_machineGunEquipped)
		{
			m_playerIdleSprite->SetPosition(m_body->GetPosition().x, m_body->GetPosition().y);
			m_playerRunningSprite->SetPosition(m_body->GetPosition().x, m_body->GetPosition().y);
		}
		else
		{
			m_playerIdleMachineGunSprite->SetPosition(m_body->GetPosition().x, m_body->GetPosition().y);
			m_playerRunningMGSprite->SetPosition(m_body->GetPosition().x, m_body->GetPosition().y);
		}
	}

	UpdateAnimations();
	UpdateBullets();
}

void Player::UpdateAnimations()
{
	//Cycle animation
	if (!m_machineGunEquipped)
	{
		if (m_runningFrames / 10 >= RUNNING_ANIMATION_FRAMES)
		{
			m_runningFrames = 0;
		}
	}
	// Cycle running machine gun animation
	if (m_machineGunEquipped)
	{
		if (m_runningMGFrames / 5 >= RUNNING_MG_ANIMATION_FRAMES)
		{
			m_runningMGFrames = 0;
		}
	}

	// Blood animation
	if (!m_alive && !m_stopBloodAnimation)
	{
		//Cycle animation
		if (m_bloodFrames / 6 >= BLOOD_ANIMATION_FRAMES)
		{
			m_bloodFrames = 0;
			m_stopBloodAnimation = true;
		}

		// Increase blood animation frames
		m_bloodAnimationTime++;
		if (m_bloodAnimationTime >= m_bloodAnimationLimit)
		{
			++m_bloodFrames;
			m_bloodAnimationTime = 0;
		}
	}
}

void Player::UpdateBullets()
{
	// Update bullets
	if (m_bullets.size() > 0)// 1
	{
		// Iterate through list of bullets
		// 2a, 2b, 2c
		for (m_bulletIterator = m_bullets.begin(); m_bulletIterator != m_bullets.end(); ++m_bulletIterator)// 2
		{
			(*m_bulletIterator)->Update();// 3

			if ((*m_bulletIterator)->CheckBulletGruntCollision(m_hasMachineGun) || (*m_bulletIterator)->CheckBulletRobotCollision()) // 4, 5
			{
				// 6
				(*m_bulletIterator)->Destroy();
				m_bullets.erase(m_bulletIterator);
				break;
			}// 7

			// Remove bullet if out of bounds
			if ((*m_bulletIterator)->OutOfBounds(m_rect))// 8
			{
				// 9
				(*m_bulletIterator)->Destroy();
				m_bullets.erase(m_bulletIterator);
				break;
			}// 10
		}
	} // End if 11
}

void Player::ToggleWeapons()
{
	if (m_hasMachineGun)
	{
		if (KeyBoardInput::GetInstance()->isKeyPressed(SDLK_1))
		{
			m_machineGunEquipped = false;
		}
		if (KeyBoardInput::GetInstance()->isKeyPressed(SDLK_2))
		{
			m_machineGunEquipped = true;
		}
	}
}

// Collision Checks
#pragma region Collision Checks
void Player::CheckCollisions()
{
	// Collision with health object
	if (m_health < 100)
	{
		if (CheckHealthCollision())
		{
			m_health += 50;
			if (m_health > 100)
			{
				SetHealth(100);
			}
		}
	}

	// Collision with mine object
	if (CheckMineCollision())
	{
		// Kill player
		m_health -= 50;
	}

	// Collision with electric barrier
	if (CheckBarrierCollision())
	{
		// Kill player
		m_health -= 50;
	}

	if (CheckTeleporterCollision())
	{
		m_reachedTeleporter = true;
		m_body->SetLinearVelocity(b2Vec2(0, m_body->GetLinearVelocity().y - 0.000001f));
	}

	CheckSwitchCollision();

	// Collsion with platform
	if (CheckMovingPlatformCollision())
	{
		collidingWithMovingPlat = true;
	}
	else
	{
		collidingWithMovingPlat = false;
	}

	// Collision with machine gun
	if (CheckMachineGunCollision() && !m_hasMachineGun)
	{
		m_hasMachineGun = true;
		m_machineGunEquipped = true;
	}

	// Collision with platform currently on
	CheckStaticPlatCollision();
}

bool Player::CheckScoreCollision()
{
	return PickupManager::GetInstance()->CheckScoreCollision(m_body);
}
bool Player::CheckHealthCollision()
{
	return PickupManager::GetInstance()->CheckHealthCollision(m_body);
}
bool Player::CheckAmmoCollision()
{
	return PickupManager::GetInstance()->CheckAmmoCollision(m_body);
}
bool Player::CheckMineCollision()
{
	return ObstacleManager::GetInstance()->CheckMineCollision(m_body);
}
bool Player::CheckBarrierCollision()
{
	return ObstacleManager::GetInstance()->CheckBarrierCollision(m_body);
}
bool Player::CheckSwitchCollision()
{
	return ObstacleManager::GetInstance()->CheckSwitchCollision(m_body);
}
bool Player::CheckTeleporterCollision()
{
	return Teleporter::GetInstance()->CheckCollision(m_body->GetPosition().x, m_body->GetPosition().y, m_rect.w, m_rect.h);
}
bool Player::CheckMovingPlatformCollision()
{
	return PlatformManager::GetInstance()->CheckCollision(m_body);
}
bool Player::CheckMachineGunCollision()
{
	return PickupManager::GetInstance()->CheckMachineGunCollision(m_body);
}
SDL_Rect* Player::CheckStaticPlatCollision()
{
	platformOn = PlatformManager::GetInstance()->CheckStaticPlatCollision(m_body);
	return NULL;
}
#pragma endregion

void Player::Move()
{
	// Key presses
	// Move left
	MoveLeft();
	// Move right
	MoveRight();
	// Jump
	if (KeyBoardInput::GetInstance()->isKeyPressed(SDLK_SPACE) && m_canJump)
	{
		Jump();
	}
	// Shoot
	if (KeyBoardInput::GetInstance()->isKeyPressed(SDLK_j))
	{
		if (!m_machineGunEquipped)
		{
			if (m_timeToShoot >= m_shootTimerLimit)
			{
				Shoot();
				m_timeToShoot = 0;
				if (SoundManager::GetInstance()->SoundOn())
					SoundManager::GetInstance()->play(SoundManager::GUNSHOT);
			}
		}
		else
		{
			if (m_timeToShoot >= m_shootTimerMGLimit && m_machineGunAmmo >= 1)
			{
				Shoot();
				m_timeToShoot = 0;
				m_machineGunAmmo -= 1;
				if (SoundManager::GetInstance()->SoundOn())
					SoundManager::GetInstance()->play(SoundManager::GUNSHOT);
			}
		}
	}
	// IF NOT MOVING
	if (!KeyBoardInput::GetInstance()->isKeyPressed(SDLK_SPACE) && !KeyBoardInput::GetInstance()->isKeyPressed(SDLK_d)
		&& !KeyBoardInput::GetInstance()->isKeyPressed(SDLK_a))
	{
		m_running = false;
		m_idle = true;
		if (!collidingWithMovingPlat)
		{
			m_body->SetLinearVelocity(b2Vec2{ 0, m_body->GetLinearVelocity().y });
			m_canJump = false;
		}
		else
		{
			m_body->SetLinearVelocity(b2Vec2(PlatformManager::GetInstance()->xSpeedOfMovingPlatform, PlatformManager::GetInstance()->ySpeedOfMovingPlatform));
			m_canJump = true;
		}
	}
}

void Player::MoveLeft()
{
	if (KeyBoardInput::GetInstance()->isKeyPressed(SDLK_a))
	{
		if (!collidingWithMovingPlat)
		{
			m_body->SetLinearVelocity(b2Vec2(-2, m_body->GetLinearVelocity().y));
		}
		else
		{
			m_body->SetLinearVelocity(b2Vec2(PlatformManager::GetInstance()->xSpeedOfMovingPlatform - 2, PlatformManager::GetInstance()->ySpeedOfMovingPlatform));
		}

		// Change sprite image if not already moving left
		if (!m_movingLeft)
		{
			m_source = { 0, 0, 912, 107 };
			m_playerRunningSprite->Init("Images/Player/PlayerRunningLeft.png", m_rect, m_source);
			m_playerRunningSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });

			m_source = { 0, 0, 414, 108 };
			m_playerRunningMGSprite->Init("Images/Player/PlayerMachineGunRunningSpriteLeft.png", m_rect, m_source);
			m_playerRunningMGSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });

			m_movingLeft = true;
			m_movingRight = false;
		}

		// Change booleans
		m_running = true;
		m_idle = false;
		m_drawn = false;

		// Increase running frames
		if (!m_machineGunEquipped)
		{
			m_runningAnimationTime++;
			if (m_runningAnimationTime >= m_runningAnimationLimit)
			{
				++m_runningFrames;
				m_runningAnimationTime = 0;
			}
		}
		else
		{
			m_runningMGAnimationTime++;
			if (m_runningMGAnimationTime >= m_runningMGAnimationLimit)
			{
				++m_runningMGFrames;
				m_runningMGAnimationTime = 0;
			}
		}
	}
}

void Player::MoveRight()
{
	if (KeyBoardInput::GetInstance()->isKeyPressed(SDLK_d))// 1
	{
		if (!collidingWithMovingPlat)// 2
		{
			m_body->SetLinearVelocity(b2Vec2(2, m_body->GetLinearVelocity().y));// 3
		}
		else
		{
			// 4
			m_body->SetLinearVelocity(b2Vec2(PlatformManager::GetInstance()->xSpeedOfMovingPlatform + 2, PlatformManager::GetInstance()->ySpeedOfMovingPlatform));
		}// 5

		// Change sprite image if not already moving right
		if (!m_movingRight)// 6
		{
			// 7
			//if (!m_machineGunEquipped)
			//{
			m_source = { 0, 3, 912, 107 };
			m_playerRunningSprite->Init("Images/Player/PlayerRunningRight.png", m_rect, m_source);
			m_playerRunningSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
			//}
			//else
			//{
			m_source = { 0, 0, 414, 108 };
			m_playerRunningMGSprite->Init("Images/Player/PlayerMachineGunRunningSpriteRight.png", m_rect, m_source);
			m_playerRunningMGSprite->SetOffset(SDL_Point{ m_rect.w / 2, m_rect.h / 2 });
			
			m_movingLeft = false;
			m_movingRight = true;
		}// 8

		// Change booleans
		// 9
		m_running = true;
		m_idle = false;
		m_drawn = false;

		// Increase running frames
		if (!m_machineGunEquipped)// 10
		{
			m_runningAnimationTime++;// 11
			if (m_runningAnimationTime >= m_runningAnimationLimit)// 12
			{
				// 13
				++m_runningFrames;
				m_runningAnimationTime = 0;
			}// 14
		}
		else
		{
			// 15
			m_runningMGAnimationTime++;
			if (m_runningMGAnimationTime >= m_runningMGAnimationLimit)// 16
			{
				// 17
				++m_runningMGFrames;
				m_runningMGAnimationTime = 0;
			}//18
		}// 19
	}// 20
}

void Player::Jump()
{
	if (!collidingWithMovingPlat)
	{
		float impulse = -1 * 6.0f;
		m_body->ApplyLinearImpulse(b2Vec2(0, impulse), m_body->GetWorldCenter(), true);
	}
	else
	{
		m_body->SetLinearVelocity(b2Vec2{ m_body->GetLinearVelocity().x, 0 });
		float impulse = -1 * 6.0f;
		m_body->ApplyLinearImpulse(b2Vec2(0, impulse), m_body->GetWorldCenter(), true);
	}
}

void Player::Shoot()
{
	SDL_Texture* m_bulletTexture = Sprite::loadTexture("Images/Player/Bullet.png", Renderer::GetInstance()->Get_SDL_RENDERER());
	SDL_Rect m_bulletSource = { 0, 0, 63, 63 };
	SDL_Rect m_bulletPos;

	if (m_movingRight)
	{
		if (!m_machineGunEquipped)
			m_bulletPos = { m_rect.x + m_rect.w - 22 * playerScale, m_rect.y - 21 * playerScale, 8 * playerScale, 8 * playerScale };
		else
			m_bulletPos = { m_rect.x + m_rect.w - 20 * playerScale, m_rect.y - 14 * playerScale, 7 * playerScale, 7 * playerScale };
	}
	else
	{
		if (!m_machineGunEquipped)
			m_bulletPos = { m_rect.x - 26 * playerScale, m_rect.y - 21 * playerScale, 8 * playerScale, 8 * playerScale };
		else
			m_bulletPos = { m_rect.x - 27 * playerScale, m_rect.y - 14 * playerScale, 7 * playerScale, 7 * playerScale };
	}

	Bullet* bullet = new Bullet(m_bulletTexture, m_bulletPos, world, m_bulletSource, m_movingRight);

	m_bullets.push_back(bullet);
}

void Player::Reset()
{
	// Reset variables
	m_lives -= 1;
	m_body->SetTransform(b2Vec2(m_startRect.x, m_startRect.y), 0);
	m_alive = true;
	m_reachedTeleporter = false;
	m_stopBloodAnimation = false;
	collidingWithMovingPlat = false;
	m_health = OldHealth;
	m_score = OldScore;

	m_body->SetLinearVelocity(b2Vec2(0, m_body->GetLinearVelocity().y - 0.000001f));

	// Reset machine gun
	m_hasMachineGun = oldPickedUp;
	m_machineGunEquipped = oldPickedUp;
	m_machineGunAmmo = OldMachineGunAmmo;
}

void Player::LevelComplete()
{
	// Reset variables
	m_body->SetTransform(b2Vec2(m_startRect.x, m_startRect.y), 0);
	OldScore = m_score;
	OldHealth = m_health;
	OldMachineGunAmmo = m_machineGunAmmo;
	oldPickedUp = m_hasMachineGun;
	m_alive = true;
	m_reachedTeleporter = false;
	m_stopBloodAnimation = false;
}

void Player::FinishedLevel()
{
	// Reset variables
	m_body->SetTransform(b2Vec2(m_startRect.x, m_startRect.y), 0);
	m_alive = true;
	m_reachedTeleporter = false;
}

#pragma region Getters/Setters
b2Body *Player::getBody()
{
	return m_body;
}
int Player::GetScore()
{
	return m_score;
}
void Player::SetScore(int myScore)
{
	m_score = myScore;
}
int Player::GetHealth()
{
	return m_health;
}
void Player::SetHealth(int myHealth)
{
	m_health = myHealth;
}
bool Player::GetAlive()
{
	return m_alive;
}
void Player::SetAlive(bool myAlive)
{
	m_alive = myAlive;
}
SDL_Rect Player::GetPosition()
{
	return m_rect;
}
bool Player::GetReachedTeleporter()
{
	return m_reachedTeleporter;
}
void Player::SetReachedTeleporter(bool myReachedTeleporter)
{
	m_reachedTeleporter = myReachedTeleporter;
}
int Player::GetLives()
{
	return m_lives;
}
void Player::SetLives(int myLives)
{
	m_lives = myLives;
}
bool Player::MachineGunPickedUp()
{
	return oldPickedUp;
}
void Player::SetMachineGunPickedUp(bool myMachineGun)
{
	oldPickedUp = myMachineGun;
}
bool Player::MachineGunEquipped()
{
	return m_machineGunEquipped;
}
int Player::GetMachineGunAmmo()
{
	return m_machineGunAmmo;
}
void Player::SetMachineGunAmmo(int myMachineGunAmmo)
{
	m_machineGunAmmo = myMachineGunAmmo;
}
std::vector<int> Player::GetPlatformOn()
{
	return platformOn;
}
#pragma endregion



