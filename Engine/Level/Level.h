#pragma once

#include "Core.h"
#include <vector> // 동적 배열
#include "RTTI.h"

class Actor;

class ENGINE_API Level : public RTTI
{
	RTTI_DECLARATIONS(Level, RTTI)

public:
	Level();
	virtual ~Level();

	// 레벨에 액터를 추가하는 함수
	void AddActor(Actor* newActor);

	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	virtual void Render();

private:
	// 정렬 순서대로 액터 정렬하는 함수
	void SortActorsBySortingOrder();

protected:
	// 레벨에 배치된 모든 액터를 관리하는 배열
	std::vector<Actor*> actors;

};

