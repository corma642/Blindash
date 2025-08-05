#include "Actor.h"
#include "Utils/Utils.h"
#include "Engine.h"
#include "Level/Level.h"

#include <Windows.h>
#include <iostream>

Actor::Actor(const char* image, Color color, const Vector2& position)
	: color(color), position(position)
{
	width = static_cast<int>(strlen(image));

	this->image = new char[width + 1];
	
	strcpy_s(this->image, width + 1, image);
}

Actor::~Actor()
{
	SafeDeleteArray(image);
}

void Actor::BeginPlay()
{
	hasBeganPlay = true;
}

void Actor::Tick(float deltaTiem)
{
}

void Actor::Render()
{
	Engine::Get().WriteToBuffer(position, image, color, static_cast<int>(sortingOrder));
}

void Actor::SetPosition(const Vector2& newPosition)
{
	// ���� ��ġ�� ������Ʈ ����
	if (position == newPosition)
	{
		return;
	}

	// ��ġ ����
	position = newPosition;
}

void Actor::Destroy()
{
	// ���� ���� ����
	if (isExpired)
	{
		return;
	}

	// ���� ��û�Ǿ��ٰ� ����
	isExpired = true;
	owner->DestroyActor(this);
}

void Actor::QuitGame()
{
	Engine::Get().Quit();
}
