#include "Level.h"
#include "Actor\Actor.h"

Level::Level()
{
}

Level::~Level()
{
	// ������ ��ġ�� ��� ���� �޸� ����
	for (Actor* actor : actors)
	{
		// �޸� ���� �Լ�
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
	SortActorsBySortingOrder();

	for (Actor* const actor : actors)
	{
		// �˻� (���� ��ġ�� ���� ���� ���� ���Ͱ� �ִ��� Ȯ��)
		Actor* searchedActor = nullptr;
		for (Actor* const otherActor : actors)
		{
			// ���� ���͸� ����
			if (actor == otherActor)
			{
				continue;
			}

			// ��ġ�� ���� ���� Ȯ��
			if (actor->Position() == otherActor->Position())
			{
				// ���� ���� �� �� ���� ����
				if (actor->sortingOrder < otherActor->sortingOrder)
				{
					// ���� �� ���� ����
					searchedActor = otherActor;
					break;
				}
			}
		}

		// � ���Ϳ� ���� ��ġ�� ���� ������ �� ���� ���Ͱ� ������, �׸��� �ʰ� �ǳʶٱ�
		if (searchedActor)
		{
			continue;
		}

		// ��ο� ��
		actor->Render();
	}
}

void Level::SortActorsBySortingOrder()
{
	// ���� ������ ��ġ�� ��� ������ ũ��
	int actorsSize = static_cast<int>(actors.size());

	// ���� ����
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
