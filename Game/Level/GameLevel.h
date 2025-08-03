#pragma once

#include "Level/Level.h"
#include "Interface/GameLevelInterface.h"

class GameLevel : public Level, public IGameLevelInterface
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();

	virtual void Render() override;

	// 이동 가능 여부를 반환하는 함수
	virtual bool CanMove(
		const class Actor* inActor,
		const Vector2& currentPosition,
		const Vector2& nextPosition) override;

private:
	void ReadStageFile(const char* fileName);
};

