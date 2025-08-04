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
		class Actor* inActor,
		const Vector2& currentPosition,
		const Vector2& nextPosition) override;

private:
	void ReadStageFile(const char* fileName);

	// 남은 스코어 출력 함수
	void PrintScore();

	// 플레이어와 점수의 처리
	void ProcessPlayerAndScore(Actor* inScore);

	// 플레이어와 적의 처리
	void ProcessPlayerAndEnemy(Actor* inPlayer);

private:
	// 클리어까지 남은 점수(스코어)
	int remainingScore = 0;

	// 맵의 x, y값 좌표
	Vector2 stagePos = Vector2::Zero;

	// 플레이어의 죽음 처리를 위한 변수
	bool isPlayerDead = false;

	// Todo: 암흑 시야 on/off 옵션
	bool useDark = false;
};