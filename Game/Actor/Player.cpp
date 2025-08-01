#include "Player.h"

Player::Player(const Vector2& position)
	: super('D', Color::Yellow, position)
{
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

}
