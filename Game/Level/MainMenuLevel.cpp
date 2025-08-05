#include "MainMenuLevel.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Input.h"

#include <iostream>

MainMenuLevel::MainMenuLevel()
{
	// �޴� ������ �߰�
	items.push_back(new MainMenuItem(
		"Game Start",
		[]() { Game::Get().GameStart(); }
	));

	// �޴� ������ �߰�
	items.push_back(new MainMenuItem(
		"Quit Game",
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

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// Game�� ��� �޴� ��� �߰� �� ȣ���ؾ� ��
		Game::Get().ToggleOptionMenu();

		currentIndex = 0;
	}
}

void MainMenuLevel::Render()
{
	super::Render();

	// ���� ���� ���
	PrintGameName();

	// �޴� ������ ������
	Color textColor = (currentIndex == 0) ? selectedColor : unSelectedColor;
	Engine::Get().WriteToBuffer(Vector2(17, 14), items[0]->menuText, textColor);

	textColor = (currentIndex == 1) ? selectedColor : unSelectedColor;
	Engine::Get().WriteToBuffer(Vector2(39, 14), items[1]->menuText, textColor);
}

void MainMenuLevel::PrintGameName()
{
	char gameLogo[30][100]{
		{ "8888888888888888888888888888888888888888888888888888888888888888888" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "88                                                               88" },
		{ "8888888888888888888888888888888888888888888888888888888888888888888" }
	};

	for (int i = 0; i < 30; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(0, i), gameLogo[i], Color::Blue);
	}


	char menuLogoN[10][100]{
		{ "                                                  ##    ##" },
		{ "                                                  ###   ##" },
		{ "                                                  ####  ##" },
		{ "                                                  ## ## ##" },
		{ "                                                  ##  ####" },
		{ "                                                  ##   ###" },
		{ "                                                  ##    ##" },
		{ "                                                  ##     #" },
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 3), menuLogoN[i], Color::Red);
	}


	char menuLogoA2[10][100]{
		{ "                                           ##### " },
		{ "                                          #######" },
		{ "                                          ##   ##" },
		{ "                                          #######" },
		{ "                                          #######" },
		{ "                                          ##   ##" },
		{ "                                          ##   ##" },
		{ "                                          ##   ##" },
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 3), menuLogoA2[i], Color::Green);
	}


	char menuLogoM[10][100]{
		{ "                               ##   ##   " },
		{ "                              ###  ####  " },
		{ "                              #### ##### " },
		{ "                              ## ####### " },
		{ "                              ##  ### ###" },
		{ "                              ##   ##  ##" },
		{ "                              ##   ##  ##" },
		{ "                              ##        #" },
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 3), menuLogoM[i], Color::Yellow);
	}


	char menuLogoC[10][100]{
		{ "                     #####  " },
		{ "                    ####### " },
		{ "                    ##   ## " },
		{ "                    ##      " },
		{ "                    ##      " },
		{ "                    ##   ## " },
		{ "                     #######" },
		{ "                      ##### " },
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 3), menuLogoC[i], Color::Violet);
	}


	char menuLogoA1[10][100]{
		{ "             ##### " },
		{ "            #######" },
		{ "            ##   ##" },
		{ "            #######" },
		{ "            #######" },
		{ "            ##   ##" },
		{ "            ##   ##" },
		{ "             #   # " },
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 3), menuLogoA1[i], Color::Red);
	}


	char menuLogoP[10][100]{
		{ "     ##### " },
		{ "    #######" },
		{ "    ##   ##" },
		{ "    #######" },
		{ "    ###### " },
		{ "    ##     " },
		{ "    ##     " },
		{ "     #     " },
	};

	for (int i = 0; i < 10; ++i)
	{
		Engine::Get().WriteToBuffer(Vector2(2, i + 3), menuLogoP[i], Color::Blue);
	}
}
