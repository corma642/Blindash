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

	// 반복 이동 타이머 목표 시간 설정
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
	// 슈퍼 모드 활성화 요청이 있고 이미 활성화된 상태라면 타이머 리셋
	if (bStartSuperMode && bEnableSuperMode)
	{
		SuperModeTimer.Reset();		// 지속 시간 재초기화
		bStartSuperMode = false;	// 활성화 요청 플래그 초기화
	}

	// 슈퍼 모드 활성화 요청이 있고 비활성화 상태라면 슈퍼 모드 활성화
	if (bStartSuperMode && !bEnableSuperMode)
	{
		SuperModeTimer.SetTargetTime(SuperModeTime);	// 타이머 목표 시간 설정
		bStartSuperMode = false;						// 활성화 요청 플래그 초기화
		bEnableSuperMode = true;						// 슈퍼 모드 활성화
		SetColor(Color::LightGreen);					// 색상 변경 (슈퍼 모드 표시)

		Level* OwningLevel = GetOwner();
		if (!OwningLevel)
		{
			__debugbreak();
		}

		// 글로벌 비전 활성화 (시야 범위 전체로 확장)
		OwningLevel->SetEnableGlobalVision(true);
	}

	// 슈퍼 모드가 활성화된 상태라면 타이머 업데이트 및 만료 확인
	if (bEnableSuperMode)
	{
		// 타이머 진행 (deltaTime만큼)
		SuperModeTimer.Tick(deltaTime);

		// 타이머 만료 시 (슈퍼 모드 지속시간 종료)
		if (SuperModeTimer.IsTimeout())
		{
			SuperModeTimer.Reset();			// 타이머 리셋
			bEnableSuperMode = false;		// 슈퍼 모드 비활성화
			SetColor(Color::LightYellow);	// 색상 변경 (기본 상태 표시)

			Level* OwningLevel = GetOwner();
			if (!OwningLevel)
			{
				__debugbreak();
			}

			// 글로벌 비전 비활성화
			OwningLevel->SetEnableGlobalVision(false);
		}
	}
}

void Player::PlayerMovement(float deltaTime)
{
	// 왼쪽 이동 (A 또는 LEFT 키)
	if (Input::Get().GetKeyDown('A') || Input::Get().GetKeyDown(VK_LEFT))
	{
		HorizontalMove(false, false); // 초기 이동 (반복 아님)
	}
	else if (Input::Get().GetKey('A') || Input::Get().GetKey(VK_LEFT))  // GetKeyDown에서 GetKey로 수정 (지속 입력 확인)
	{
		playerRepeatMoveXTimer.Tick(deltaTime); // 타이머 업데이트
		HorizontalMove(false, true); // 반복 이동
	}
	else if (Input::Get().GetKeyUp('A') || Input::Get().GetKeyUp(VK_LEFT))  // GetKeyDown에서 GetKeyUp으로 수정 (키 떼기)
	{
		playerRepeatMoveXTimer.Reset(); // 타이머 리셋
	}

	// 오른쪽 이동 (D 또는 RIGHT 키)
	if (Input::Get().GetKeyDown('D') || Input::Get().GetKeyDown(VK_RIGHT))
	{
		HorizontalMove(true, false); // 초기 이동
	}
	else if (Input::Get().GetKey('D') || Input::Get().GetKey(VK_RIGHT))
	{
		playerRepeatMoveXTimer.Tick(deltaTime); // 타이머 업데이트
		HorizontalMove(true, true); // 반복 이동
	}
	else if (Input::Get().GetKeyUp('D') || Input::Get().GetKeyUp(VK_RIGHT))
	{
		playerRepeatMoveXTimer.Reset();
	}

	// 위쪽 이동 (W 또는 UP 키)
	if (Input::Get().GetKeyDown('W') || Input::Get().GetKeyDown(VK_UP))
	{
		VerticalMove(false, false); // 초기 이동
	}
	else if (Input::Get().GetKey('W') || Input::Get().GetKey(VK_UP))
	{
		playerRepeatMoveYTimer.Tick(deltaTime); // 타이머 업데이트
		VerticalMove(false, true); // 반복 이동
	}
	else if (Input::Get().GetKeyUp('W') || Input::Get().GetKeyUp(VK_UP))
	{
		playerRepeatMoveYTimer.Reset();
	}

	// 아래쪽 이동 (S 또는 DOWN 키)
	if (Input::Get().GetKeyDown('S') || Input::Get().GetKeyDown(VK_DOWN))
	{
		VerticalMove(true, false); // 초기 이동
	}
	else if (Input::Get().GetKey('S') || Input::Get().GetKey(VK_DOWN))
	{
		playerRepeatMoveYTimer.Tick(deltaTime); // 타이머 업데이트
		VerticalMove(true, true); // 반복 이동
	}
	else if (Input::Get().GetKeyUp('S') || Input::Get().GetKeyUp(VK_DOWN))
	{
		playerRepeatMoveYTimer.Reset();
	}
}

void Player::HorizontalMove(bool sign, bool isRepeat)
{
	// 현재 위치 복사
	Vector2 position = Position();

	// 반복 이동 시 타이머 확인
	if (isRepeat)
	{
		if (!playerRepeatMoveXTimer.IsTimeout())
		{
			return; // 이동 임곗값이 지나지 않았으면 않으면 이동 안 함
		}

		// 타이머(이동 임곗값) 리셋
		playerRepeatMoveXTimer.Reset();
	}

	// X축 이동 적용
	if (sign) position.x++;
	else position.x--;

	// 이동 가능 여부 확인
	if (gameLevelInterface->CanMove(this, Position(), position))
	{
		// 위치 업데이트
		SetPosition(position);

		// 레벨의 플레이어 위치 갱신
		GetOwner()->SetPlayerPos(position);
	}
}

void Player::VerticalMove(bool sign, bool isRepeat)
{
	Vector2 position = Position();

	// 반복 이동 시 타이머 확인
	if (isRepeat)
	{
		if (!playerRepeatMoveYTimer.IsTimeout())
		{
			return; // 이동 임곗값이 지나지 않았으면 않으면 이동 안 함
		}

		// 타이머(이동 임곗값) 리셋
		playerRepeatMoveYTimer.Reset();
	}

	// Y축 이동 적용
	if (sign) position.y++;
	else position.y--;

	if (gameLevelInterface->CanMove(this, Position(), position))
	{
		// 위치 업데이트
		SetPosition(position);

		// 레벨의 플레이어 위치 갱신
		GetOwner()->SetPlayerPos(position);
	}
}
