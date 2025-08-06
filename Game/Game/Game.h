#pragma once

#include "Engine.h"

class Level;

class Game : public Engine
{
public:
	Game();
	virtual ~Game();

	// 메인 메뉴 함수
	void GameStart();

	// 옵션 함수
	void ChangeMainMenu();
	void ToggleOptionMenu();

	// 스테이지 선택 함수
	void ChangeSelectStageMenu(int stageNumber);

	// 게임 오버 시, 수행할 로직
	virtual void OnGameOver() override;

	virtual void CleanUp() override;

	static Game& Get();

private:
	// 옵션 레벨
	Level* optionLevel = nullptr;

	// 플레이중인 게임 레벨
	Level* gameLevel = nullptr;

	// 현재 메인 메뉴 레벨을 보여주고있는지 확인하는 함수
	bool showMainMenu = false;

	// 현재 옵션 레벨을 보여주고있는지 확인하는 함수
	bool showOptionMenu = false;

	static Game* instance;
};

