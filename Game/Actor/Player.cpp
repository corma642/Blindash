#include "Player.h"
#include "Input.h"
#include "Engine.h"

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

	if (Input::Get().GetKey('A'))
	{
		Vector2 position = Position();
		position.x--;
		SetPosition(position);
	}

	if (Input::Get().GetKey('D'))
	{
		Vector2 position = Position();
		position.x++;
		SetPosition(position);
	}

	if (Input::Get().GetKey('W'))
	{
		Vector2 position = Position();
		position.y--;
		SetPosition(position);
	}

	if (Input::Get().GetKey('S'))
	{
		Vector2 position = Position();
		position.y++;
		SetPosition(position);
	}
}
