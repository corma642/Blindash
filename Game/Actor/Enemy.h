#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"
#include "Math/Vector2.h"

#include <utility>

enum class MoveDirection : unsigned __int8
{
	None = 0,
	Up,
	Right,
	Down,
	Left,
};

class Enemy : public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)

public:
	Enemy(const Vector2 position);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime);

private:
	// 랜덤 이동 방향 구하는 함수
	void GetRandomMoveDirection(MoveDirection& inMoveDirection, Vector2& inMoveValue);

	// 이전에 이동한 방향으로 이동했는지 검사하는 함수
	bool IsPrevMove(MoveDirection moveDirection);

private:
	// 이동 대기 타이머
	Timer enemyMoveTimer;

	// 이전에 이동한 방향
	MoveDirection prevMoveDirection = MoveDirection::None;

	// 게임 레벨 인터페이스
	class IGameLevelInterface* gameLevelInterface = nullptr;
};

