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

	// �߰� �� ���� ��û�� ���͸� ó���ϴ� �Լ�
	void ProcessAddAndDestroyActors();

protected:
	// ������ ��ġ�� ��� ���͸� �����ϴ� �迭
	std::vector<Actor*> actors;

	// �߰� ��û�� ���͸� �����ϴ� �迭
	std::vector<Actor*> addRequstedActors;

	// ���� ��û�� ���͸� �����ϴ� �迭
	std::vector<Actor*> destroyRequstedActors;
};

