#pragma once

#include "Level/Level.h"
#include "Actor/Actor.h"

#include <vector>

struct MainMenuItem
{
	// 함수 포인터 선언
	typedef void(*OnSelected)();

	MainMenuItem(const char* text, OnSelected onSelected)
		: onSelected(onSelected)
	{
		// 메뉴 텍스트 문자열 복사
		size_t length = strlen(text) + 1;
		menuText = new char[length];
		strcpy_s(menuText, length, text);
	}

	~MainMenuItem()
	{
		SafeDeleteArray(menuText);
	}

	// 메뉴 텍스트
	char* menuText = nullptr;

	// 메뉴 선택 시 실행할 동작
	OnSelected onSelected = nullptr;
};

class MainMenuLevel : public Level
{
	RTTI_DECLARATIONS(MainMenuLevel, Level)

public:
	MainMenuLevel();
	~MainMenuLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	void PrintGameName();

private:
	// 현재 선택된 아이템의 인덱스
	int currentIndex = 0;

	// 아이템 선택 색상
	Color selectedColor = Color::Green;

	// 아이템 선택 안됐을 때 색상
	Color unSelectedColor = Color::White;

	// 아이템 배열
	std::vector<MainMenuItem*> items;

	// 메뉴 아이템 수
	int length = 0;
};