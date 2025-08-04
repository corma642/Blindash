#include "Wall.h"

Wall::Wall(const Vector2& position)
	: super("8", Color::SkyBlue, position)
{
	SetSortingOrder(SortingOrder::Wall);
}
