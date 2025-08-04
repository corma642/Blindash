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
	// �÷��̾� ���� �̵� (X��) / sign true = +, false = -
	void HorizontalMove(bool sign, bool isRepeat);

	// �÷��̾� ���� �̵� (Y��) / sign true = +, false = -
	void VerticalMove(bool sign, bool isRepeat);

private:
	// �ݺ�/���� �̵� Ÿ�̸�
	Timer playerRepeatMoveXTimer;
	Timer playerRepeatMoveYTimer;

	// �ݺ�/���� �̵� �Ӱ谪
	float xMoveThreshold = 0.15f;
	float yMoveThreshold = 0.3f;

	// ���� ���� �������̽�
	class IGameLevelInterface* gameLevelInterface = nullptr;
};
