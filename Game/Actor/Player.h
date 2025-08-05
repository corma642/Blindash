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

	// �÷��̾� GlobalVision Getter/Setter
	FORCEINLINE void StartGlobalVision() { bStartGlobalVision = true; }
	FORCEINLINE bool GetEnableGlobalVision() const { return bEnableGlobalVision; }
	FORCEINLINE void SetEnableGlobalVision(bool inGlobalVision) { bEnableGlobalVision = inGlobalVision; }

	// �÷��̾� ���� ��� Getter/Setter
	FORCEINLINE void StartSuperMode() { bStartSuperMode = true; }
	FORCEINLINE bool GetEnableSuperMode() const { return bEnableSuperMode; }
	FORCEINLINE void SetEnableSuperMode(bool inSuperMode) { bEnableSuperMode = inSuperMode; }

public:
	// �÷��̾��� �þ� �ݰ��� ���������� �ణ ������Ű�� �Լ�
	void ItemExpandVisionRange();

	// n�ʰ� ������ ��� ����, �� ��ü�� �� ���̵��� �ϴ� �Լ�
	void ItemActivateGlobalVision(float deltaTime);

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
	float xMoveThreshold = 0.15f;
	float yMoveThreshold = 0.3f;

	// ���� ���� �������̽�
	class IGameLevelInterface* gameLevelInterface = nullptr;


	// �÷��̾� GlobalVision ���� ����
	Timer GlobalVisionTimer;
	bool bStartGlobalVision = false;
	bool bEnableGlobalVision = false;
	float GlobalVisionTime = 3.0f;

	// �÷��̾� ���� ��� ���� ����
	Timer SuperModeTimer;
	bool bStartSuperMode = false;
	bool bEnableSuperMode = false;
	float SuperModeTime = 3.0f;
};
