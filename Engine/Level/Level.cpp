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
	actors.push_back(newActor);

	// ���ʽ� ����
	newActor->SetOwner(this);
}

void Level::BeginPlay()
{
	for (Actor* const actor : actors)
	{
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
		actor->Tick(deltaTime);
	}
}

void Level::Render()
{
	// �׸��� ���� ���� ���� �������� ���ġ(����)
	//SortActorsBySortingOrder();

	for (Actor* const actor : actors)
	{
		// �˻� (���� ��ġ�� ���� ���� ���� ���Ͱ� �ִ��� Ȯ��).
		//Actor* searchedActor = nullptr;
		//for (Actor* const otherActor : actors)
		//{
		//	// ���� ���͸� ����.
		//	if (actor == otherActor)
		//	{
		//		continue;
		//	}

		//	// ��ġ�� ���� ���� Ȯ��.
		//	if (actor->Position() == otherActor->Position())
		//	{
		//		// ���� ���� �� �� ���� ����.
		//		if (actor->sortingOrder < otherActor->sortingOrder)
		//		{
		//			// ���� �� ���� ����.
		//			searchedActor = otherActor;
		//			break;
		//		}
		//	}
		//}

		//// � ���Ϳ� ���� ��ġ�� ���� ������ �� ���� ���Ͱ� ������, �׸��� �ʰ� �ǳʶٱ�.
		//if (searchedActor)
		//{
		//	continue;
		//}

		// ��ο� ��.
		actor->Render();
	}
}

void Level::SortActorsBySortingOrder()
{
	std::sort(actors.begin(), actors.end(), [&](Actor*& a, Actor*& b)
		{
			return a->sortingOrder > b->sortingOrder;
		});
}
