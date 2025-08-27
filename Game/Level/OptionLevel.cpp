#include "OptionLevel.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Input.h"

#include <iostream>

OptionLevel::OptionLevel()
{
	// �޴� ������ �߰�
	items.push_back(new OptionMenuItem(
		"Resume Game",
		[]() { Game::Get().ToggleOptionMenu(); }
	));

	// �޴� ������ �߰�
	items.push_back(new OptionMenuItem(
		"Main Menu",
		[]() { Game::Get().ToggleOptionMenu(); Game::Get().ChangeMainMenu(); }
	));

	// �޴� ������ �߰�
	items.push_back(new OptionMenuItem(
		"Quit Game",
		[]() { Game::Get().Quit(); }
	));

	// ������ �� �̸� ����
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

	// �Է� ó��
	if (Input::Get().GetKeyDown(VK_UP) || Input::Get().GetKeyDown('W'))
	{
		currentIndex = (currentIndex - 1 + length) % length;
	}

	if (Input::Get().GetKeyDown(VK_DOWN) || Input::Get().GetKeyDown('S'))
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
		Game::Get().ToggleOptionMenu();
		currentIndex = 0;
	}
}

void OptionLevel::Render()
{
	super::Render();

	PrintBackGround();

	// �޴� ������ ������
	for (int i = 0, yPos = 5; i < length; ++i, yPos += 2)
	{
		// ������ ���� Ȯ��
		Color textColor =
			(i == currentIndex) ? selectedColor : unSelectedColor;

		// �޴� �ؽ�Ʈ ���
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
