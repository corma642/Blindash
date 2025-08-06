#pragma once

#include "Level/Level.h"
#include "Actor/Actor.h"

#include <vector>

struct SelectMenuItem
{
	// �Լ� ������ ����
	typedef void(*OnSelected)();

	SelectMenuItem(const char* text, OnSelected onSelected)
		: onSelected(onSelected)
	{
		// �޴� �ؽ�Ʈ ���ڿ� ����
		size_t length = strlen(text) + 1;
		menuText = new char[length];
		strcpy_s(menuText, length, text);
	}

	~SelectMenuItem()
	{
		SafeDeleteArray(menuText);
	}

	// �޴� �ؽ�Ʈ
	char* menuText = nullptr;

	// �޴� ���� �� ������ ����
	OnSelected onSelected = nullptr;
};

class SelectStageLevel : public Level
{
	RTTI_DECLARATIONS(SelectStageLevel, Level)

public:
	SelectStageLevel();
	~SelectStageLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

private:
	void PrintBackGround();

private:
	// ���� ���õ� �������� �ε���
	int currentIndex = 0;

	// ������ ���� ����
	Color selectedColor = Color::Green;

	// ������ ���� �ȵ��� �� ����
	Color unSelectedColor = Color::White;

	// ������ �迭
	std::vector<SelectMenuItem*> items;

	// �޴� ������ ��
	int length = 0;
};