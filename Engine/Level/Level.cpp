#include "Level.h"
#include "Actor\Actor.h"
#include "Utils/Utils.h"

#include <algorithm>

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

		actor->Render();
	}
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
