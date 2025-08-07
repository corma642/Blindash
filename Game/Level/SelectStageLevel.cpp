#include "SelectStageLevel.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Input.h"
#include "Utils/StageLimit.h"

#include <iostream>

SelectStageLevel::SelectStageLevel()
{
	// 메뉴 아이템 추가
	items.push_back(new SelectMenuItem(
		">> Stage 1",
		[]() { Game::Get().ChangeSelectStageMenu(1, StageLimit::GetStageLimit(1)); }
	));

	// 메뉴 아이템 추가
	items.push_back(new SelectMenuItem(
		">> Stage 2",
		[]() { Game::Get().ChangeSelectStageMenu(2, StageLimit::GetStageLimit(2)); }
	));

	// 메뉴 아이템 추가
	items.push_back(new SelectMenuItem(
		">> Stage 3",
		[]() { Game::Get().ChangeSelectStageMenu(3, StageLimit::GetStageLimit(3)); }
	));

	// 메뉴 아이템 추가
	items.push_back(new SelectMenuItem(
		">> Stage 4",
		[]() { Game::Get().ChangeSelectStageMenu(3, StageLimit::GetStageLimit(4)); }
	));

	// 메뉴 아이템 추가
	items.push_back(new SelectMenuItem(
		">> BACK TO MAIN MENU",
		[]() { Game::Get().ChangeMainMenu(); }
	));

	// 아이템 수 미리 저장
	length = static_cast<int>(items.size());
}

SelectStageLevel::~SelectStageLevel()
{
	for (SelectMenuItem* item : items)
	{
		SafeDelete(item);
	}

	items.clear();
}

void SelectStageLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 입력 처리
	if (Input::Get().GetKeyDown(VK_UP) || Input::Get().GetKeyDown('W'))
	{
		currentIndex = (currentIndex - 1 + length) % length;
	}

	if (Input::Get().GetKeyDown(VK_DOWN) || Input::Get().GetKeyDown('S'))
	{
		currentIndex = (currentIndex + 1) % length;
	}

	// Enter Key 입력
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// 메뉴 아이템이 저장하고 있는 함수 호출
		items[currentIndex]->onSelected();
	}
}

void SelectStageLevel::Render()
{
	super::Render();

	PrintBackGround();

	// 메뉴 아이템 렌더링
	for (int i = 0, yPos = 5; i < length; ++i, yPos += 2)
	{
		// 아이템 색상 확인
		Color textColor =
			(i == currentIndex) ? selectedColor : unSelectedColor;

		// 메뉴 텍스트 출력
		Engine::Get().WriteToBuffer(Vector2(2, yPos), items[i]->menuText, textColor);
	}
}

void SelectStageLevel::PrintBackGround()
{
	char buffer[30][100]{
		{ "+----------------------------------------------------------+" },
		{ "|                                                          |" },
		{ "|                                                          |" },
		{ "+----------------------------------------------------------+" },
		{ "|                                                          |" },
		{ "|                                                          |" },
		{ "|                                                          |" },
		{ "|                                                          |" },
		{ "|                                                          |" },
		{ "|                                                          |" },
		{ "|                                                          |" },
		{ "|                                                          |" },
		{ "|                                                          |" },
		{ "|                                                          |" },
		{ "|                                                          |" },
		{ "+----------------------------------------------------------+" }
	};

	for (int i = 0; i < 30; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(0, i), buffer[i], Color::Blue);
	}


	char buffer2[1][100]{
		{ "* BLINDASH *" },
	};

	for (int i = 0; i < 1; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(24, 1), buffer2[i], Color::White);
	}

	char buffer3[1][100]{
		{ "< CHOOSE A STAGE >" },
	};

	for (int i = 0; i < 1; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(21, 2), buffer3[i], Color::Green);
	}

}
