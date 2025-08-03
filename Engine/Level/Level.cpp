#include "Level.h"
#include "Actor\Actor.h"
#include "Utils/Utils.h"

#include <algorithm>

Level::Level()
{
}

Level::~Level()
{
	// 레벨에 배치된 모든 액터 메모리 해제
	for (Actor* actor : actors)
	{
		SafeDelete(actor);
	}

	actors.clear();
}

void Level::AddActor(Actor* newActor)
{
	actors.push_back(newActor);

	// 오너십 설정
	newActor->SetOwner(this);
}

void Level::BeginPlay()
{
	for (Actor* const actor : actors)
	{
		// 이미 BeginPlay가 호출된 개체는 건너뛰기
		if (actor->HasBeganPlay())
		{
			continue;
		}

		actor->BeginPlay();
	}
}

void Level::Tick(float deltaTime)
{
	for (Actor* const actor : actors)
	{
		actor->Tick(deltaTime);
	}
}

void Level::Render()
{
	for (Actor* const actor : actors)
	{
		actor->Render();
	}
}
