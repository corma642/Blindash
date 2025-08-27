#include "Wall.h"

Wall::Wall(const Vector2& position)
	: super("8", Color::LightBlue, position)
{
	SetSortingOrder(SortingOrder::Wall);
}
