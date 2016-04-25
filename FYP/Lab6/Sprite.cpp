#include "stdafx.h"
#include <Sprite.h>


Sprite::Sprite()
{
	m_offset = {0,0};
	rotation = 0;
}

void Sprite::Init(std::string imagePath, SDL_Rect destination, SDL_Rect source)
{
	myTexture = loadTexture(imagePath, Renderer::GetInstance()->Get_SDL_RENDERER());
	m_Destrination = destination;
	m_Source = source;
}

void Sprite::Init(SDL_Texture* imageText, SDL_Rect destination, SDL_Rect source)
{
	myTexture = imageText;
	m_Destrination = destination;
	m_Source = source;
}

void Sprite::SetPosition(float x, float y)
{ 
	m_Destrination.x = x;
	m_Destrination.y = y;
}

SDL_Rect Sprite::GetPosition()
{
	return m_Destrination;
}

void Sprite::SetWidth(float w)
{
	m_Destrination.w = w; 
}

void Sprite::SetHeight(float h)
{
	m_Destrination.h = h;
}

void Sprite::SetDestinationRect(SDL_Rect destination)
{
	m_Destrination = destination;
}

void Sprite::SetSourceRect(SDL_Rect source)
{ 
	m_Source = source;
}

void Sprite::SetOffset(SDL_Point offset)
{
	m_offset = offset;
}

void Sprite::SetRotation(double rot)
{
	rotation = rot;
}

void Sprite::Draw(float zIndex)
{
	SDL_Rect newDest{ 
		m_Destrination.x - m_offset.x, 
		m_Destrination.y - m_offset.y,
		m_Destrination.w, 
		m_Destrination.h 
	};
	Renderer::GetInstance()->DrawImage(&m_Source, &newDest, myTexture, rotation, &m_offset, zIndex);
}

void Sprite::DrawNoCamOffset()
{
	SDL_Rect newDest{ m_Destrination.x - m_offset.x, m_Destrination.y - m_offset.y, m_Destrination.w, m_Destrination.h };
	Renderer::GetInstance()->DrawImageNoOffset(&m_Source, &newDest, myTexture, rotation, &m_offset);
}

SDL_Texture* Sprite::loadTexture(std::string path, SDL_Renderer* gRenderer)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL){
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}


		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}


	return newTexture;
}

SDL_Rect Sprite::getDestRect(){
	return m_Destrination;
}