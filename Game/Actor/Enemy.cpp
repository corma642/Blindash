#include "Enemy.h"
#include "Utils/Utils.h"
#include "Level/Level.h"
#include "Interface/GameLevelInterface.h"

#include <iostream>

Enemy::Enemy(const Vector2 position)
	:super("M", Color::Violet, position)
{
	SetSortingOrder(SortingOrder::Enemy);

	// 타이머 설정 (적 이동 시간 설정)
	enemyMoveTimer.SetTargetTime(Utils::RandomFloat(0.1f, 0.3f));
}

void Enemy::BeginPlay()
{
	super::BeginPlay();

	if (GetOwner())
	{
		gameLevelInterface = dynamic_cast<IGameLevelInterface*>(GetOwner());
	}

	if (!gameLevelInterface)
	{
		std::cout << "Enemy: Can not cast to owner level to ICanPlayerMove" << "\n";
	}
}

void Enemy::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 적 이동
	enemyMoveTimer.Tick(deltaTime);

	if (!enemyMoveTimer.IsTimeout())
	{
		return;
	}

	// 타이머 정리
	enemyMoveTimer.Reset();
	enemyMoveTimer.SetTargetTime(Utils::RandomFloat(0.1f, 0.3f));

	// 랜덤 이동 방향 구하기
	MoveDirection moveDirection = static_cast<MoveDirection>(Utils::Random(1, 4));

	// 이동 벡터 값
	Vector2 moveValue = Vector2::Zero;

	// 현재 위치
	Vector2 position = Position();

	// 해당 방향으로 이동할 수 있는지 검사
	// 안되면 다시 랜덤 이동 방향 구하기
	while (true)
	{
		if (IsPrevMove(moveDirection))
		{
			GetRandomMoveDirection(moveDirection, moveValue);
			continue;
		}

		if (gameLevelInterface->CanMove(this, position, position + moveValue))
		{
			break;
		}
		else
		{
			GetRandomMoveDirection(moveDirection, moveValue);
		}
	}

	prevMoveDirection = moveDirection;
	SetPosition(position + moveValue);
}

void Enemy::GetRandomMoveDirection(MoveDirection& inMoveDirection, Vector2& inMoveValue)
{
	int index = Utils::Random(1, 4);

	switch (index)
	{
	case 1:
		inMoveDirection = MoveDirection::Up;
		inMoveValue.x = 0;
		inMoveValue.y = Utils::Random(-1, -3);
		break;
	case 2:
		inMoveDirection = MoveDirection::Right;
		inMoveValue.x = Utils::Random(1, 3);
		inMoveValue.y = 0;
		break;
	case 3:
		inMoveDirection = MoveDirection::Down;
		inMoveValue.x = 0;
		inMoveValue.y = Utils::Random(1, 3);
		break;
	case 4:
		inMoveDirection = MoveDirection::Left;
		inMoveValue.x = Utils::Random(-1, -3);
		inMoveValue.y = 0;
		break;
	}
}

bool Enemy::IsPrevMove(MoveDirection moveDirection)
{
	switch (moveDirection)
	{
	case MoveDirection::Up:
		if (prevMoveDirection == MoveDirection::Down) return true;
		else return false;

	case MoveDirection::Right:
		if (prevMoveDirection == MoveDirection::Left) return true;
		else return false;

	case MoveDirection::Down:
		if (prevMoveDirection == MoveDirection::Up) return true;
		else return false;

	case MoveDirection::Left:
		if (prevMoveDirection == MoveDirection::Right) return true;
		else return false;
	}

	return true;
}
