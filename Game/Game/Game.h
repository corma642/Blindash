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
	void ToggleMainMenu();
	void ToggleOptionMenu();

	// �������� Ŭ���� ��, ������ ����
	virtual void OnStageClear() override;

	// ���� ���� ��, ������ ����
	virtual void OnGameOver() override;

	virtual void CleanUp() override;

	static Game& Get();

private:
	// �ɼ� ����
	Level* optionLevel = nullptr;

	// ȭ�鿡 �Ⱥ��̴� ����
	Level* backLevel = nullptr;

	// ���� �ɼ� ������ �����ְ� �ִ����� ��Ÿ��
	bool showOption = false;

	// ���� ���� �޴� ������ �����ְ� �ִ����� ��Ÿ��
	bool showMainMenu = false;

	static Game* instance;
};

