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
		// 게임 오버 문구 출력
		char buffer[20]{ "[ GAME OVER... ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 1, stagePos.y), buffer);

		// 게임 종료
		Engine::Get().Quit();
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
				ProcessPlayerAndScore(i);
				return true;

			case SortingOrder::Wall:
				return false;

			case SortingOrder::Item:
				// Todo: 플레이어 아이템 획득 로직 구현해야 함
				return true;

			case SortingOrder::Enemy:
				ProcessPlayerAndEnemy(inActor);
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
				ProcessPlayerAndEnemy(i);
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

		// x좌표 증가 처리
		stagePos.x++;
	}

	// 버퍼 해제
	delete[] buffer;

	// 파일 닫기
	fclose(mapFile);
}

void GameLevel::PrintScore()
{
	char buffer[20]{};
	sprintf_s(buffer, 20, "남은 점수: %d", remainingScore);

	Engine::Get().WriteToBuffer(Vector2(stagePos.x + 1, 1), buffer);
}

void GameLevel::ProcessPlayerAndScore(Actor* inScore)
{
	// 남은 점수 1 뺴줌
	remainingScore--;

	inScore->Destroy();
}

void GameLevel::ProcessPlayerAndEnemy(Actor* inPlayer)
{
	// 죽음 처리 (게임 종료)
	isPlayerDead = true;

	inPlayer->Destroy();
}
