#include "Enemy.h"
#include "Utils/Utils.h"
#include "Level/Level.h"
#include "Interface/GameLevelInterface.h"

#include <iostream>

Enemy::Enemy(const Vector2 position)
	:super("M", Color::Violet, position)
{
	SetSortingOrder(SortingOrder::Enemy);

	// Ÿ�̸� ���� (�� �̵� �ð� ����)
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

	// �� �̵�
	enemyMoveTimer.Tick(deltaTime);

	if (!enemyMoveTimer.IsTimeout())
	{
		return;
	}

	// Ÿ�̸� ����
	enemyMoveTimer.Reset();
	enemyMoveTimer.SetTargetTime(Utils::RandomFloat(0.1f, 0.3f));

	// ���� �̵� ���� ���ϱ�
	MoveDirection moveDirection = static_cast<MoveDirection>(Utils::Random(1, 4));

	// �̵� ���� ��
	Vector2 moveValue = Vector2::Zero;

	// ���� ��ġ
	Vector2 position = Position();

	// �ش� �������� �̵��� �� �ִ��� �˻�
	// �ȵǸ� �ٽ� ���� �̵� ���� ���ϱ�
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
