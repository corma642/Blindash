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
	// ������ ��ġ�� ��� ���� �޸� ����
	for (Actor* actor : actors)
	{
		SafeDelete(actor);
	}

	actors.clear();
}

void Level::AddActor(Actor* newActor)
{
	// ��� �迭�� �߰�
	addRequstedActors.push_back(newActor);
}

void Level::DestroyActor(Actor* destroyedActor)
{
	// ��� �迭�� �߰�
	destroyRequstedActors.push_back(destroyedActor);
}

void Level::BeginPlay()
{
	for (Actor* const actor : actors)
	{
		// ���� ��û�� ������ ��� �������� ����
		if (actor->isExpired)
		{
			continue;
		}

		// �̹� BeginPlay�� ȣ��� ��ü�� �ǳʶٱ�
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
		// ���� ��û�� ������ ��� �������� ����
		if (actor->isExpired)
		{
			continue;
		}

		// ���� �ֱ� Ȯ�� �� ó��
		if (actor->autoDestroy)
		{
			// ���� �ֱ� ���� ó��
			actor->lifeTime -= deltaTime;

			// ������ ���ߴ��� Ȯ��
			if (actor->lifeTime <= 0.0f)
			{
				// ���� ���� ��û�ϰ�, ���� ���� ó��
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
		// ���� ��û�� ������ ��� �������� ����
		if (actor->isExpired)
		{
			continue;
		}

		// �� ���Ͱ� �����ص� �Ǵ� �������� �Ǻ�
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
	// actors �迭���� ���� ó��
	for (auto it = actors.begin(); it != actors.end();)
	{
		// ���� ��û�� �������� Ȯ�� �� �迭���� ����
		if ((*it)->isExpired)
		{
			// erase �Լ��� ����ϸ� iterator�� ��ȿȭ�Ǳ� ������
			// ��ȯ�Ǵ� ���� �����ؾ� ��
			it = actors.erase(it);
			continue;
		}

		++it;
	}

	// destroyRequstedActors �迭�� ��ȸ�ϸ� ���� delete
	for (auto* actor : destroyRequstedActors)
	{
		// �޸� ����
		SafeDelete(actor);
	}

	// ���� �迭 �ʱ�ȭ
	destroyRequstedActors.clear();

	// addRequstedActors �迭�� ��ȸ�ϸ� ���ο� ���� �߰�
	for (auto* const actor : addRequstedActors)
	{
		actors.push_back(actor);
		actor->SetOwner(this);
	}

	// �߰� �迭 �ʱ�ȭ
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
	// ���� ���Ͱ� �ƴ� ��쿡�� �ٷ� ���� ����
	if (inActor->GetSortingOrder() != SortingOrder::Dark)
	{
		return true;
	}

	// �÷��̾ GlobalVision ���¶�� �׻� ���� ���� ����
	if (GetEnableGlobalVision())
	{
		return false;
	}

	// �����Ϸ��� ���� ������ ��ġ
	Vector2 position = inActor->Position();

	// �÷��̾�� ���� ���Ͱ��� �Ÿ� ���ϱ�
	if (IsWithinEllipticalZone(position, playerPos)) return false;

	return true;
}

bool Level::IsWithinEllipticalZone(const Vector2& actorPos, const Vector2& playerPos) const
{
	float deltaX = static_cast<float>(abs(actorPos.x - playerPos.x));
	float deltaY = static_cast<float>(abs(actorPos.y - playerPos.y));

	// ����ȭ�� Ÿ�� ������: (x/a)�� + (y/b)�� <= 1
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

	// ����ȭ�� Ÿ�� ������: (x/a)�� + (y/b)�� <= 1
	float normalizedDistance = (deltaX * deltaX) / (newPlayerVisionWidth * newPlayerVisionWidth) +
		(deltaY * deltaY) / (newPlayerVisionHeight * newPlayerVisionHeight);

	return normalizedDistance <= 1.0f;
}
