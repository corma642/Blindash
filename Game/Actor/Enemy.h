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
	// ���� �̵� ���� ���ϴ� �Լ�
	void GetRandomMoveDirection(MoveDirection& inMoveDirection, Vector2& inMoveValue);

	// ������ �̵��� �������� �̵��ߴ��� �˻��ϴ� �Լ�
	bool IsPrevMove(MoveDirection moveDirection);

private:
	// �̵� ��� Ÿ�̸�
	Timer enemyMoveTimer;

	// ������ �̵��� ����
	MoveDirection prevMoveDirection = MoveDirection::None;

	// ���� ���� �������̽�
	class IGameLevelInterface* gameLevelInterface = nullptr;
};

