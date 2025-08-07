#pragma once

#include "Level/Level.h"
#include "Interface/GameLevelInterface.h"
#include "Utils/Timer.h"

class GameLevel : public Level, public IGameLevelInterface
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel(const int stageNumber, const float timeLimit);

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	// 이동 가능 여부를 반환하는 함수
	virtual bool CanMove(
		class Actor* inActor,
		const Vector2& currentPosition,
		const Vector2& nextPosition) override;

	// 플레이어 시야 반경 설정 함수
	virtual void SetPlayerVisionRadius(float newVisionWidth, float newVisionHeight) override;

	// 플레이어의 시야 반경이 늘어난 외곽 테두리 타일 구하는 함수
	void CalculateVisionRing();

private:
	void ReadStageFile(const char* fileName);

	// 남은 스코어 출력 함수
	void PrintRemainingScore();

	// 남은 제한 시간 출력 함수
	void PrintTimeLimit();

	// 플레이어와 점수의 처리
	void ProcessPlayerAndScore(Actor* inPlayer, Actor* inScore);

	// 플레이어와 적의 처리
	void ProcessPlayerAndEnemy(Actor* inPlayer, Actor* inEnemy);

private:
	// 클리어까지 남은 점수
	int remainingScore = 0;

	// 스테이지 제한 시간
	float timeLimit = 0.0f;

	// 현재 스테이지 레벨
	int currnetStage = 0;
};