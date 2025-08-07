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

	// ������ ���͸� �߰��ϴ� �Լ�
	void AddActor(Actor* newActor);

	// ���� ���� �Լ�
	void DestroyActor(Actor* destroyedActor);

	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void Render();

	// �÷��̾� ��ġ Getter/Setter
	FORCEINLINE Vector2 GetPlayerPos() const { return playerPos; }
	FORCEINLINE void SetPlayerPos(const Vector2& newPosition) { playerPos = newPosition; }

	// �÷��̾� �þ� �ݰ� Getter
	FORCEINLINE float GetPlayerVisionWidth() const { return playerVisionWidth; }
	FORCEINLINE float GetPlayerVisionHeight() const { return playerVisionHeight; }
	
	// �÷��̾� �þ� �ݰ� ���� �Լ�
	virtual void SetPlayerVisionRadius(float newVisionWidth, float newVisionHeight);

	// GlobalVision Getter/Setter
	FORCEINLINE bool GetEnableGlobalVision() const { return bEnableGlobalVision; }
	FORCEINLINE void SetEnableGlobalVision(const bool inEnableGlobalVision)
	{
		bEnableGlobalVision = inEnableGlobalVision;
	}

	// �߰� �� ���� ��û�� ���͸� ó���ϴ� �Լ�
	void ProcessAddAndDestroyActors();

	// ���������� Ŭ�����ߴ��� Ȯ���ϴ� �Լ�
	bool StageClear();

	// ������ �й�� �������� Ȯ���ϴ� �Լ�
	bool GameOver();

protected:
	// �����ص� �Ǵ� �������� �Ǻ��ϴ� �Լ�
	bool ShouldRenderActor(Actor* inActor);

	// �÷��̾� �þ� ���� ���� �ִ��� �˻��ϴ� �Լ�
	bool IsWithinEllipticalZone(const Vector2& actorPos, const Vector2& playerPos) const;

	// �÷��̾� �þ� ���� 1ĭ ���� ���� �ִ��� �˻��ϴ� �Լ�
	bool IsWithinEllipticalZoneV2(const Vector2& actorPos, const Vector2& playerPos) const;

protected:
	// ������ ��ġ�� ��� ���͸� �����ϴ� �迭
	std::vector<Actor*> actors;

	// �߰� ��û�� ���͸� �����ϴ� �迭
	std::vector<Actor*> addRequstedActors;

	// ���� ��û�� ���͸� �����ϴ� �迭
	std::vector<Actor*> destroyRequstedActors;

	// ���� x, y�� ��ǥ
	Vector2 stagePos = Vector2::Zero;

protected:
	// �÷��̾��� ��ġ
	Vector2 playerPos = Vector2::Zero;

	// �÷��̾��� �þ� (���̴� �Ÿ�)
	float playerVisionWidth = 2.0f;
	float playerVisionHeight = 1.5f;

	// GlobalVision Ȱ��ȭ ����
	bool bEnableGlobalVision = false;

	// �������� Ŭ���� ����
	bool IsStageClear = false;

	// �÷��̾��� ���� ����
	bool isPlayerDead = false;

	// �������� ���� �ð� �ʰ� ����
	bool isTimeOut = false;
};

