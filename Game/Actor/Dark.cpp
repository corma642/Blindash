#include "Dark.h"
#include "Level/Level.h"
#include "Actor/Effect/ScoreDestroyEffect.h"

Dark::Dark(const Vector2& position)
	: super("-", Color::Intensity, position)
{
	SetSortingOrder(SortingOrder::Dark);
}
