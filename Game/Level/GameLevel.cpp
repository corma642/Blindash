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
				// Todo: 플레이어 점수 획득 로직 구현해야 함
				return true;

			case SortingOrder::Wall:
				return false;

			case SortingOrder::Item:
				// Todo: 플레이어 아이템 획득 로직 구현해야 함
				return true;

			case SortingOrder::Enemy:
				// Todo: 플레이어 적 충돌 로직 구현해야 함
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

			switch (nextPosSortingOrder)
			{
			case SortingOrder::Wall:
				return false;

			case SortingOrder::Enemy:
				return false;

			case SortingOrder::Player:
				// Todo: 적 플레이어 충돌 로직 구현해야 함
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

	// x, y값 좌표
	Vector2 pos;

	// 문자 배열 순회
	while (index < size)
	{
		// 맵 문자 확인
		char mapCharacter = buffer[index++];

		// 개행 문자 처리
		if (mapCharacter == '\n')
		{
			// 다음 줄로 넘기면서, x 좌표 초기화
			pos.x = 0;
			++pos.y;
			continue;
		}

		// 각 문자별 처리
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

		// x좌표 증가 처리
		pos.x++;
	}

	// 버퍼 해제
	delete[] buffer;

	// 파일 닫기
	fclose(mapFile);
}
