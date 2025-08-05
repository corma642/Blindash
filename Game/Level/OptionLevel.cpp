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
		[]() { Game::Get().ToggleMainMenu(); }
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
		// Game�� ��� �޴� ��� �߰� �� ȣ���ؾ� ��
		Game::Get().ToggleOptionMenu();

		currentIndex = 0;
	}
}

void OptionLevel::Render()
{
	super::Render();

	// ���� ���� ���
	char buffer[25]{ "[ PAC-MAN GAME ]" };
	Engine::Get().WriteToBuffer(Vector2(0, 0), buffer, Color::White);

	int yPos = 2;

	// �޴� ������ ������
	for (int i = 0; i < length; ++i)
	{
		// ������ ���� Ȯ��
		Color textColor =
			(i == currentIndex) ? selectedColor : unSelectedColor;

		// �޴� �ؽ�Ʈ ���
		Engine::Get().WriteToBuffer(Vector2(0, yPos), items[i]->menuText, textColor);
		yPos += 2;
	}
}