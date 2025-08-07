#include "Score.h"
#include "Level/Level.h"
#include "Actor/Effect/ScoreDestroyEffect.h"

Score::Score(const Vector2& position)
	: super(".", Color::White, position)
{
	SetSortingOrder(SortingOrder::Score);
}

void Score::OnDestroy()
{
	super::OnDestroy();

	owner->AddActor(new ScoreDestroyEffect(position));
}
