#include "Enemy.h"

Enemy::Enemy(const Vector2 position)
	:super('M', Color::Violet, position)
{
	SetSortingOrder(SortingOrder::Enemy);
}

void Enemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);

}