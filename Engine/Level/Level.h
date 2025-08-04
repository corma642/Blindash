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
	Vector2 GetPlayerPos() const;
	void SetPlayerPos(const Vector2& newPosition);

	// �߰� �� ���� ��û�� ���͸� ó���ϴ� �Լ�
	void ProcessAddAndDestroyActors();

protected:
	// �����ص� �Ǵ� �������� �Ǻ��ϴ� �Լ�
	bool ShouldRenderActor(Actor* inActor);

	// �÷��̾� �þ� ���� �ִ��� �˻��ϴ� �Լ�
	bool IsWithinEllipticalZone(const Vector2& actorPos, const Vector2& playerPos) const;

protected:
	// ������ ��ġ�� ��� ���͸� �����ϴ� �迭
	std::vector<Actor*> actors;

	// �߰� ��û�� ���͸� �����ϴ� �迭
	std::vector<Actor*> addRequstedActors;

	// ���� ��û�� ���͸� �����ϴ� �迭
	std::vector<Actor*> destroyRequstedActors;

	// �÷��̾��� ��ġ
	Vector2 playerPos = Vector2::Zero;

	// �÷��̾��� �þ� (���̴� �Ÿ�)
	float playerVisionWidth = 4.0f;
	float playerVisionHeight = 3.0f;
};

