#include "Dark.h"

Dark::Dark(const Vector2& position)
	: super("-", Color::Intensity, position)
{
	SetSortingOrder(SortingOrder::Dark);
}
