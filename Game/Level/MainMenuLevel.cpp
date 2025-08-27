#include "MainMenuLevel.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Input.h"

#include <iostream>

MainMenuLevel::MainMenuLevel()
{
	// �޴� ������ �߰�
	items.push_back(new MainMenuItem(
		"GAME START",
		[]() { Game::Get().GameStart(); }
	));

	// �޴� ������ �߰�
	items.push_back(new MainMenuItem(
		"QUIT GAME",
		[]() { Game::Get().Quit(); }
	));

	// ������ �� �̸� ����
	length = static_cast<int>(items.size());
}

MainMenuLevel::~MainMenuLevel()
{
	for (MainMenuItem* item : items)
	{
		SafeDelete(item);
	}

	items.clear();
}

void MainMenuLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// �Է� ó��
	if (Input::Get().GetKeyDown(VK_LEFT) || Input::Get().GetKeyDown('A'))
	{
		currentIndex = (currentIndex - 1 + length) % length;
	}

	if (Input::Get().GetKeyDown(VK_RIGHT) || Input::Get().GetKeyDown('D'))
	{
		currentIndex = (currentIndex + 1) % length;
	}

	// Enter Key �Է�
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// �޴� �������� �����ϰ� �ִ� �Լ� ȣ��
		items[currentIndex]->onSelected();
	}
}

void MainMenuLevel::Render()
{
	super::Render();

	// ���� ���� ���
	PrintGameName2();

	// �޴� ������ ������
	Color textColor = (currentIndex == 0) ? selectedColor : unSelectedColor;
	Engine::Get().WriteToBuffer(Vector2(17, 10), items[0]->menuText, textColor);

	textColor = (currentIndex == 1) ? selectedColor : unSelectedColor;
	Engine::Get().WriteToBuffer(Vector2(39, 10), items[1]->menuText, textColor);
}

void MainMenuLevel::PrintGameName2()
{
	char gameLogo[30][100]{
	{ "88888888888888888888888888888888888888888888888888888888888888888888" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88                                                                88" },
	{ "88888888888888888888888888888888888888888888888888888888888888888888" }
	};

	for (int i = 0; i < 30; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(0, i), gameLogo[i], Color::LightBlue);
	}

	char menuLogoH[10][100]{
		{"                                                       _____    "},
		{"                                                      |  |  |   "},
		{"                                                      |     |   "},
		{"                                                      |__|__|   "},
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 2), menuLogoH[i], Color::LightGreen);
	}

	char menuLogoS[10][100]{
		{"                                               _____ "},
		{"                                              |   __|"},
		{"                                              |__   |"},
		{"                                              |_____|"},
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 2), menuLogoS[i], Color::LightSkyBlue);
	}


	char menuLogoA[10][100]{
		{"                                       _____ "},
		{"                                      |  _  |"},
		{"                                      |     |"},
		{"                                      |__|__|"},
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 2), menuLogoA[i], Color::LightRed);
	}


	char menuLogoD[10][100]{
		{"                               ____  "},
		{"                              |    \\"},
		{"                              |  |  |"},
		{"                              |____/ "},
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 2), menuLogoD[i], Color::LightYellow);
	}


	char menuLogoN[10][100]{
		{"                       _____ "},
		{"                      |   | |"},
		{"                      | | | |"},
		{"                      |_|___|"},
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 2), menuLogoN[i], Color::LightViolet);
	}


	char menuLogoI[10][100]{
		{"                  ___ "},
		{"                 |   |"},
		{"                 |- -|"},
		{"                 |___|"},
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 2), menuLogoI[i], Color::LightBlue);
	}


	char menuLogoL[10][100]{
		{"           __    "},
		{"          |  |   "},
		{"          |  |__ "},
		{"          |_____|"},
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 2), menuLogoL[i], Color::LightGreen);
	}


	char menuLogoB[10][100]{
		{"    _____ "},
		{"   | __  |"},
		{"   | __ -|"},
		{"   |_____|"},
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 2), menuLogoB[i], Color::LightRed);
	}
}
