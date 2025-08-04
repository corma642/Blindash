#include "GameLevel.h"
#include "Math/Vector2.h"
#include "Engine.h"
#include "Utils/Utils.h"

#include "Actor/Player.h"
#include "Actor/Enemy.h"
#include "Actor/Wall.h"
#include "Actor/Dark.h"
#include "Actor/Score.h"

#include <iostream>

GameLevel::GameLevel()
{
	ReadStageFile("Stage_02.txt");
}

void GameLevel::Render()
{
	super::Render();

	if (isPlayerDead)
	{
		// ���� ���� ���� ���
		char buffer[20]{ "[ GAME OVER... ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 1, stagePos.y), buffer);

		// ���� ����
		Engine::Get().Quit();
	}

	// Ŭ������� ���� ���ھ� ���
	PrintScore();
}

bool GameLevel::CanMove(class Actor* inActor, const Vector2& currentPosition, const Vector2& nextPosition)
{
	// ���� ������ ����� �̵� ����
	if (nextPosition.x < 0 || nextPosition.y < 0 || nextPosition.x >= stagePos.x || nextPosition.y >= stagePos.y)
	{
		return false;
	}

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
				ProcessPlayerAndScore(i);
				return true;

			case SortingOrder::Wall:
				return false;

			case SortingOrder::Item:
				// Todo: �÷��̾� ������ ȹ�� ���� �����ؾ� ��
				return true;

			case SortingOrder::Enemy:
				ProcessPlayerAndEnemy(inActor);
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
			if (nextPosSortingOrder == SortingOrder::Dark)
			{
				continue;
			}

			switch (nextPosSortingOrder)
			{
			case SortingOrder::Wall:
				return false;

			case SortingOrder::Player:
				ProcessPlayerAndEnemy(i);
				return true;

			default:
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

	// ���� �迭 ��ȸ
	while (index < size)
	{
		// �� ���� Ȯ��
		char mapCharacter = buffer[index++];

		// ���� ���� ó��
		if (mapCharacter == '\n')
		{
			// ���� �ٷ� �ѱ�鼭, x ��ǥ �ʱ�ȭ
			stagePos.x = 0;
			++stagePos.y;
			continue;
		}

		// �� ���ں� ó��
		switch (mapCharacter)
		{
		case '8':
			//AddActor(new Dark(pos));
			AddActor(new Wall(stagePos));
			break;
		case '.':
			//AddActor(new Dark(pos));
			AddActor(new Score(stagePos));
			remainingScore++;
			break;
		case 'D':
			//AddActor(new Player(pos));
			AddActor(new Player(stagePos));
			break;
		case 'M':
			//AddActor(new Dark(pos));
			AddActor(new Enemy(stagePos));
			break;
		}

		// x��ǥ ���� ó��
		stagePos.x++;
	}

	// ���� ����
	delete[] buffer;

	// ���� �ݱ�
	fclose(mapFile);
}

void GameLevel::PrintScore()
{
	char buffer[20]{};
	sprintf_s(buffer, 20, "���� ����: %d", remainingScore);

	Engine::Get().WriteToBuffer(Vector2(stagePos.x + 1, 1), buffer);
}

void GameLevel::ProcessPlayerAndScore(Actor* inScore)
{
	// ���� ���� 1 ����
	remainingScore--;

	inScore->Destroy();
}

void GameLevel::ProcessPlayerAndEnemy(Actor* inPlayer)
{
	// ���� ó�� (���� ����)
	isPlayerDead = true;

	inPlayer->Destroy();
}
