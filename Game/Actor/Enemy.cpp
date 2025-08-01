#include "Enemy.h"

Enemy::Enemy(const Vector2 position)
	:super('M', Color::Violet, position)
{
}

void Enemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);

}