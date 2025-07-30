#include "Level.h"
#include "Actor\Actor.h"

Level::Level()
{
}

Level::~Level()
{
	// 레벨에 있는 액터 메모리 해제
	for (Actor* actor : actors)
	{
		// null 확인 후 액터 제거
		//if (actor)
		//{
		//	// 삭제 및 메모리 정리
		//	delete actor;
		//	actor = nullptr;
		//}

		// 메모리 정리 함수
		SafeDelete(actor);
	}

	// std::vector 정리
	actors.clear();
}

void Level::AddActor(Actor* newActor)
{
	// 예외처리(중복 여부 확인) 필요함

	actors.emplace_back(newActor);

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

	// 그리기(Render Pass)
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
	int actorsSize = static_cast<int>(actors.size());

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
