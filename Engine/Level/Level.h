#pragma once

#include "Core.h"
#include "RTTI.h"
#include "Math/Vector2.h"

#include <vector>

class Actor;

class ENGINE_API Level : public RTTI
{
	RTTI_DECLARATIONS(Level, RTTI)

public:
	Level();
	virtual ~Level();

	// 레벨에 액터를 추가하는 함수
	void AddActor(Actor* newActor);

	// 액터 삭제 함수
	void DestroyActor(Actor* destroyedActor);

	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void Render();

	// 플레이어 위치 Getter/Setter
	FORCEINLINE Vector2 GetPlayerPos() const { return playerPos; }
	FORCEINLINE void SetPlayerPos(const Vector2& newPosition) { playerPos = newPosition; }

	// 플레이어 시야 반경 Getter/Setter
	FORCEINLINE float GetPlayerVisionWidth() const { return playerVisionWidth; }
	FORCEINLINE void SetPlayerVisionWidth(const float newPlayerVisionWidth)
	{
		playerVisionWidth = newPlayerVisionWidth;
	}

	FORCEINLINE float GetPlayerVisionHeight() const { return playerVisionHeight; }
	FORCEINLINE void SetPlayerVisionHeight(const float newPlayerVisionHeight)
	{
		playerVisionHeight = newPlayerVisionHeight;
	}

	// GlobalVision Getter/Setter
	FORCEINLINE bool GetEnableGlobalVision() const { return bEnableGlobalVision; }
	FORCEINLINE void SetEnableGlobalVision(const bool inEnableGlobalVision)
	{
		bEnableGlobalVision = inEnableGlobalVision;
	}

	// 추가 및 삭제 요청된 액터를 처리하는 함수
	void ProcessAddAndDestroyActors();

	// 스테이지를 클리어했는지 확인하는 함수
	bool StageClear();

	// 플레이어가 사망했는지 확인하는 함수
	bool GameOver();

protected:
	// 렌더해도 되는 액터인지 판별하는 함수
	bool ShouldRenderActor(Actor* inActor);

	// 플레이어 시야 내에 있는지 검사하는 함수
	bool IsWithinEllipticalZone(const Vector2& actorPos, const Vector2& playerPos) const;

protected:
	// 레벨에 배치된 모든 액터를 관리하는 배열
	std::vector<Actor*> actors;

	// 추가 요청된 액터를 관리하는 배열
	std::vector<Actor*> addRequstedActors;

	// 삭제 요청된 액터를 관리하는 배열
	std::vector<Actor*> destroyRequstedActors;

protected:
	// 플레이어의 위치
	Vector2 playerPos = Vector2::Zero;

	// 플레이어의 시야 (보이는 거리)
	float playerVisionWidth = 4.0f;
	float playerVisionHeight = 3.0f;

	// GlobalVision 활성화 여부
	bool bEnableGlobalVision = false;

	// 스테이지 클리어 여부
	bool IsStageClear = false;

	// 플레이어의 죽음 여부
	bool isPlayerDead = false;
};

