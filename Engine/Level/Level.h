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

	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void Render();

protected:
	// ������ ��ġ�� ��� ���͸� �����ϴ� �迭
	std::vector<Actor*> actors;
};

