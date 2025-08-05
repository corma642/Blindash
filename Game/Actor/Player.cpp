#include "Player.h"
#include "Input.h"
#include "Engine.h"
#include "Level/Level.h"
#include "Interface/GameLevelInterface.h"
#include "Utils/Timer.h"

#include <iostream>

Player::Player(const Vector2& position)
	: super("D", Color::Yellow, position)
{
	SetSortingOrder(SortingOrder::Player);

	// Ÿ�̸� ���� (�ݺ�/���� �̵� ���� �ð� ����)
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
		Engine::Get().Quit();
		return;
	}

	// �̵� �Է� ó��
	PlayerMovement(deltaTime);

	// GlobalVision
	ItemActivateGlobalVision(deltaTime);

	// ���� ���
	ItemActivateSuperMode(deltaTime);
}

void Player::ItemExpandVisionRange()
{
	Level* OwningLevel = GetOwner();
	if (!OwningLevel)
	{
		__debugbreak();
	}

	float currentPlayerVistionWidth = OwningLevel->GetPlayerVisionWidth();
	float currentPlayerVistionHeight = OwningLevel->GetPlayerVisionHeight();

	OwningLevel->SetPlayerVisionWidth(currentPlayerVistionWidth + 1.0f);
	OwningLevel->SetPlayerVisionHeight(currentPlayerVistionHeight + 0.5f);
}

void Player::ItemActivateGlobalVision(float deltaTime)
{
	// GlobalVision Ȱ��ȭ ���¿��� �߰� Ȱ��ȭ
	if (bStartGlobalVision && bEnableGlobalVision)
	{
		// ���� �ð� �ʱ�ȭ
		GlobalVisionTimer.Reset();
		bStartGlobalVision = false;
	}

	// GlobalVision ��Ȱ��ȭ ���¿��� GlobalVision Ȱ��ȭ
	if (bStartGlobalVision && !bEnableGlobalVision)
	{
		GlobalVisionTimer.SetTargetTime(GlobalVisionTime);
		bStartGlobalVision = false;
		bEnableGlobalVision = true;

		Level* OwningLevel = GetOwner();
		if (!OwningLevel)
		{
			__debugbreak();
		}
		OwningLevel->SetEnableGlobalVision(true);
	}

	// GlobalVision Ȱ��ȭ
	if (bEnableGlobalVision)
	{
		GlobalVisionTimer.Tick(deltaTime);

		if (GlobalVisionTimer.IsTimeout())
		{
			GlobalVisionTimer.Reset();
			bEnableGlobalVision = false;

			Level* OwningLevel = GetOwner();
			if (!OwningLevel)
			{
				__debugbreak();
			}
			OwningLevel->SetEnableGlobalVision(false);
		}
	}
}

void Player::ItemActivateSuperMode(float deltaTime)
{
	// ���� ��� Ȱ��ȭ ���¿��� �߰� Ȱ��ȭ
	if (bStartSuperMode && bEnableSuperMode)
	{
		// ���� �ð� �ʱ�ȭ
		SuperModeTimer.Reset();
		bStartSuperMode = false;
	}

	// ���� ��� ��Ȱ��ȭ ���¿��� ���� ��� Ȱ��ȭ
	if (bStartSuperMode && !bEnableSuperMode)
	{
		SuperModeTimer.SetTargetTime(SuperModeTime);
		bStartSuperMode = false;
		bEnableSuperMode = true;
		SetColor(Color::Green);

		Level* OwningLevel = GetOwner();
		if (!OwningLevel)
		{
			__debugbreak();
		}
		OwningLevel->SetEnableGlobalVision(true);
	}

	// ���� ��� Ȱ��ȭ
	if (bEnableSuperMode)
	{
		SuperModeTimer.Tick(deltaTime);

		if (SuperModeTimer.IsTimeout())
		{
			SuperModeTimer.Reset();
			bEnableSuperMode = false;
			SetColor(Color::Yellow);

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
	if (Input::Get().GetKeyDown('A'))
	{
		HorizontalMove(false, false);
	}
	else if (Input::Get().GetKey('A'))
	{
		playerRepeatMoveXTimer.Tick(deltaTime);
		HorizontalMove(false, true);
	}
	else if (Input::Get().GetKeyUp('A'))
	{
		playerRepeatMoveXTimer.Reset();
	}

	if (Input::Get().GetKeyDown('D'))
	{
		HorizontalMove(true, false);
	}
	else if (Input::Get().GetKey('D'))
	{
		playerRepeatMoveXTimer.Tick(deltaTime);
		HorizontalMove(true, true);
	}
	else if (Input::Get().GetKeyUp('D'))
	{
		playerRepeatMoveXTimer.Reset();
	}

	if (Input::Get().GetKeyDown('W'))
	{
		VerticalMove(false, false);
	}
	else if (Input::Get().GetKey('W'))
	{
		playerRepeatMoveYTimer.Tick(deltaTime);
		VerticalMove(false, true);
	}
	else if (Input::Get().GetKeyUp('W'))
	{
		playerRepeatMoveYTimer.Reset();
	}

	if (Input::Get().GetKeyDown('S'))
	{
		VerticalMove(true, false);
	}
	else if (Input::Get().GetKey('S'))
	{
		playerRepeatMoveYTimer.Tick(deltaTime);
		VerticalMove(true, true);
	}
	else if (Input::Get().GetKeyUp('S'))
	{
		playerRepeatMoveYTimer.Reset();
	}
}

void Player::HorizontalMove(bool sign, bool isRepeat)
{
	Vector2 position = Position();

	// �ݺ�/���� �̵�
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

		// ������ �÷��̾� ��ġ ����
		GetOwner()->SetPlayerPos(position);
	}
}

void Player::VerticalMove(bool sign, bool isRepeat)
{
	Vector2 position = Position();

	// �ݺ�/���� �̵�
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

		// ������ �÷��̾� ��ġ ����
		GetOwner()->SetPlayerPos(position);
	}
}
