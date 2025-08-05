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
		// 스테이지 클리어 문구 출력
		char buffer1[25]{ "[ !- GAME CLEAR -! ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 1), buffer1, Color::Green);

		// 다음 스테이지 바로가기 문구 출력
		char buffer2[50]{ "[ 다음 스테이지 바로가기 >> \"Enter\" ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y), buffer2, Color::White);
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

void GameLevel::ReadStageFile(const char* fileName)
{
	// 최종 에셋 경로 완성
	char filePath[256]{};
	sprintf_s(filePath, 256, "../Assets/%s", fileName);

	FILE* mapFile = nullptr;
	fopen_s(&mapFile, filePath, "rb");

	// 예외 처리
	if (!mapFile)
	{
		std::cout << "맵 파일 읽기 실패: " << fileName << "\n";
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

	// 배열 순회를 위한 인덱스 변수
	int index = 0;

	// 문자열 길이 값
	int size = static_cast<int>(readSize);

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

		// 각 문자별 처리
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

		// x좌표 증가 처리
		stagePos.x++;
	}
	if (useDark) AddActor(new Dark(stagePos));

	// 버퍼 해제
	delete[] buffer;

	// 파일 닫기
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
	// 20% 확률로 아이템 발동
	int itemActivation = Utils::Random(1, 100);
	if (itemActivation <= 20)
	{
		Player* player = inPlayer->As<Player>();
		if (!player)
		{
			__debugbreak();
		}

		// 발동할 아이템 능력 랜덤 뽑기
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
