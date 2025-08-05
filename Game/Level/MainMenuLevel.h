#pragma once

#include "Level/Level.h"
#include "Actor/Actor.h"

#include <vector>

struct MainMenuItem
{
	// �Լ� ������ ����
	typedef void(*OnSelected)();

	MainMenuItem(const char* text, OnSelected onSelected)
		: onSelected(onSelected)
	{
		// �޴� �ؽ�Ʈ ���ڿ� ����
		size_t length = strlen(text) + 1;
		menuText = new char[length];
		strcpy_s(menuText, length, text);
	}

	~MainMenuItem()
	{
		SafeDeleteArray(menuText);
	}

	// �޴� �ؽ�Ʈ
	char* menuText = nullptr;

	// �޴� ���� �� ������ ����
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
	// ���� ���õ� �������� �ε���
	int currentIndex = 0;

	// ������ ���� ����
	Color selectedColor = Color::Green;

	// ������ ���� �ȵ��� �� ����
	Color unSelectedColor = Color::White;

	// ������ �迭
	std::vector<MainMenuItem*> items;

	// �޴� ������ ��
	int length = 0;
};