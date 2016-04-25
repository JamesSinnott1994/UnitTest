#include "stdafx.h"
#include "ElectricSwitch.h"

ElectricSwitch::ElectricSwitch(){}

ElectricSwitch::ElectricSwitch(SDL_Texture* text, SDL_Rect pRect, b2World *wWorld, SDL_Rect src, int id)
{
	onTexture = Sprite::loadTexture("Images/Obstacles/OnSwitch.png", Renderer::GetInstance()->Get_SDL_RENDERER());
	offTexture = Sprite::loadTexture("Images/Obstacles/OffSwitch.png", Renderer::GetInstance()->Get_SDL_RENDERER());

	myRect = pRect;

	// Define the ground body.
	myBodyDef.position.Set(pRect.x, pRect.y);
	myBodyDef.type = b2_staticBody;

	// Define the ground box shape.
	// The extents are the half-widths of the box.
	myShape.SetAsBox(pRect.w / 2, pRect.h / 2);

	myBodyFixtureDef.shape = &myShape;

	// The body is also added to the world.
	myBody = wWorld->CreateBody(&myBodyDef);

	// Add the ground fixture to the ground body.
	myBody->CreateFixture(&myBodyFixtureDef);
	sprite.Init(text, pRect, src);
	sprite.SetOffset(SDL_Point{ pRect.w / 2, pRect.h / 2 });

	m_on = true;
	hasBarrier = false;

	m_id = id;
}

void ElectricSwitch::Draw()
{
	sprite.Draw(1);
}

void ElectricSwitch::Update()
{

}

void ElectricSwitch::Reset()
{
	m_on = true;
	SDL_Rect m_source = { 0, 0, 180, 216 };
	sprite.Init("Images/Obstacles/OnSwitch.png", myRect, m_source);
	sprite.SetOffset(SDL_Point{ myRect.w / 2, myRect.h / 2 });
	myBody->SetActive(true);
}

void ElectricSwitch::Destroy()
{
	myBody->GetWorld()->DestroyBody(myBody);
}

bool ElectricSwitch::CheckCollision(b2Body* playerBody)
{
	bool collided = (b2TestOverlap(myBody->GetFixtureList()->GetAABB(0), playerBody->GetFixtureList()->GetAABB(0)));
	if (collided && m_on)
	{
		SDL_Rect m_source = { 0, 0, 180, 216 };
		sprite.Init("Images/Obstacles/OffSwitch.png", myRect, m_source);
		sprite.SetOffset(SDL_Point{ myRect.w / 2, myRect.h / 2 });
		SoundManager::GetInstance()->play(SoundManager::GetInstance()->ELECTROCUTED);
		m_on = false;
	}
	return collided;
}

bool ElectricSwitch::GetOn()
{
	return m_on;
}

void ElectricSwitch::SetOn(bool myOn)
{
	m_on = myOn;
}

int ElectricSwitch::GetSwitchID()
{
	return m_id;
}

void ElectricSwitch::SetSwitchID(int myID)
{
	m_id = myID;
}