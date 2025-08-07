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
		// 입력 처리
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
		// 스테이지 클리어 문구 출력
		char buffer1[25]{ "[ !- GAME CLEAR -! ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 2), buffer1, Color::Green);

		// 다음 스테이지 바로가기 문구 출력
		char buffer2[50]{ "[ 다음 스테이지 바로가기 >> \"Enter\" ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 1), buffer2, Color::White);

		// 메인 메뉴로 가기 문구 출력
		char buffer3[50]{ "[ 메인 메뉴로 가기 >> \"ESC\" ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 0), buffer3, Color::White);
	}
	else if (isPlayerDead)
	{
		// 게임 오버 문구 출력
		char buffer[20]{ "[ YOU DIED... ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y), buffer, Color::Red);
	}

	// 클리어까지 남은 스코어 출력
	PrintScore();
}

bool GameLevel::CanMove(class Actor* inActor, const Vector2& currentPosition, const Vector2& nextPosition)
{
	// 맵의 범위를 벗어나면 이동 실패
	if (nextPosition.x < 0 || nextPosition.y < 0 || nextPosition.x >= stagePos.x || nextPosition.y >= stagePos.y)
	{
		return false;
	}

	// 이 액터의 종류 확인
	SortingOrder sortingOrder = SortingOrder::None;
	sortingOrder = inActor->GetSortingOrder();

	// 플레이어의 이동
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

	// 적의 이동
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

	// 여기 왔다는 것은 빈 공간이라는 의미
	return true;
}

void GameLevel::SetPlayerVisionRadius(float newVisionWidth, float newVisionHeight)
{
	// 슈퍼 모드(전체 비전)가 아닌 경우에만 시야 범위가 증가되는 부분에 이펙트 추가
	if (!GetEnableGlobalVision())
	{
		CalculateVisionRing();
	}

	// 시야 반경 업데이트
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
			// 시야 범위 밖이면 건너뜀
			if (!IsWithinEllipticalZone(Vector2(x, y), playerPos))
			{
				continue;
			}

			// 시야 범위의 테두리라면
			if (!IsWithinEllipticalZoneV2(Vector2(x, y), playerPos))
			{
				AddActor(new ExpandVisionEffect(Vector2(x, y)));
			}
		}
	}
}

void GameLevel::ReadStageFile(const char* fileName)
{
	// 최종 에셋 경로 완성
	char filePath[256]{};
	sprintf_s(filePath, 256, "../Assets/%s", fileName);

	FILE* mapFile = nullptr;
	fopen_s(&mapFile, filePath, "rt");

	// 예외 처리
	if (!mapFile)
	{
		__debugbreak();
		return;
	}

	// 파싱(Parcing, 해석)
	fseek(mapFile, 0, SEEK_END);
	size_t fileSize = ftell(mapFile);
	rewind(mapFile);

	// 확인한 파일 크기를 활용해 버퍼 할당
	char* buffer = new char[fileSize + 1];
	memset(buffer, 0, fileSize + 1);
	size_t readSize = fread(buffer, sizeof(char), fileSize, mapFile);

	// 문자열 길이
	int size = static_cast<int>(readSize);

	// 직사각형 경계 계산
	int mapWidth = 0;
	int mapHeight = 0;
	int currentLineWidth = 0;

	// 맵의 크기 계산
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

	// 마지막 줄 처리 (파일 끝에 개행이 없는 경우)
	if (currentLineWidth > 0)
	{
		if (currentLineWidth > mapWidth)
		{
			mapWidth = currentLineWidth;
		}
		mapHeight++;
	}

	// 배열 순회를 위한 좌표 및 인덱스 변수
	stagePos = Vector2(0, 0);
	int index = 0;

	// 문자 배열 순회
	while (index < size)
	{
		// 맵 문자 확인
		char mapCharacter = buffer[index++];

		// 개행 문자 처리
		if (mapCharacter == '\n')
		{
			// 다음 줄로 넘기면서, x 좌표 초기화
			stagePos.x = 0;
			++stagePos.y;
			continue;
		}

		// 가장자리 판단 로직
		bool isTopEdge = (stagePos.y == 0);
		bool isBottomEdge = (stagePos.y == mapHeight - 1);
		bool isLeftEdge = (stagePos.x == 0);
		bool isRightEdge = (stagePos.x == mapWidth - 1);

		bool isEdge = isTopEdge || isBottomEdge || isLeftEdge || isRightEdge;

		// 각 문자별 처리
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

		// x좌표 증가 처리
		stagePos.x++;
	}

	// 리소스 정리
	SafeDeleteArray(buffer);
	fclose(mapFile);
}

void GameLevel::PrintScore()
{
	char buffer1[20]{};
	char buffer2[20]{};
	sprintf_s(buffer1, 20, "현재 점수: %d", currentScore);
	sprintf_s(buffer2, 20, "남은 점수: %d", remainingScore);

	Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, 1), buffer1, currentScore > 0 ? Color::Green : Color::White);
	Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, 2), buffer2, Color::White);
}

void GameLevel::ProcessPlayerAndScore(Actor* inPlayer, Actor* inScore)
{
	// 점수 처리
	currentScore++;
	inScore->SetSortingOrder(SortingOrder::None);

	// 아이템 발동 처리
	// 15% 확률로 아이템 발동
	int itemActivation = Utils::Random(1, 100);
	if (itemActivation <= 15)
	{
		Player* player = inPlayer->As<Player>();
		if (!player)
		{
			__debugbreak();
		}

		// 발동할 아이템 능력 랜덤 뽑기
		int itemType = Utils::Random(1, 5);

		switch (itemType)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			// 플레이어 시야 반경 증가
			player->ItemExpandVisionRange();
			break;

		case 5:
			// 슈퍼 모드 활성화
			player->StartSuperMode();
			break;
		}
	}

	inScore->Destroy();

	// 게임 클리어 여부 확인
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

	// 플레이어가 슈퍼 모드이면, 몬스터 처치
	if (player->GetEnableSuperMode())
	{
		inEnemy->Destroy();
	}
	else
	{
		// 죽음 처리 (게임 종료)
		isPlayerDead = true;
		inPlayer->Destroy();
	}
}
