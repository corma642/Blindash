#include "SelectStageLevel.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Input.h"

#include <iostream>

SelectStageLevel::SelectStageLevel()
{
	// �޴� ������ �߰�
	items.push_back(new SelectMenuItem(
		"Stage 1",
		[]() { Game::Get().ChangeSelectStageMenu(1); }
	));

	// �޴� ������ �߰�
	items.push_back(new SelectMenuItem(
		"Stage 2",
		[]() { Game::Get().ChangeSelectStageMenu(2); }
	));

	// �޴� ������ �߰�
	items.push_back(new SelectMenuItem(
		"Stage 3",
		[]() { Game::Get().ChangeSelectStageMenu(3); }
	));

	// �޴� ������ �߰�
	items.push_back(new SelectMenuItem(
		"Back",
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

	// �޴� ������ ������
	for (int i = 0, yPos = 2; i < length; ++i, yPos += 2)
	{
		// ������ ���� Ȯ��
		Color textColor =
			(i == currentIndex) ? selectedColor : unSelectedColor;

		// �޴� �ؽ�Ʈ ���
		Engine::Get().WriteToBuffer(Vector2(0, yPos), items[i]->menuText, textColor);
	}
}