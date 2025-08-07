#include "SelectStageLevel.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Input.h"
#include "Utils/StageLimit.h"

#include <iostream>

SelectStageLevel::SelectStageLevel()
{
	// �޴� ������ �߰�
	items.push_back(new SelectMenuItem(
		">> Stage 1",
		[]() { Game::Get().ChangeSelectStageMenu(1, StageLimit::GetStageLimit(1)); }
	));

	// �޴� ������ �߰�
	items.push_back(new SelectMenuItem(
		">> Stage 2",
		[]() { Game::Get().ChangeSelectStageMenu(2, StageLimit::GetStageLimit(2)); }
	));

	// �޴� ������ �߰�
	items.push_back(new SelectMenuItem(
		">> Stage 3",
		[]() { Game::Get().ChangeSelectStageMenu(3, StageLimit::GetStageLimit(3)); }
	));

	// �޴� ������ �߰�
	items.push_back(new SelectMenuItem(
		">> Stage 4",
		[]() { Game::Get().ChangeSelectStageMenu(3, StageLimit::GetStageLimit(4)); }
	));

	// �޴� ������ �߰�
	items.push_back(new SelectMenuItem(
		">> BACK TO MAIN MENU",
		[]() { Game::Get().ChangeMainMenu(); }
	));

	// ������ �� �̸� ����
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
}

void SelectStageLevel::Render()
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
