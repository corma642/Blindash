#pragma once

#include "Actor/Actor.h"

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player(const Vector2& position);

	virtual void Tick(float deltaTime) override;

private:
	// �÷��̾� �̵� �ӵ� (����: �ʴ� ����)
	float xMoveSpeed = 10.0f;
	float yMoveSpeed = 5.0f;

	// ��ǥ�� �����̱� ������ ���� �� ����� ���� float ���� ����
	float xPos = 0.0f;
	float yPos = 0.0f;
};
