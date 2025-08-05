#include "Wall.h"

Wall::Wall(const Vector2& position)
	: super("8", Color::Blue, position)
{
	SetSortingOrder(SortingOrder::Wall);
}
