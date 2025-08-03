#include "GameLevel.h"
#include "Math/Vector2.h"
#include "Engine.h"

#include "Actor/Player.h"
#include "Actor/Enemy.h"
#include "Actor/Wall.h"
#include "Actor/Dark.h"
#include "Actor/Score.h"

#include <iostream>

GameLevel::GameLevel()
{
	ReadStageFile("Stage_01.txt");
}

void GameLevel::Render()
{
	super::Render();
}

bool GameLevel::CanMove(const class Actor* inActor, const Vector2& currentPosition, const Vector2& nextPosition)
{
	// �� ������ ���� Ȯ��
	SortingOrder sortingOrder = SortingOrder::None;
	sortingOrder = inActor->GetSortingOrder();

	// �÷��̾��� �̵�
	if (sortingOrder == SortingOrder::Player)
	{
		for (const auto& i : actors)
		{
			if (i->Position() != nextPosition)
			{
				continue;
			}

			SortingOrder nextPosSortingOrder = i->GetSortingOrder();

			switch (nextPosSortingOrder)
			{
			case SortingOrder::Score:
				// Todo: �÷��̾� ���� ȹ�� ���� �����ؾ� ��
				return true;

			case SortingOrder::Wall:
				return false;

			case SortingOrder::Item:
				// Todo: �÷��̾� ������ ȹ�� ���� �����ؾ� ��
				return true;

			case SortingOrder::Enemy:
				// Todo: �÷��̾� �� �浹 ���� �����ؾ� ��
				return false;
			}
		}
	}

	// ���� �̵�
	if (sortingOrder == SortingOrder::Enemy)
	{
		for (const auto& i : actors)
		{
			if (i->Position() != nextPosition)
			{
				continue;
			}

			SortingOrder nextPosSortingOrder = i->GetSortingOrder();

			switch (nextPosSortingOrder)
			{
			case SortingOrder::Wall:
				return false;

			case SortingOrder::Enemy:
				return false;

			case SortingOrder::Player:
				// Todo: �� �÷��̾� �浹 ���� �����ؾ� ��
				return true;
			}
		}
	}

	// ���� �Դٴ� ���� �� �����̶�� �ǹ�
	return true;
}

void GameLevel::ReadStageFile(const char* fileName)
{
	// ���� ���� ��� �ϼ�
	char filePath[256]{};
	sprintf_s(filePath, 256, "../Assets/%s", fileName);

	FILE* mapFile = nullptr;
	fopen_s(&mapFile, filePath, "rb");

	// ���� ó��
	if (!mapFile)
	{
		std::cout << "�� ���� �б� ����: " << fileName << "\n";
		__debugbreak();
		return;
	}

	// �Ľ�(Parcing, �ؼ�)
	fseek(mapFile, 0, SEEK_END);
	size_t fileSize = ftell(mapFile);
	rewind(mapFile);

	// Ȯ���� ���� ũ�⸦ Ȱ���� ���� �Ҵ�
	char* buffer = new char[fileSize + 1];
	memset(buffer, 0, fileSize + 1);
	size_t readSize = fread(buffer, sizeof(char), fileSize, mapFile);

	// �迭 ��ȸ�� ���� �ε��� ����
	int index = 0;

	// ���ڿ� ���� ��
	int size = static_cast<int>(readSize);

	// x, y�� ��ǥ
	Vector2 pos;

	// ���� �迭 ��ȸ
	while (index < size)
	{
		// �� ���� Ȯ��
		char mapCharacter = buffer[index++];

		// ���� ���� ó��
		if (mapCharacter == '\n')
		{
			// ���� �ٷ� �ѱ�鼭, x ��ǥ �ʱ�ȭ
			pos.x = 0;
			++pos.y;
			continue;
		}

		// �� ���ں� ó��
		switch (mapCharacter)
		{
		case '8':
			//AddActor(new Dark(pos));
			AddActor(new Wall(pos));
			break;
		case '.':
			//AddActor(new Dark(pos));
			AddActor(new Score(pos));
			break;
		case 'D':
			//AddActor(new Player(pos));
			AddActor(new Player(pos));
			break;
		case 'M':
			//AddActor(new Dark(pos));
			AddActor(new Enemy(pos));
			break;
		}

		// x��ǥ ���� ó��
		pos.x++;
	}

	// ���� ����
	delete[] buffer;

	// ���� �ݱ�
	fclose(mapFile);
}
