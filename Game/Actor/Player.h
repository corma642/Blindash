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

	// 플레이어 GlobalVision Getter/Setter
	FORCEINLINE void StartGlobalVision() { bStartGlobalVision = true; }
	FORCEINLINE bool GetEnableGlobalVision() const { return bEnableGlobalVision; }
	FORCEINLINE void SetEnableGlobalVision(bool inGlobalVision) { bEnableGlobalVision = inGlobalVision; }

	// 플레이어 슈퍼 모드 Getter/Setter
	FORCEINLINE void StartSuperMode() { bStartSuperMode = true; }
	FORCEINLINE bool GetEnableSuperMode() const { return bEnableSuperMode; }
	FORCEINLINE void SetEnableSuperMode(bool inSuperMode) { bEnableSuperMode = inSuperMode; }

public:
	// 플레이어의 시야 반경을 영구적으로 약간 증가시키는 함수
	void ItemExpandVisionRange();

	// n초간 암흑이 모두 걷혀, 맵 전체가 다 보이도록 하는 함수
	void ItemActivateGlobalVision(float deltaTime);

	// n초간 암흑이 모두 걷히고, 플레이어가 무적이 되며, 몬스터또한 처치 가능한 함수
	void ItemActivateSuperMode(float deltaTime);

private:
	// 이동 입력 처리
	void PlayerMovement(float deltaTime);

	// 수평 이동 (X축) / sign true = +, false = -
	void HorizontalMove(bool sign, bool isRepeat);

	// 수직 이동 (Y축) / sign true = +, false = -
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


	// 플레이어 GlobalVision 상태 변수
	Timer GlobalVisionTimer;
	bool bStartGlobalVision = false;
	bool bEnableGlobalVision = false;
	float GlobalVisionTime = 3.0f;

	// 플레이어 슈퍼 모드 상태 변수
	Timer SuperModeTimer;
	bool bStartSuperMode = false;
	bool bEnableSuperMode = false;
	float SuperModeTime = 3.0f;
};
