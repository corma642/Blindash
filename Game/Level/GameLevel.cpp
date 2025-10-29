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
		// 입력 처리
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
		// 스테이지 클리어 문구 출력
		char buffer1[25]{ "[ !- GAME CLEAR -! ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 2), buffer1, Color::LightGreen);

		// 다음 스테이지 바로가기 문구 출력
		char buffer2[50]{ "[ 다음 스테이지 바로가기 >> \"Enter\" ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 1), buffer2, Color::LightWhite);

		// 메인 메뉴로 가기 문구 출력
		char buffer3[50]{ "[ 메인 메뉴로 가기 >> \"ESC\" ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y - 0), buffer3, Color::LightWhite);
	}
	else if (isPlayerDead)
	{
		// 게임 오버 문구 출력
		char buffer[20]{ "[ YOU DIED... ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y), buffer, Color::LightRed);
	}
	else if (isTimeOut)
	{
		// 타임 아웃 문구 출력
		char buffer[20]{ "[ TIME OUT... ]" };
		Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y), buffer, Color::LightRed);
	}
	else
	{
		// 스테이지 남은 제한 시간 출력
		PrintTimeLimit();
	}

	// 클리어까지 남은 스코어 출력
	PrintRemainingScore();
}

bool GameLevel::CanMove(class Actor* inActor, const Vector2& currentPosition, const Vector2& nextPosition)
{
	// 맵 범위를 벗어나면 이동 불가
	if (nextPosition.x < 0 || nextPosition.y < 0 || nextPosition.x >= stagePos.x || nextPosition.y >= stagePos.y)
	{
		return false;
	}

	// 액터의 종류 확인
	SortingOrder sortingOrder = inActor->GetSortingOrder();

	// 플레이어 이동 처리
	if (sortingOrder == SortingOrder::Player)
	{
		for (const auto& i : actors)
		{
			// 다음 위치와 일치하지 않으면 스킵
			if (i->Position() != nextPosition)
			{
				continue;
			}

			// 다음 위치의 액터 종류 확인
			SortingOrder nextPosSortingOrder = i->GetSortingOrder();

			// 종류에 따른 로직 처리
			switch (nextPosSortingOrder)
			{
			case SortingOrder::Score:
				ProcessPlayerAndScore(inActor, i);	// 플레이어와 점수 아이템 처리
				return true;						// 이동 성공

			case SortingOrder::Wall:
				return false;						// 벽: 이동 불가

			case SortingOrder::Enemy:
				ProcessPlayerAndEnemy(inActor, i);	// 플레이어와 적 처리
				return false;						// 적: 이동 불가 (처리 후)
			}
		}
	}

	// 적 이동 처리
	if (sortingOrder == SortingOrder::Enemy)
	{
		for (const auto& i : actors)
		{
			// 다음 위치와 일치하지 않으면 스킵
			if (i->Position() != nextPosition)
			{
				continue;
			}

			// 다음 위치의 액터 종류 확인
			SortingOrder nextPosSortingOrder = i->GetSortingOrder();

			// 시야가 가려진 경우 무시
			if (nextPosSortingOrder == SortingOrder::Dark)
			{
				continue;
			}

			// 종류에 따른 로직 처리
			switch (nextPosSortingOrder)
			{
			case SortingOrder::Wall:
				return false;						// 벽: 이동 불가

			case SortingOrder::Player:
				ProcessPlayerAndEnemy(i, inActor);	// 적과 플레이어 처리 (순서 주의)
				return true;						// 이동 성공 (처리 후)

			default:
				return true;						// 기타: 이동 가능
			}
		}
	}

	// 빈 공간: 이동 가능
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
	// 파일 경로 생성
	char filePath[256]{};
	sprintf_s(filePath, 256, "../Assets/%s", fileName);

	// 파일 열기
	FILE* mapFile = nullptr;
	fopen_s(&mapFile, filePath, "rt");

	// 파일 열기 실패 시 디버그 브레이크
	if (!mapFile)
	{
		__debugbreak();
		return;
	}

	// 파일 크기 확인
	fseek(mapFile, 0, SEEK_END);
	size_t fileSize = ftell(mapFile);
	rewind(mapFile);

	// 버퍼 할당 및 초기화
	char* buffer = new char[fileSize + 1];
	memset(buffer, 0, fileSize + 1);

	// 파일 내용 읽기
	size_t readSize = fread(buffer, sizeof(char), fileSize, mapFile);
	int size = static_cast<int>(readSize);

	// 맵 크기 계산 (너비와 높이)
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

	// 마지막 줄 처리 (개행 없이 끝나는 경우)
	if (currentLineWidth > 0)
	{
		if (currentLineWidth > mapWidth)
		{
			mapWidth = currentLineWidth;
		}
		mapHeight++;
	}

	// 스테이지 위치 초기화
	stagePos = Vector2(0, 0);
	int index = 0;

	// 버퍼 순회하며 맵 파싱 및 액터 생성
	while (index < size)
	{
		// 맵 문자 확인
		char mapCharacter = buffer[index++];

		// 개행 문자 처리: 다음 줄로 이동
		if (mapCharacter == '\n')
		{
			// 다음 줄로 넘기면서, x 좌표 초기화
			stagePos.x = 0;
			++stagePos.y;
			continue;
		}

		// 가장자리 여부 판단
		bool isTopEdge = (stagePos.y == 0);
		bool isBottomEdge = (stagePos.y == mapHeight - 1);
		bool isLeftEdge = (stagePos.x == 0);
		bool isRightEdge = (stagePos.x == mapWidth - 1);

		bool isEdge = isTopEdge || isBottomEdge || isLeftEdge || isRightEdge;

		// 문자에 따른 액터 생성
		switch (mapCharacter)
		{
		case '8': // 벽 생성
			// 가장자리가 아니면 Dark 추가
			if (!isEdge) AddActor(new Dark(stagePos));
			AddActor(new Wall(stagePos));
			break;

		case '.': // 점수 생성
			if (!isEdge) AddActor(new Dark(stagePos));
			AddActor(new Score(stagePos));
			remainingScore++; // 남은 점수 증가
			break;

		case 'D': // 플레이어 생성
			if (!isEdge) AddActor(new Dark(stagePos));
			AddActor(new Player(stagePos));
			playerPos = stagePos; // 플레이어 위치 저장
			break;

		case 'M': // 적 생성
			if (!isEdge) AddActor(new Dark(stagePos));
			AddActor(new Enemy(stagePos));
			break;

		case ' ': // 암흑 생성 (빈 공간)
			if (!isEdge) AddActor(new Dark(stagePos));
			break;
		}

		// X 좌표 증가
		stagePos.x++;
	}

	// 리소스 정리
	SafeDeleteArray(buffer);
	fclose(mapFile);
}

