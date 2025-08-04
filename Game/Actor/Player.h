#pragma once

#include "Actor/Actor.h"

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player(const Vector2& position);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

private:
	// �÷��̾� �̵� ó�� �Լ�
	// xy(false) = x, xy(true) = y
	// sign(false) = -, sign(true) = +
	// sequence = ���� �Է� ���� Ȯ��
	void PlayerMove(const bool xy, const bool sign, const bool sequence, float deltaTime);

private:
	// �÷��̾� �̵� �ӵ� (����: �ʴ� ����)
	float xMoveSpeed = 7.0f;
	float yMoveSpeed = 3.0f;

	// ��ǥ�� �����̱� ������ ���� �� ����� ���� float ���� ����
	float xPos = 0.0f;
	float yPos = 0.0f;

	// ���� ���� �������̽�
	class IGameLevelInterface* gameLevelInterface = nullptr;
};
