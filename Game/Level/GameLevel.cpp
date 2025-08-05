#include "GameLevel.h"
#include "Math/Vector2.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Level/Level.h"

#include "Actor/Player.h"
#include "Actor/Enemy.h"
#include "Actor/Wall.h"
#include "Actor/Dark.h"
#include "Actor/Score.h"

#include <iostream>

GameLevel::GameLevel()
{
	ReadStageFile("Stage_03.txt");
}

void GameLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);
}

void GameLevel::Render()
{
	super::Render();

	if (IsStageClear)
	{
		// �������� Ŭ���� ���� ���
		char buffer1[25]{ "[ !- GAME CLEAR -! ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 1), buffer1, Color::Green);

		// ���� �������� �ٷΰ��� ���� ���
		char buffer2[50]{ "[ ���� �������� �ٷΰ��� >> \"Enter\" ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y), buffer2, Color::White);
	}
	else if (isPlayerDead)
	{
		// ���� ���� ���� ���
		char buffer[20]{ "[ YOU DIED... ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y), buffer, Color::Red);
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
				ProcessPlayerAndScore(inActor, i);
				return true;

			case SortingOrder::Wall:
				return false;

			case SortingOrder::Enemy:
				ProcessPlayerAndEnemy(inActor, i);
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
				ProcessPlayerAndEnemy(i, inActor);
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
			if (useDark) AddActor(new Dark(stagePos));
			AddActor(new Wall(stagePos));
			break;

		case '.':
			if (useDark) AddActor(new Dark(stagePos));
			AddActor(new Score(stagePos));
			remainingScore++;
			break;

		case 'D':
			if (useDark) AddActor(new Dark(stagePos));
			AddActor(new Player(stagePos));
			playerPos = stagePos;
			break;

		case 'M':
			if (useDark) AddActor(new Dark(stagePos));
			AddActor(new Enemy(stagePos));
			break;

		default:
			if (useDark) AddActor(new Dark(stagePos));
			break;
		}

		// x��ǥ ���� ó��
		stagePos.x++;
	}
	if (useDark) AddActor(new Dark(stagePos));

	// ���� ����
	delete[] buffer;

	// ���� �ݱ�
	fclose(mapFile);
}

void GameLevel::PrintScore()
{
	char buffer1[20]{};
	char buffer2[20]{};
	sprintf_s(buffer1, 20, "���� ����: %d", currentScore);
	sprintf_s(buffer2, 20, "���� ����: %d", remainingScore);

	Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, 1), buffer1, currentScore > 0 ? Color::Green : Color::White);
	Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, 2), buffer2, Color::White);
}

void GameLevel::ProcessPlayerAndScore(Actor* inPlayer, Actor* inScore)
{
	// ���� ó��
	currentScore++;
	inScore->SetSortingOrder(SortingOrder::None);

	// ������ �ߵ� ó��
	// 20% Ȯ���� ������ �ߵ�
	int itemActivation = Utils::Random(1, 100);
	if (itemActivation <= 20)
	{
		Player* player = inPlayer->As<Player>();
		if (!player)
		{
			__debugbreak();
		}

		// �ߵ��� ������ �ɷ� ���� �̱�
		int itemType = Utils::Random(1, 6);

		switch (itemType)
		{
		case 1:
		case 2:
		case 3:
			player->ItemExpandVisionRange();
			break;

		case 4:
		case 5:
			player->StartGlobalVision();
			break;

		case 6:
			player->StartSuperMode();
			break;
		}
	}

	inScore->Destroy();

	// ���� Ŭ���� ���� Ȯ��
	if (currentScore == remainingScore)
	{
		IsStageClear = true;
	}
}

void GameLevel::ProcessPlayerAndEnemy(Actor* inPlayer, Actor* inEnemy)
{
	Player* player = inPlayer->As<Player>();
	if (!player)
	{
		__debugbreak();
	}

	// �÷��̾ ���� ����̸�, ���� óġ
	if (player->GetEnableSuperMode())
	{
		inEnemy->Destroy();
	}
	else
	{
		// ���� ó�� (���� ����)
		isPlayerDead = true;
		inPlayer->Destroy();
	}
}
