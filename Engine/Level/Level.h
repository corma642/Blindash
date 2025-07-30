#pragma once

#include "Core.h"
#include <vector> // ���� �迭
#include "RTTI.h"

class Actor;

class ENGINE_API Level : public RTTI
{
	RTTI_DECLARATIONS(Level, RTTI)

public:
	Level();
	virtual ~Level();

	// ������ ���͸� �߰��ϴ� �Լ�
	void AddActor(Actor* newActor);

	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void Render();

private:
	// ���� ������� ���� �����ϴ� �Լ�
	void SortActorsBySortingOrder();

protected:
	// ������ ��ġ�� ��� ���͸� �����ϴ� �迭
	std::vector<Actor*> actors;

};

