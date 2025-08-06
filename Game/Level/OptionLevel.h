#pragma once

#include "Level/Level.h"
#include "Actor/Actor.h"

#include <vector>

struct OptionMenuItem
{
	// �Լ� ������ ����
	typedef void(*OnSelected)();

	OptionMenuItem(const char* text, OnSelected onSelected)
		: onSelected(onSelected)
	{
		// �޴� �ؽ�Ʈ ���ڿ� ����
		size_t length = strlen(text) + 1;
		menuText = new char[length];
		strcpy_s(menuText, length, text);
	}

	~OptionMenuItem()
	{
		SafeDeleteArray(menuText);
	}

	// �޴� �ؽ�Ʈ
	char* menuText = nullptr;

	// �޴� ���� �� ������ ����
	OnSelected onSelected = nullptr;
};

class OptionLevel : public Level
{
	RTTI_DECLARATIONS(OptionLevel, Level)

public:
	OptionLevel();
	~OptionLevel();

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
	std::vector<OptionMenuItem*> items;

	// �޴� ������ ��
	int length = 0;
};