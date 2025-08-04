#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player(const Vector2& position);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

private:
	// 플레이어 수평 이동 (X축) / sign true = +, false = -
	void HorizontalMove(bool sign, bool isRepeat);

	// 플레이어 수직 이동 (Y축) / sign true = +, false = -
	void VerticalMove(bool sign, bool isRepeat);

private:
	// 반복/연속 이동 타이머
	Timer playerRepeatMoveXTimer;
	Timer playerRepeatMoveYTimer;

	// 반복/연속 이동 임계값
	float xMoveThreshold = 0.15f;
	float yMoveThreshold = 0.3f;

	// 게임 레벨 인터페이스
	class IGameLevelInterface* gameLevelInterface = nullptr;
};
