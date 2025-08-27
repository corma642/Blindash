#include "Player.h"
#include "Input.h"
#include "Level/Level.h"
#include "Interface/GameLevelInterface.h"
#include "Utils/Timer.h"
#include "Game/Game.h"

#include <iostream>

Player::Player(const Vector2& position)
	: super("D", Color::LightYellow, position)
{
	SetSortingOrder(SortingOrder::Player);

	// 타이머 설정 (반복/연속 이동 제한 시간 설정)
	playerRepeatMoveXTimer.SetTargetTime(xMoveThreshold);
	playerRepeatMoveYTimer.SetTargetTime(yMoveThreshold);
}

void Player::BeginPlay()
{
	super::BeginPlay();

	if (GetOwner())
	{
		gameLevelInterface = dynamic_cast<IGameLevelInterface*>(GetOwner());
	}

	if (!gameLevelInterface)
	{
		std::cout << "Player: Can not cast to owner level to ICanPlayerMove" << "\n";
	}
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (Input::Get().GetKey(VK_ESCAPE))
	{
		Game::Get().ToggleOptionMenu();
		return;
	}

	// 이동 입력 처리
	PlayerMovement(deltaTime);

	// 슈퍼 모드
	ItemActivateSuperMode(deltaTime);
}

void Player::ItemExpandVisionRange()
{
	float currentPlayerVistionWidth = owner->GetPlayerVisionWidth();
	float currentPlayerVistionHeight = owner->GetPlayerVisionHeight();

	// 시야 반경 증가
	owner->SetPlayerVisionRadius(currentPlayerVistionWidth + 1.0f, currentPlayerVistionHeight + 0.5f);
}

void Player::ItemActivateSuperMode(float deltaTime)
{
	// 슈퍼 모드 활성화 상태에서 추가 활성화
	if (bStartSuperMode && bEnableSuperMode)
	{
		// 지속 시간 초기화
		SuperModeTimer.Reset();
		bStartSuperMode = false;
	}

	// 슈퍼 모드 비활성화 상태에서 슈퍼 모드 활성화
	if (bStartSuperMode && !bEnableSuperMode)
	{
		SuperModeTimer.SetTargetTime(SuperModeTime);
		bStartSuperMode = false;
		bEnableSuperMode = true;
		SetColor(Color::LightGreen);

		Level* OwningLevel = GetOwner();
		if (!OwningLevel)
		{
			__debugbreak();
		}
		OwningLevel->SetEnableGlobalVision(true);
	}

	// 슈퍼 모드 활성화
	if (bEnableSuperMode)
	{
		SuperModeTimer.Tick(deltaTime);

		if (SuperModeTimer.IsTimeout())
		{
			SuperModeTimer.Reset();
			bEnableSuperMode = false;
			SetColor(Color::LightYellow);

			Level* OwningLevel = GetOwner();
			if (!OwningLevel)
			{
				__debugbreak();
			}
			OwningLevel->SetEnableGlobalVision(false);
		}
	}
}

void Player::PlayerMovement(float deltaTime)
{
	if (Input::Get().GetKeyDown('A') || Input::Get().GetKeyDown(VK_LEFT))
	{
		HorizontalMove(false, false);
	}
	else if (Input::Get().GetKey('A') || Input::Get().GetKeyDown(VK_LEFT))
	{
		playerRepeatMoveXTimer.Tick(deltaTime);
		HorizontalMove(false, true);
	}
	else if (Input::Get().GetKeyUp('A') || Input::Get().GetKeyDown(VK_LEFT))
	{
		playerRepeatMoveXTimer.Reset();
	}

	if (Input::Get().GetKeyDown('D') || Input::Get().GetKeyDown(VK_RIGHT))
	{
		HorizontalMove(true, false);
	}
	else if (Input::Get().GetKey('D') || Input::Get().GetKeyDown(VK_RIGHT))
	{
		playerRepeatMoveXTimer.Tick(deltaTime);
		HorizontalMove(true, true);
	}
	else if (Input::Get().GetKeyUp('D') || Input::Get().GetKeyDown(VK_RIGHT))
	{
		playerRepeatMoveXTimer.Reset();
	}

	if (Input::Get().GetKeyDown('W') || Input::Get().GetKeyDown(VK_UP))
	{
		VerticalMove(false, false);
	}
	else if (Input::Get().GetKey('W') || Input::Get().GetKeyDown(VK_UP))
	{
		playerRepeatMoveYTimer.Tick(deltaTime);
		VerticalMove(false, true);
	}
	else if (Input::Get().GetKeyUp('W') || Input::Get().GetKeyDown(VK_UP))
	{
		playerRepeatMoveYTimer.Reset();
	}

	if (Input::Get().GetKeyDown('S') || Input::Get().GetKeyDown(VK_DOWN))
	{
		VerticalMove(true, false);
	}
	else if (Input::Get().GetKey('S') || Input::Get().GetKeyDown(VK_DOWN))
	{
		playerRepeatMoveYTimer.Tick(deltaTime);
		VerticalMove(true, true);
	}
	else if (Input::Get().GetKeyUp('S') || Input::Get().GetKeyDown(VK_DOWN))
	{
		playerRepeatMoveYTimer.Reset();
	}
}

void Player::HorizontalMove(bool sign, bool isRepeat)
{
	Vector2 position = Position();

	// 반복/연속 이동
	if (isRepeat)
	{
		if (!playerRepeatMoveXTimer.IsTimeout())
		{
			return;
		}

		playerRepeatMoveXTimer.Reset();
	}

	if (sign) position.x++;
	else position.x--;

	if (gameLevelInterface->CanMove(this, Position(), position))
	{
		SetPosition(position);

		// 레벨의 플레이어 위치 갱신
		GetOwner()->SetPlayerPos(position);
	}
}

void Player::VerticalMove(bool sign, bool isRepeat)
{
	Vector2 position = Position();

	// 반복/연속 이동
	if (isRepeat)
	{
		if (!playerRepeatMoveYTimer.IsTimeout())
		{
			return;
		}

		playerRepeatMoveYTimer.Reset();
	}

	if (sign) position.y++;
	else position.y--;

	if (gameLevelInterface->CanMove(this, Position(), position))
	{
		SetPosition(position);

		// 레벨의 플레이어 위치 갱신
		GetOwner()->SetPlayerPos(position);
	}
}
