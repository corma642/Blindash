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
	// 스테이지 선택 레벨 추가 후 레벨 변환 요청
	showMainMenu = false;
	ChangeLevel(new SelectStageLevel());
}

void Game::ChangeMainMenu()
{
	// 메인 레벨 추가 후 레벨 변환 요청
	showMainMenu = true;
	ChangeLevel(new MainMenuLevel());
}

void Game::ToggleOptionMenu()
{
	// 화면 정리
	system("cls");

	// 토글 처리
	showOptionMenu = !showOptionMenu;

	if (showOptionMenu)
	{
		// 게임 레벨을 뒤로 밀기
		gameLevel = mainLevel;

		// 옵션 메뉴 레벨을 메인 레벨로 설정
		mainLevel = optionLevel;
	}
	else
	{
		// 게임 레벨을 메인 레벨로 설정
		mainLevel = gameLevel;
	}
}

void Game::ChangeSelectStageMenu(int stageNumber)
{
	// 게임 레벨 추가 후 레벨 전환 요청
	gameLevel = new GameLevel(stageNumber);
	ChangeLevel(gameLevel);

	// 옵션 레벨 생성
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
			// 게임 레벨 제거
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
