#pragma once

#include "Engine.h"

class Level;

class Game : public Engine
{
public:
	Game();
	virtual ~Game();

	// ���� �޴� �Լ�
	void GameStart();

	// �ɼ� �Լ�
	void ChangeMainMenu();
	void ToggleOptionMenu();

	// �������� ���� �Լ�
	void ChangeSelectStageMenu(int stageNumber);

	// ���� ���� ��, ������ ����
	virtual void OnGameOver() override;

	virtual void CleanUp() override;

	static Game& Get();

private:
	// �ɼ� ����
	Level* optionLevel = nullptr;

	// �÷������� ���� ����
	Level* gameLevel = nullptr;

	// ���� ���� �޴� ������ �����ְ��ִ��� Ȯ���ϴ� �Լ�
	bool showMainMenu = false;

	// ���� �ɼ� ������ �����ְ��ִ��� Ȯ���ϴ� �Լ�
	bool showOptionMenu = false;

	static Game* instance;
};

