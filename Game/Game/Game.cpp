#include "Game.h"
#include "Level/MainMenuLevel.h"
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
	// ȭ�� ����
	system("cls");

	// ���� ���� �߰�
	AddLevel(new GameLevel());

	// �ɼ� ���� ����
	optionLevel = new OptionLevel();
}

void Game::ToggleMainMenu()
{
	// ȭ�� ����
	system("cls");

	CleanUp();

	showMainMenu = true;
	AddLevel(new MainMenuLevel);
}

void Game::ToggleOptionMenu()
{
	// ȭ�� ����
	system("cls");

	// ��� ó��
	showOption = !showOption;

	if (showOption)
	{
		// ���� ������ �ڷ� �б�
		backLevel = mainLevel;

		// �޴� ������ ���� ������ ����
		mainLevel = optionLevel;
	}
	else
	{
		// ���� ������ ���� ������ ����
		mainLevel = backLevel;
	}
}

void Game::OnStageClear()
{
	ToggleMainMenu();
}

void Game::OnGameOver()
{
	ToggleMainMenu();
}

void Game::CleanUp()
{
	if (showMainMenu)
	{
		Engine::CleanUp();
	}
	else
	{
		// �̶��� Engine�� mainLevel�� OptionLevel
		if (showOption)
		{
			// ���� ���� ����
			SafeDelete(backLevel);
		}

		showOption = false;
		SafeDelete(optionLevel);
		Engine::CleanUp();
	}
}

Game& Game::Get()
{
	return *instance;
}
