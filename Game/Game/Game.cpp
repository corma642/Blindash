#include "Engine.h"
#include "Game.h"
#include "Input.h"

#include "Level/MainMenuLevel.h"
#include "Level/SelectStageLevel.h"
#include "Level/GameLevel.h"
#include "Level/OptionLevel.h"

Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;

	showMainMenu = true;
	AddLevel(new MainMenuLevel);
}

Game::~Game()
{
	CleanUp();
}

void Game::GameStart()
{
	// �������� ���� ���� �߰� �� ���� ��ȯ ��û
	showMainMenu = false;
	ChangeLevel(new SelectStageLevel());
}

void Game::ChangeMainMenu()
{
	// ���� ���� �߰� �� ���� ��ȯ ��û
	showMainMenu = true;
	ChangeLevel(new MainMenuLevel());
}

void Game::ToggleOptionMenu()
{
	// ȭ�� ����
	system("cls");

	// ��� ó��
	showOptionMenu = !showOptionMenu;

	if (showOptionMenu)
	{
		// ���� ������ �ڷ� �б�
		gameLevel = mainLevel;

		// �ɼ� �޴� ������ ���� ������ ����
		mainLevel = optionLevel;
	}
	else
	{
		// ���� ������ ���� ������ ����
		mainLevel = gameLevel;
	}
}

void Game::ChangeSelectStageMenu(int stageNumber)
{
	// ���� ���� �߰� �� ���� ��ȯ ��û
	gameLevel = new GameLevel(stageNumber);
	ChangeLevel(gameLevel);

	// �ɼ� ���� ����
	if (optionLevel)
	{
		SafeDelete(optionLevel);
	}
	optionLevel = new OptionLevel();
}

void Game::OnGameOver()
{
	ChangeMainMenu();
}

void Game::CleanUp()
{
	if (showMainMenu)
	{
		if (showOptionMenu)
		{
			SafeDelete(gameLevel);
			showOptionMenu = false;
		}
		else
		{
			SafeDelete(optionLevel);
		}
	}
	else
	{
		if (showOptionMenu)
		{
			// ���� ���� ����
			SafeDelete(gameLevel);
			mainLevel = nullptr;
		}

		SafeDelete(optionLevel);
	}
	Engine::CleanUp();
}

Game& Game::Get()
{
	return *instance;
}
