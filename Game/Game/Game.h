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
	void ToggleMainMenu();
	void ToggleOptionMenu();

	// 스테이지 클리어 시, 수행할 로직
	virtual void OnStageClear() override;

	// 게임 오버 시, 수행할 로직
	virtual void OnGameOver() override;

	virtual void CleanUp() override;

	static Game& Get();

private:
	// 옵션 레벨
	Level* optionLevel = nullptr;

	// 화면에 안보이는 레벨
	Level* backLevel = nullptr;

	// 현재 옵션 레벨을 보여주고 있는지를 나타냄
	bool showOption = false;

	// 현재 메인 메뉴 레벨을 보여주고 있는지를 나타냄
	bool showMainMenu = false;

	static Game* instance;
};

