﻿#pragma once

#include "Core.h"
#include "Input.h"
#include "Math/Vector2.h"
#include "Math/Color.h"

#include <Windows.h>

// 엔진 설정 구조체
struct EngineSettings
{
	// 콘솔 화면 가로 크기.
	int width = 0;

	// 콘솔 화면 세로 크기.
	int height = 0;

	// 타겟 프레임 속도.
	float framerate = 0.0f;
};

// 정렬 순서 정보까지 저장할 수 있는 이미지 버퍼 구조체
struct ImageBuffer
{
	ImageBuffer(int bufferCount)
	{
		charInfoArray = new CHAR_INFO[bufferCount];
		memset(charInfoArray, 0, sizeof(CHAR_INFO) * bufferCount);

		sortingOrderArray = new int[bufferCount];
		memset(charInfoArray, 0, sizeof(int) * bufferCount);
	}

	~ImageBuffer()
	{
		SafeDeleteArray(charInfoArray);
		SafeDeleteArray(sortingOrderArray);
	}

	// 콘솔에 그릴 때 사용할 구조체 (문자,색상 값 저장).
	CHAR_INFO* charInfoArray = nullptr;

	// 해당 위치에 그릴지를 판단할 때 사용할 정렬 순서 값(sortingOrder).
	int* sortingOrderArray = nullptr;
};

class Level;
class ScreenBuffer;

class ENGINE_API Engine
{
public:
	// 생성자
	Engine();

	// 소멸자
	virtual ~Engine();

	// 엔진 실행 함수
	void Run();

	// 문자 그리기 요청 함수.
	void WriteToBuffer(
		const Vector2& position,
		const char* image,
		Color color = Color::White,
		int sortingOrder = 0
	);

	// 레벨 추가 함수
	void AddLevel(Level* newLevel);

	// 레벨 변경 함수.
	void ChangeLevel(Level* newLevel);

	// 메모리 해제 함수
	virtual void CleanUp();

	// 화면 가로/세로 크기 반환 함수.
	inline const int Width() const { return settings.width; }
	inline const int Height() const { return settings.height; }

	// 엔진 종료 함수
	void Quit();

	// 싱글톤 함수
	static Engine& Get();

protected:
	// 엔진/게임 초기화 시 사용 (레벨 추가 등).
	virtual void OnInitialized();

	void BeginPlay();
	void Tick(float deltaTime = 0.0f);

	// 화면 지우는 함수 (전체를 빈 문자열로 설정).
	void Clear();

	// 액터 그리기 함수 (백버퍼에 기록).
	void Render();

	// 버퍼 교환 함수 (프론트 버퍼 <-> 백버퍼)
	void Present();

	// 백버퍼 렌더 타겟 반환 함수.
	ScreenBuffer* GetRenderer() const;

	// 글자 버퍼 지우는 함수.
	void ClearImageBuffer();

	// 엔진 설정 로드 함수
	void LoadEngineSettings();

	// 게임 오버 시, 수행할 로직
	virtual void OnGameOver() = 0;

protected:
	// 엔진 종료 플래그.
	bool isQuit = false;

	// 메인 레벨
	Level* mainLevel = nullptr;

	// 입력 관리자
	Input input;

	// 문자, 색상, 정렬 순서까지 저장하는 버퍼
	ImageBuffer* imageBuffer = nullptr;

	// 이중 버퍼.
	ScreenBuffer* renderTargets[2]{};

	// 백버퍼로 사용하는 렌더 타겟의 인덱스.
	int currentRenderTargetIndex = 0;

	// 레벨 변경이 요청됐는지 여부를 확인하는 변수.
	bool levelChangeRequested = false;

	// 변경 요청된 레벨 정보.
	Level* changeRequestedLevel = nullptr;

	// 엔진 설정
	EngineSettings settings;

	// 싱글톤 변수
	static Engine* instance;
};