#include "Level.h"
#include "Actor\Actor.h"

Level::Level()
{
}

Level::~Level()
{
	// 레벨에 배치된 모든 액터 메모리 해제
	for (Actor* actor : actors)
	{
		// 메모리 정리 함수
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
	// 그리기 전에 정렬 순서 기준으로 재배치(정렬)
	SortActorsBySortingOrder();

	for (Actor* const actor : actors)
	{
		// 검사 (같은 위치에 정렬 순서 높은 액터가 있는지 확인)
		Actor* searchedActor = nullptr;
		for (Actor* const otherActor : actors)
		{
			// 같은 액터면 무시
			if (actor == otherActor)
			{
				continue;
			}

			// 위치가 같은 액터 확인
			if (actor->Position() == otherActor->Position())
			{
				// 정렬 순서 비교 후 액터 저장
				if (actor->sortingOrder < otherActor->sortingOrder)
				{
					// 저장 및 루프 종료
					searchedActor = otherActor;
					break;
				}
			}
		}

		// 어떤 액터와 같은 위치에 정렬 순서가 더 높은 액터가 있으면, 그리지 않고 건너뛰기
		if (searchedActor)
		{
			continue;
		}

		// 드로우 콜
		actor->Render();
	}
}

void Level::SortActorsBySortingOrder()
{
	// 현재 레벨에 배치된 모든 액터의 크기
	int actorsSize = static_cast<int>(actors.size());

	// 버블 정렬
	for (int i = 0; i < actorsSize; ++i)
	{
		for (int j = 0; j < actorsSize - 1; ++j)
		{
			if (actors[j]->sortingOrder > actors[j + 1]->sortingOrder)
			{
				std::swap(actors[j], actors[j + 1]);
			}
		}
	}
}
