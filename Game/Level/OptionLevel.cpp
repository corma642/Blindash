#include "OptionLevel.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Input.h"

#include <iostream>

OptionLevel::OptionLevel()
{
	// 메뉴 아이템 추가
	items.push_back(new OptionMenuItem(
		"Resume Game",
		[]() { Game::Get().ToggleOptionMenu(); }
	));

	// 메뉴 아이템 추가
	items.push_back(new OptionMenuItem(
		"Main Menu",
		[]() { Game::Get().ToggleOptionMenu(); Game::Get().ChangeMainMenu(); }
	));

	// 메뉴 아이템 추가
	items.push_back(new OptionMenuItem(
		"Quit Game",
		[]() { Game::Get().Quit(); }
	));

	// 아이템 수 미리 저장
	length = static_cast<int>(items.size());
}

OptionLevel::~OptionLevel()
{
	for (OptionMenuItem* item : items)
	{
		SafeDelete(item);
	}

	items.clear();
}

void OptionLevel::Tick(float deltaTime)
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

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		Game::Get().ToggleOptionMenu();
		currentIndex = 0;
	}
}

void OptionLevel::Render()
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

void OptionLevel::PrintBackGround()
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
		{ "+----------------------------------------------------------+" }
	};

	for (int i = 0; i < 30; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(0, i), buffer[i], Color::LightBlue);
	}


	char buffer2[1][100]{
		{ "* BLINDASH *" },
	};

	for (int i = 0; i < 1; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(24, 1), buffer2[i], Color::LightWhite);
	}

	char buffer3[1][100]{
		{ "< OPTION >" },
	};

	for (int i = 0; i < 1; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(25, 2), buffer3[i], Color::LightGreen);
	}

}
