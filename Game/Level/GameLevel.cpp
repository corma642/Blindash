#include "GameLevel.h"
#include "Math/Vector2.h"

#include <iostream>

GameLevel::GameLevel()
{
	ReadStageFile("Stage_01.txt");
}

void GameLevel::Render()
{
	super::Render();
}

void GameLevel::ReadStageFile(const char* fileName)
{
	// 최종 에셋 경로 완성
	char filePath[256]{};
	sprintf_s(filePath, 256, "../Assets/%s", fileName);

	FILE* mapFile = nullptr;
	fopen_s(&mapFile, filePath, "rb");

	// 예외처리
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

			// Todo: 테스트용도. 객체 생성 시, 나중에 삭제해야 함
			std::cout << "\n";

			continue;
		}

		// 각 문자별 처리
		switch (mapCharacter)
		{
		case '8': // Wall 액터 생성
			//AddActor(new Wall(pos));
			std::cout << "8";
			break;
		case '.': // Score 액터 생성
			// Ground 액터 생성
			//AddActor(new Ground(pos));
			std::cout << ".";
			break;
		case 'D':
			// Ground도 같이 생성
			//AddActor(new Ground(pos));

			// Player 액터 생성
			//AddActor(new Player(pos));
			std::cout << "D";
			break;
		//case 'b':
		//	// 땅도 같이 생성
		//	AddActor(new Ground(pos));

		//	// Box 액터 생성
		//	AddActor(new Box(pos));
		//	break;
		//case 't':
		//	// Target 액터 생성
		//	AddActor(new Target(pos));

		//	// 목표 점수 증가 처리
		//	targetScore++;
		//	break;
		}
		// x좌표 증가 처리
		pos.x++;
	}

	// 버퍼 해제
	delete[] buffer;

	// 파일 닫기
	fclose(mapFile);
}
