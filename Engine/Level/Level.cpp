#include "Level.h"
#include "Actor\Actor.h"
#include "Utils/Utils.h"
#include "Engine.h"

#include <algorithm>
#include <vector>

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
	// 대기 배열에 추가
	addRequstedActors.push_back(newActor);
}

void Level::DestroyActor(Actor* destroyedActor)
{
	// 대기 배열에 추가
	destroyRequstedActors.push_back(destroyedActor);
}

void Level::BeginPlay()
{
	for (Actor* const actor : actors)
	{
		// 삭제 요청된 액터의 경우 수행하지 않음
		if (actor->isExpired)
		{
			continue;
		}

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
		// 삭제 요청된 액터의 경우 수행하지 않음
		if (actor->isExpired)
		{
			continue;
		}

		// 생명 주기 확인 및 처리
		if (actor->autoDestroy)
		{
			// 생명 주기 감소 처리
			actor->lifeTime -= deltaTime;

			// 수명이 다했는지 확인
			if (actor->lifeTime <= 0.0f)
			{
				// 액터 제거 요청하고, 다음 액터 처리
				actor->lifeTime = 0.0f;
				actor->Destroy();
				continue;
			}
		}

		actor->Tick(deltaTime);
	}
}

void Level::Render()
{
	for (Actor* const actor : actors)
	{
		// 삭제 요청된 액터의 경우 수행하지 않음
		if (actor->isExpired)
		{
			continue;
		}

		// 이 액터가 렌더해도 되는 액터인지 판별
		if (ShouldRenderActor(actor))
		{
			actor->Render();
		}
	}
}

void Level::SetPlayerVisionRadius(float newVisionWidth, float newVisionHeight)
{
}

void Level::ProcessAddAndDestroyActors()
{
	// actors 배열에서 제외 처리
	for (auto it = actors.begin(); it != actors.end();)
	{
		// 삭제 요청된 액터인지 확인 후 배열에서 제외
		if ((*it)->isExpired)
		{
			// erase 함수를 사용하면 iterator가 무효화되기 때문에
			// 반환되는 값을 저장해야 함
			it = actors.erase(it);
			continue;
		}

		++it;
	}

	// destroyRequstedActors 배열을 순회하며 액터 delete
	for (auto* actor : destroyRequstedActors)
	{
		// 메모리 해제
		SafeDelete(actor);
	}

	// 삭제 배열 초기화
	destroyRequstedActors.clear();

	// addRequstedActors 배열을 순회하며 새로운 액터 추가
	for (auto* const actor : addRequstedActors)
	{
		actors.push_back(actor);
		actor->SetOwner(this);
	}

	// 추가 배열 초기화
	addRequstedActors.clear();
}

bool Level::StageClear()
{
	return IsStageClear;
}

bool Level::GameOver()
{
	return isPlayerDead || isTimeOut;
}

bool Level::ShouldRenderActor(Actor* inActor)
{
	// 암흑 액터가 아닌 경우에는 바로 성공 리턴
	if (inActor->GetSortingOrder() != SortingOrder::Dark)
	{
		return true;
	}

	// 플레이어가 GlobalVision 상태라면 항상 암흑 액터 무시
	if (GetEnableGlobalVision())
	{
		return false;
	}

	// 렌더하려는 암흑 액터의 위치
	Vector2 position = inActor->Position();

	// 플레이어와 암흑 액터간의 거리 구하기
	if (IsWithinEllipticalZone(position, playerPos)) return false;

	return true;
}

bool Level::IsWithinEllipticalZone(const Vector2& actorPos, const Vector2& playerPos) const
{
	float deltaX = static_cast<float>(abs(actorPos.x - playerPos.x));
	float deltaY = static_cast<float>(abs(actorPos.y - playerPos.y));

	// 정규화된 타원 방정식: (x/a)² + (y/b)² <= 1
	float normalizedDistance = (deltaX * deltaX) / (playerVisionWidth * playerVisionWidth) +
		(deltaY * deltaY) / (playerVisionHeight * playerVisionHeight);

	return normalizedDistance <= 1.0f;
}

bool Level::IsWithinEllipticalZoneV2(const Vector2& actorPos, const Vector2& playerPos) const
{
	float newPlayerVisionWidth = playerVisionWidth - 1.0f;
	float newPlayerVisionHeight = playerVisionHeight - 0.5f;

	float deltaX = static_cast<float>(abs(actorPos.x - playerPos.x));
	float deltaY = static_cast<float>(abs(actorPos.y - playerPos.y));

	// 정규화된 타원 방정식: (x/a)² + (y/b)² <= 1
	float normalizedDistance = (deltaX * deltaX) / (newPlayerVisionWidth * newPlayerVisionWidth) +
		(deltaY * deltaY) / (newPlayerVisionHeight * newPlayerVisionHeight);

	return normalizedDistance <= 1.0f;
}
