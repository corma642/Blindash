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

	// �÷��̾� ���� ��� Getter/Setter
	FORCEINLINE void StartSuperMode() { bStartSuperMode = true; }
	FORCEINLINE bool GetEnableSuperMode() const { return bEnableSuperMode; }
	FORCEINLINE void SetEnableSuperMode(bool inSuperMode) { bEnableSuperMode = inSuperMode; }

public:
	// �÷��̾��� �þ� �ݰ��� ���������� �ణ ������Ű�� �Լ�
	void ItemExpandVisionRange();

	// n�ʰ� ������ ��� ������, �÷��̾ ������ �Ǹ�, ���Ͷ��� óġ ������ �Լ�
	void ItemActivateSuperMode(float deltaTime);

private:
	// �̵� �Է� ó��
	void PlayerMovement(float deltaTime);

	// ���� �̵� (X��) / sign true = +, false = -
	void HorizontalMove(bool sign, bool isRepeat);

	// ���� �̵� (Y��) / sign true = +, false = -
	void VerticalMove(bool sign, bool isRepeat);

private:
	// �ݺ�/���� �̵� Ÿ�̸�
	Timer playerRepeatMoveXTimer;
	Timer playerRepeatMoveYTimer;

	// �ݺ�/���� �̵� �Ӱ谪
	float xMoveThreshold = 0.1f;
	float yMoveThreshold = 0.2f;

	// ���� ���� �������̽�
	class IGameLevelInterface* gameLevelInterface = nullptr;

	// �÷��̾� ���� ��� ���� ����
	Timer SuperModeTimer;
	bool bStartSuperMode = false;
	bool bEnableSuperMode = false;
	float SuperModeTime = 3.0f;
};
