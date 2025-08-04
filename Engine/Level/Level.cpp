#include "Level.h"
#include "Actor\Actor.h"
#include "Utils/Utils.h"

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

Vector2 Level::GetPlayerPos() const
{
	return playerPos;
}

void Level::SetPlayerPos(const Vector2& newPosition)
{
	playerPos = newPosition;
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

bool Level::ShouldRenderActor(Actor* inActor)
{
	// ���� ���Ͱ� �ƴ� ��쿡�� �ٷ� ���� ����
	if (inActor->GetSortingOrder() != SortingOrder::Dark)
	{
		return true;
	}

	// �����Ϸ��� ���� ������ ��ġ
	Vector2 position = inActor->Position();

	// �÷��̾�� ���� ���Ͱ��� ����ư �Ÿ� ���ϱ�
	if (IsWithinEllipticalZone(position, playerPos)) return false;
	
	return true;
}

bool Level::IsWithinEllipticalZone(const Vector2& playerPos, const Vector2& actorPos) const
{
	float deltaX = static_cast<float>(abs(actorPos.x - playerPos.x));
	float deltaY = static_cast<float>(abs(actorPos.y - playerPos.y));

	// ����ȭ�� Ÿ�� ������: (x/a)�� + (y/b)�� <= 1
	float normalizedDistance = (deltaX * deltaX) / (playerVisionWidth * playerVisionWidth) +
		(deltaY * deltaY) / (playerVisionHeight * playerVisionHeight);

	return normalizedDistance <= 1.0f;
}
