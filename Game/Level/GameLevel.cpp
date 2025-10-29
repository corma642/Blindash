#include "GameLevel.h"
#include "Math/Vector2.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Level/Level.h"
#include "Utils/StageLimit.h"

#include "Actor/Player.h"
#include "Actor/Enemy.h"
#include "Actor/Wall.h"
#include "Actor/Dark.h"
#include "Actor/Score.h"
#include "Actor/Effect/ExpandVisionEffect.h"
#include "Actor/Effect/AddTimeLimitEffect.h"

#include <iostream>

GameLevel::GameLevel(const int stageNumber, const float timeLimit)
{
	currnetStage = stageNumber;
	this->timeLimit = timeLimit;

	char raedStageFileName[50]{};
	sprintf_s(raedStageFileName, 50, "Stage_0%d.txt", stageNumber);

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
			Game::Get().ChangeSelectStageMenu(++currnetStage, StageLimit::GetStageLimit(currnetStage));
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

		timeLimit -= deltaTime;
		if (timeLimit <= 0)
		{
			isTimeOut = true;
		}
	}
}

void GameLevel::Render()
{
	super::Render();

	if (IsStageClear)
	{
		// �������� Ŭ���� ���� ���
		char buffer1[25]{ "[ !- GAME CLEAR -! ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 2), buffer1, Color::LightGreen);

		// ���� �������� �ٷΰ��� ���� ���
		char buffer2[50]{ "[ ���� �������� �ٷΰ��� >> \"Enter\" ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 1), buffer2, Color::LightWhite);

		// ���� �޴��� ���� ���� ���
		char buffer3[50]{ "[ ���� �޴��� ���� >> \"ESC\" ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 0), buffer3, Color::LightWhite);
	}
	else if (isPlayerDead)
	{
		// ���� ���� ���� ���
		char buffer[20]{ "[ YOU DIED... ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y), buffer, Color::LightRed);
	}
	else if (isTimeOut)
	{
		// Ÿ�� �ƿ� ���� ���
		char buffer[20]{ "[ TIME OUT... ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y), buffer, Color::LightRed);
	}
	else
	{
		// �������� ���� ���� �ð� ���
		PrintTimeLimit();
	}

	// Ŭ������� ���� ���ھ� ���
	PrintRemainingScore();
}

bool GameLevel::CanMove(class Actor* inActor, const Vector2& currentPosition, const Vector2& nextPosition)
{
	// �� ������ ����� �̵� �Ұ�
	if (nextPosition.x < 0 || nextPosition.y < 0 || nextPosition.x >= stagePos.x || nextPosition.y >= stagePos.y)
	{
		return false;
	}

	// ������ ���� Ȯ��
	SortingOrder sortingOrder = inActor->GetSortingOrder();

	// �÷��̾� �̵� ó��
	if (sortingOrder == SortingOrder::Player)
	{
		for (const auto& i : actors)
		{
			// ���� ��ġ�� ��ġ���� ������ ��ŵ
			if (i->Position() != nextPosition)
			{
				continue;
			}

			// ���� ��ġ�� ���� ���� Ȯ��
			SortingOrder nextPosSortingOrder = i->GetSortingOrder();

			// ������ ���� ���� ó��
			switch (nextPosSortingOrder)
			{
			case SortingOrder::Score:
				ProcessPlayerAndScore(inActor, i);	// �÷��̾�� ���� ������ ó��
				return true;						// �̵� ����

			case SortingOrder::Wall:
				return false;						// ��: �̵� �Ұ�

			case SortingOrder::Enemy:
				ProcessPlayerAndEnemy(inActor, i);	// �÷��̾�� �� ó��
				return false;						// ��: �̵� �Ұ� (ó�� ��)
			}
		}
	}

	// �� �̵� ó��
	if (sortingOrder == SortingOrder::Enemy)
	{
		for (const auto& i : actors)
		{
			// ���� ��ġ�� ��ġ���� ������ ��ŵ
			if (i->Position() != nextPosition)
			{
				continue;
			}

			// ���� ��ġ�� ���� ���� Ȯ��
			SortingOrder nextPosSortingOrder = i->GetSortingOrder();

			// �þ߰� ������ ��� ����
			if (nextPosSortingOrder == SortingOrder::Dark)
			{
				continue;
			}

			// ������ ���� ���� ó��
			switch (nextPosSortingOrder)
			{
			case SortingOrder::Wall:
				return false;						// ��: �̵� �Ұ�

			case SortingOrder::Player:
				ProcessPlayerAndEnemy(i, inActor);	// ���� �÷��̾� ó�� (���� ����)
				return true;						// �̵� ���� (ó�� ��)

			default:
				return true;						// ��Ÿ: �̵� ����
			}
		}
	}

	// �� ����: �̵� ����
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
	// ���� ��� ����
	char filePath[256]{};
	sprintf_s(filePath, 256, "../Assets/%s", fileName);

	// ���� ����
	FILE* mapFile = nullptr;
	fopen_s(&mapFile, filePath, "rt");

	// ���� ���� ���� �� ����� �극��ũ
	if (!mapFile)
	{
		__debugbreak();
		return;
	}

	// ���� ũ�� Ȯ��
	fseek(mapFile, 0, SEEK_END);
	size_t fileSize = ftell(mapFile);
	rewind(mapFile);

	// ���� �Ҵ� �� �ʱ�ȭ
	char* buffer = new char[fileSize + 1];
	memset(buffer, 0, fileSize + 1);

	// ���� ���� �б�
	size_t readSize = fread(buffer, sizeof(char), fileSize, mapFile);
	int size = static_cast<int>(readSize);

	// �� ũ�� ��� (�ʺ�� ����)
	int mapWidth = 0;
	int mapHeight = 0;
	int currentLineWidth = 0;

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

	// ������ �� ó�� (���� ���� ������ ���)
	if (currentLineWidth > 0)
	{
		if (currentLineWidth > mapWidth)
		{
			mapWidth = currentLineWidth;
		}
		mapHeight++;
	}

	// �������� ��ġ �ʱ�ȭ
	stagePos = Vector2(0, 0);
	int index = 0;

	// ���� ��ȸ�ϸ� �� �Ľ� �� ���� ����
	while (index < size)
	{
		// �� ���� Ȯ��
		char mapCharacter = buffer[index++];

		// ���� ���� ó��: ���� �ٷ� �̵�
		if (mapCharacter == '\n')
		{
			// ���� �ٷ� �ѱ�鼭, x ��ǥ �ʱ�ȭ
			stagePos.x = 0;
			++stagePos.y;
			continue;
		}

		// �����ڸ� ���� �Ǵ�
		bool isTopEdge = (stagePos.y == 0);
		bool isBottomEdge = (stagePos.y == mapHeight - 1);
		bool isLeftEdge = (stagePos.x == 0);
		bool isRightEdge = (stagePos.x == mapWidth - 1);

		bool isEdge = isTopEdge || isBottomEdge || isLeftEdge || isRightEdge;

		// ���ڿ� ���� ���� ����
		switch (mapCharacter)
		{
		case '8': // �� ����
			// �����ڸ��� �ƴϸ� Dark �߰�
			if (!isEdge) AddActor(new Dark(stagePos));
			AddActor(new Wall(stagePos));
			break;

		case '.': // ���� ����
			if (!isEdge) AddActor(new Dark(stagePos));
			AddActor(new Score(stagePos));
			remainingScore++; // ���� ���� ����
			break;

		case 'D': // �÷��̾� ����
			if (!isEdge) AddActor(new Dark(stagePos));
			AddActor(new Player(stagePos));
			playerPos = stagePos; // �÷��̾� ��ġ ����
			break;

		case 'M': // �� ����
			if (!isEdge) AddActor(new Dark(stagePos));
			AddActor(new Enemy(stagePos));
			break;

		case ' ': // ���� ���� (�� ����)
			if (!isEdge) AddActor(new Dark(stagePos));
			break;
		}

		// X ��ǥ ����
		stagePos.x++;
	}

	// ���ҽ� ����
	SafeDeleteArray(buffer);
	fclose(mapFile);
}

void GameLevel::PrintRemainingScore()
{
	char buffer[20]{};
	sprintf_s(buffer, 20, "���� ����: %d", remainingScore);

	Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, 2), buffer, Color::LightWhite);
}

void GameLevel::PrintTimeLimit()
{
	char buffer[50]{};
	sprintf_s(buffer, 50, "���� �ð�: %.3f", timeLimit);

	Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y), buffer, Color::LightWhite);
}

void GameLevel::ProcessPlayerAndScore(Actor* inPlayer, Actor* inScore)
{
	// ���� ���� ����
	remainingScore--;

	// ���� ������ ���� ���� �ʱ�ȭ (���� �غ�)
	inScore->SetSortingOrder(SortingOrder::None);

	// ���� ȹ�� �� Ȯ�� �̺�Ʈ ó��
	int itemActivation = Utils::Random(1, 100); // 1~100 ���� �� ����

	// 30% Ȯ���� �ð� ���� 1�� ����
	if (itemActivation <= 30)
	{
		// �ð� ���� ����
		timeLimit += 1;

		// �ð� ���� ����Ʈ ���� �� �߰�
		AddActor(new AddTimeLimitEffect("+1", Vector2(stagePos.x + 13, stagePos.y - 1)));
	}

	// 15% Ȯ���� ������ �ߵ�
	if (itemActivation <= 15)
	{
		Player* player = inPlayer->As<Player>();
		if (!player)
		{
			__debugbreak();
		}

		// ������ Ÿ�� ���� ���� (1~5)
		int itemType = Utils::Random(1, 5);

		switch (itemType)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			// �þ� �ݰ� ���� ������ �ߵ� (80% Ȯ��: 1~4)
			player->ItemExpandVisionRange();
			break;

		case 5:
			// ���� ��� Ȱ��ȭ ������ �ߵ� (20% Ȯ��)
			player->StartSuperMode();
			break;
		}
	}

	// ���� ���� �ı�
	inScore->Destroy();

	// ���� ������ 0 �����̸� �������� Ŭ����
	if (remainingScore <= 0)
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

		// ���� �ð� 5�� ����
		timeLimit += 5;
		AddActor(new AddTimeLimitEffect("+5", Vector2(stagePos.x + 13, stagePos.y - 1)));
	}
	else
	{
		// ���� ó�� (���� ����)
		isPlayerDead = true;
		inPlayer->Destroy();
	}
}
