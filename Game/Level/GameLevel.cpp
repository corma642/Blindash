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
#include "Actor/Effect/ExpandVisionEffect.h"

#include <iostream>

GameLevel::GameLevel(const int stageNum)
{
	currnetStage = stageNum;
	char raedStageFileName[50]{};
	sprintf_s(raedStageFileName, 50, "Stage_0%d.txt", stageNum);

	ReadStageFile(raedStageFileName);
}

void GameLevel::Tick(float deltaTime)
{
	if (IsStageClear)
	{
		// �Է� ó��
		if (Input::Get().GetKeyDown(VK_RETURN))
		{
			IsStageClear = false;
			Game::Get().ChangeSelectStageMenu(++currnetStage);
		}

		if (Input::Get().GetKeyDown(VK_ESCAPE))
		{
			IsStageClear = false;
			Game::Get().ChangeMainMenu();
		}
	}
	else
	{
		super::Tick(deltaTime);
	}
}

void GameLevel::Render()
{
	super::Render();

	if (IsStageClear)
	{
		// �������� Ŭ���� ���� ���
		char buffer1[25]{ "[ !- GAME CLEAR -! ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 2), buffer1, Color::Green);

		// ���� �������� �ٷΰ��� ���� ���
		char buffer2[50]{ "[ ���� �������� �ٷΰ��� >> \"Enter\" ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 1), buffer2, Color::White);

		// ���� �޴��� ���� ���� ���
		char buffer3[50]{ "[ ���� �޴��� ���� >> \"ESC\" ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 0), buffer3, Color::White);
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

void GameLevel::SetPlayerVisionRadius(float newVisionWidth, float newVisionHeight)
{
	// ���� ���(��ü ����)�� �ƴ� ��쿡�� �þ� ������ �����Ǵ� �κп� ����Ʈ �߰�
	if (!GetEnableGlobalVision())
	{
		CalculateVisionRing();
	}

	// �þ� �ݰ� ������Ʈ
	playerVisionWidth = newVisionWidth;
	playerVisionHeight = newVisionHeight;
}

void GameLevel::CalculateVisionRing()
{
	std::vector<Vector2> ringTiles;

	for (int y = 1; y < stagePos.y - 1; ++y)
	{
		for (int x = 1; x < stagePos.x - 1; ++x)
		{
			// �þ� ���� ���̸� �ǳʶ�
			if (!IsWithinEllipticalZone(Vector2(x, y), playerPos))
			{
				continue;
			}

			// �þ� ������ �׵θ����
			if (!IsWithinEllipticalZoneV2(Vector2(x, y), playerPos))
			{
				AddActor(new ExpandVisionEffect(Vector2(x, y)));
			}
		}
	}
}

void GameLevel::ReadStageFile(const char* fileName)
{
	// ���� ���� ��� �ϼ�
	char filePath[256]{};
	sprintf_s(filePath, 256, "../Assets/%s", fileName);

	FILE* mapFile = nullptr;
	fopen_s(&mapFile, filePath, "rt");

	// ���� ó��
	if (!mapFile)
	{
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

	// ���ڿ� ����
	int size = static_cast<int>(readSize);

	// ���簢�� ��� ���
	int mapWidth = 0;
	int mapHeight = 0;
	int currentLineWidth = 0;

	// ���� ũ�� ���
	for (int i = 0; i < size; i++)
	{
		if (buffer[i] == '\n')
		{
			if (currentLineWidth > mapWidth)
			{
				mapWidth = currentLineWidth;
			}
			mapHeight++;
			currentLineWidth = 0;
		}
		else
		{
			currentLineWidth++;
		}
	}

	// ������ �� ó�� (���� ���� ������ ���� ���)
	if (currentLineWidth > 0)
	{
		if (currentLineWidth > mapWidth)
		{
			mapWidth = currentLineWidth;
		}
		mapHeight++;
	}

	// �迭 ��ȸ�� ���� ��ǥ �� �ε��� ����
	stagePos = Vector2(0, 0);
	int index = 0;

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

		// �����ڸ� �Ǵ� ����
		bool isTopEdge = (stagePos.y == 0);
		bool isBottomEdge = (stagePos.y == mapHeight - 1);
		bool isLeftEdge = (stagePos.x == 0);
		bool isRightEdge = (stagePos.x == mapWidth - 1);

		bool isEdge = isTopEdge || isBottomEdge || isLeftEdge || isRightEdge;

		// �� ���ں� ó��
		switch (mapCharacter)
		{
		case '8':
			if (!isEdge) AddActor(new Dark(stagePos));
			AddActor(new Wall(stagePos));
			break;

		case '.':
			if (!isEdge) AddActor(new Dark(stagePos));
			AddActor(new Score(stagePos));
			remainingScore++;
			break;

		case 'D':
			if (!isEdge) AddActor(new Dark(stagePos));
			AddActor(new Player(stagePos));
			playerPos = stagePos;
			break;

		case 'M':
			if (!isEdge) AddActor(new Dark(stagePos));
			AddActor(new Enemy(stagePos));
			break;

		case ' ':
			if (!isEdge) AddActor(new Dark(stagePos));
			break;
		}

		// x��ǥ ���� ó��
		stagePos.x++;
	}

	// ���ҽ� ����
	SafeDeleteArray(buffer);
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
	// 15% Ȯ���� ������ �ߵ�
	int itemActivation = Utils::Random(1, 100);
	if (itemActivation <= 15)
	{
		Player* player = inPlayer->As<Player>();
		if (!player)
		{
			__debugbreak();
		}

		// �ߵ��� ������ �ɷ� ���� �̱�
		int itemType = Utils::Random(1, 5);

		switch (itemType)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			// �÷��̾� �þ� �ݰ� ����
			player->ItemExpandVisionRange();
			break;

		case 5:
			// ���� ��� Ȱ��ȭ
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