void GameLevel::PrintRemainingScore()
{
	char buffer[20]{};
	sprintf_s(buffer, 20, "남은 점수: %d", remainingScore);

	Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, 2), buffer, Color::LightWhite);
}

void GameLevel::PrintTimeLimit()
{
	char buffer[50]{};
	sprintf_s(buffer, 50, "남은 시간: %.3f", timeLimit);

	Engine::Get().WriteToBuffer(Vector2(stagePos.x + 2, stagePos.y), buffer, Color::LightWhite);
}

void GameLevel::ProcessPlayerAndScore(Actor* inPlayer, Actor* inScore)
{
	// 남은 점수 감소
	remainingScore--;

	// 점수 액터의 정렬 순서 초기화 (제거 준비)
	inScore->SetSortingOrder(SortingOrder::None);

	// 점수 획득 시 확률 이벤트 처리
	int itemActivation = Utils::Random(1, 100); // 1~100 랜덤 값 생성

	// 30% 확률로 시간 제한 1초 증가
	if (itemActivation <= 30)
	{
		// 시간 제한 증가
		timeLimit += 1;

		// 시간 증가 이펙트 생성 및 추가
		AddActor(new AddTimeLimitEffect("+1", Vector2(stagePos.x + 13, stagePos.y - 1)));
	}

	// 15% 확률로 아이템 발동
	if (itemActivation <= 15)
	{
		Player* player = inPlayer->As<Player>();
		if (!player)
		{
			__debugbreak();
		}

		// 아이템 타입 랜덤 선택 (1~5)
		int itemType = Utils::Random(1, 5);

		switch (itemType)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			// 시야 반경 증가 아이템 발동 (80% 확률: 1~4)
			player->ItemExpandVisionRange();
			break;

		case 5:
			// 슈퍼 모드 활성화 아이템 발동 (20% 확률)
			player->StartSuperMode();
			break;
		}
	}

	// 점수 액터 파괴
	inScore->Destroy();

	// 남은 점수가 0 이하이면 스테이지 클리어
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

	// 플레이어가 슈퍼 모드이면, 몬스터 처치
	if (player->GetEnableSuperMode())
	{
		inEnemy->Destroy();

		// 제한 시간 5초 증가
		timeLimit += 5;
		AddActor(new AddTimeLimitEffect("+5", Vector2(stagePos.x + 13, stagePos.y - 1)));
	}
	else
	{
		// 죽음 처리 (게임 종료)
		isPlayerDead = true;
		inPlayer->Destroy();
	}
}
