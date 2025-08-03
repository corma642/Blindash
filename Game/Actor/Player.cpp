#include "Player.h"
#include "Input.h"
#include "Engine.h"
#include "Level/Level.h"
#include "Interface/GameLevelInterface.h"

#include <iostream>

Player::Player(const Vector2& position)
	: super('D', Color::Yellow, position)
{
	SetSortingOrder(SortingOrder::Player);
}

void Player::BeginPlay()
{
	super::BeginPlay();

	if (GetOwner())
	{
		gameLevelInterface = dynamic_cast<IGameLevelInterface*>(GetOwner());
	}

	if (!gameLevelInterface)
	{
		std::cout << "Can not cast to owner level to ICanPlayerMove" << "\n";
	}
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (Input::Get().GetKey(VK_ESCAPE))
	{
		Engine::Get().Quit();
		return;
	}

	Vector2 position = Position();

	if (Input::Get().GetKey('A'))
	{
		PlayerMove(false, false, true, deltaTime);
	}
	if (Input::Get().GetKeyUp('A'))
	{
		PlayerMove(false, false, false, deltaTime);
	}

	if (Input::Get().GetKey('D'))
	{
		PlayerMove(false, true, true, deltaTime);
	}
	if (Input::Get().GetKeyUp('D'))
	{

		PlayerMove(false, true, false, deltaTime);
	}

	if (Input::Get().GetKey('W'))
	{
		PlayerMove(true, false, true, deltaTime);
	}
	if (Input::Get().GetKeyUp('W'))
	{
		PlayerMove(true, false, false, deltaTime);
	}

	if (Input::Get().GetKey('S'))
	{
		PlayerMove(true, true, true, deltaTime);
	}
	if (Input::Get().GetKeyUp('S'))
	{
		PlayerMove(true, true, false, deltaTime);
	}
}

void Player::PlayerMove(const bool xy, const bool sign, const bool sequence, float deltaTime)
{
	Vector2 position = Position();

	if (!sequence)
	{
		if (xy && sign) position.y++;
		else if (xy && !sign) position.y--;
		else if (!xy && sign) position.x++;
		else if (!xy && !sign) position.x--;

		if (gameLevelInterface->CanMove(this, Position(), position))
		{
			SetPosition(position);
		}

		xPos = 0.0f;
		yPos = 0.0f;
	}
	else
	{
		if (xy && sign) static_cast<int>(yPos += xMoveSpeed * deltaTime);
		else if (xy && !sign) static_cast<int>(yPos -= xMoveSpeed * deltaTime);
		else if (!xy && sign) static_cast<int>(xPos += xMoveSpeed * deltaTime);
		else if (!xy && !sign) static_cast<int>(xPos -= xMoveSpeed * deltaTime);

		position.x += static_cast<int>(xPos);
		position.y += static_cast<int>(yPos);

		if (gameLevelInterface->CanMove(this, Position(), position))
		{
			SetPosition(position);
		}
		else
		{
			if (xy && sign) static_cast<int>(yPos -= xMoveSpeed * deltaTime);
			else if (xy && !sign) static_cast<int>(yPos += xMoveSpeed * deltaTime);
			else if (!xy && sign) static_cast<int>(xPos -= xMoveSpeed * deltaTime);
			else if (!xy && !sign) static_cast<int>(xPos += xMoveSpeed * deltaTime);
		}
	}
}
