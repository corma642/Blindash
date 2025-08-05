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
	// 화면 정리
	system("cls");

	// 게임 레벨 추가
	AddLevel(new GameLevel());

	// 옵션 레벨 생성
	optionLevel = new OptionLevel();
}

void Game::ToggleMainMenu()
{
	// 화면 정리
	system("cls");

	CleanUp();

	showMainMenu = true;
	AddLevel(new MainMenuLevel);
}

void Game::ToggleOptionMenu()
{
	// 화면 정리
	system("cls");

	// 토글 처리
	showOption = !showOption;

	if (showOption)
	{
		// 게임 레벨을 뒤로 밀기
		backLevel = mainLevel;

		// 메뉴 레벨을 메인 레벨로 설정
		mainLevel = optionLevel;
	}
	else
	{
		// 게임 레벨을 메인 레벨로 설정
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
		// 이때는 Engine의 mainLevel이 OptionLevel
		if (showOption)
		{
			// 게임 레벨 제거
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
