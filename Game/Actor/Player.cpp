#include "Player.h"
#include "Input.h"
#include "Engine.h"

#include <iostream>

Player::Player(const Vector2& position)
	: super('D', Color::Yellow, position)
{
	SetSortingOrder(SortingOrder::Player);
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (Input::Get().GetKey(VK_ESCAPE))
	{
		Engine::Get().Quit();
		return;
	}

	if (Input::Get().GetKey('A')) xPos -= xMoveSpeed * deltaTime;
	if (Input::Get().GetKey('D')) xPos += xMoveSpeed * deltaTime;
	if (Input::Get().GetKey('W')) yPos -= yMoveSpeed * deltaTime;
	if (Input::Get().GetKey('S')) yPos += yMoveSpeed * deltaTime;

	Vector2 position = Position();
	position.x = static_cast<int>(xPos);
	position.y = static_cast<int>(yPos);

	SetPosition(position);
}
