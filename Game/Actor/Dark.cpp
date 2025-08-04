#include "Dark.h"

Dark::Dark(const Vector2& position)
	: super("=", Color::Blue, position)
{
	SetSortingOrder(SortingOrder::Dark);
}
