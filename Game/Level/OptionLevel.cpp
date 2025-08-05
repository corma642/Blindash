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
		[]() { Game::Get().ToggleMainMenu(); }
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
		// Game에 토글 메뉴 기능 추가 후 호출해야 함
		Game::Get().ToggleOptionMenu();

		currentIndex = 0;
	}
}

void OptionLevel::Render()
{
	super::Render();

	// 게임 제목 출력
	char buffer[25]{ "[ PAC-MAN GAME ]" };
	Engine::Get().WriteToBuffer(Vector2(0, 0), buffer, Color::White);

	int yPos = 2;

	// 메뉴 아이템 렌더링
	for (int i = 0; i < length; ++i)
	{
		// 아이템 색상 확인
		Color textColor =
			(i == currentIndex) ? selectedColor : unSelectedColor;

		// 메뉴 텍스트 출력
		Engine::Get().WriteToBuffer(Vector2(0, yPos), items[i]->menuText, textColor);
		yPos += 2;
	}
}