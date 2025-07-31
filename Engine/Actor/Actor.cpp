#include "Actor.h"
#include "Utils/Utils.h"
#include "Engine.h"

#include <Windows.h>
#include <iostream>

Actor::Actor(const char image, Color color, const Vector2& position)
	: image(image), color(color), position(position)
{
}

Actor::~Actor()
{
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
	// Ŀ�� �̵�
	Utils::SetConsolePosition(position);

	// �ؽ�Ʈ ���� ����
	Utils::SetConsoleTextColor(color);

	// �׸���
	std::cout << image;
}

void Actor::SetPosition(const Vector2& newPosition)
{
	if (position == newPosition)
	{
		return;
	}

	// Ŀ�� �̵�
	Utils::SetConsolePosition(position);

	std::cout << ' ';

	position = newPosition;
}

Vector2 Actor::Position() const
{
	return position;
}

void Actor::SetSortingOrder(SortingOrder sortingOrder)
{
	this->sortingOrder = sortingOrder;
}

void Actor::SetOwner(Level* newOwner)
{
	if (newOwner)
	{
		owner = newOwner;
	}
}

Level* Actor::GetOwner() const
{
	return owner;
}

void Actor::QuitGame()
{
	Engine::Get().Quit();
}
