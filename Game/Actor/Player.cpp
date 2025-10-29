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

	// �ݺ� �̵� Ÿ�̸� ��ǥ �ð� ����
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

	// �̵� �Է� ó��
	PlayerMovement(deltaTime);

	// ���� ���
	ItemActivateSuperMode(deltaTime);
}

void Player::ItemExpandVisionRange()
{
	float currentPlayerVistionWidth = owner->GetPlayerVisionWidth();
	float currentPlayerVistionHeight = owner->GetPlayerVisionHeight();

	// �þ� �ݰ� ����
	owner->SetPlayerVisionRadius(currentPlayerVistionWidth + 1.0f, currentPlayerVistionHeight + 0.5f);
}

void Player::ItemActivateSuperMode(float deltaTime)
{
	// ���� ��� Ȱ��ȭ ��û�� �ְ� �̹� Ȱ��ȭ�� ���¶�� Ÿ�̸� ����
	if (bStartSuperMode && bEnableSuperMode)
	{
		SuperModeTimer.Reset();		// ���� �ð� ���ʱ�ȭ
		bStartSuperMode = false;	// Ȱ��ȭ ��û �÷��� �ʱ�ȭ
	}

	// ���� ��� Ȱ��ȭ ��û�� �ְ� ��Ȱ��ȭ ���¶�� ���� ��� Ȱ��ȭ
	if (bStartSuperMode && !bEnableSuperMode)
	{
		SuperModeTimer.SetTargetTime(SuperModeTime);	// Ÿ�̸� ��ǥ �ð� ����
		bStartSuperMode = false;						// Ȱ��ȭ ��û �÷��� �ʱ�ȭ
		bEnableSuperMode = true;						// ���� ��� Ȱ��ȭ
		SetColor(Color::LightGreen);					// ���� ���� (���� ��� ǥ��)

		Level* OwningLevel = GetOwner();
		if (!OwningLevel)
		{
			__debugbreak();
		}

		// �۷ι� ���� Ȱ��ȭ (�þ� ���� ��ü�� Ȯ��)
		OwningLevel->SetEnableGlobalVision(true);
	}

	// ���� ��尡 Ȱ��ȭ�� ���¶�� Ÿ�̸� ������Ʈ �� ���� Ȯ��
	if (bEnableSuperMode)
	{
		// Ÿ�̸� ���� (deltaTime��ŭ)
		SuperModeTimer.Tick(deltaTime);

		// Ÿ�̸� ���� �� (���� ��� ���ӽð� ����)
		if (SuperModeTimer.IsTimeout())
		{
			SuperModeTimer.Reset();			// Ÿ�̸� ����
			bEnableSuperMode = false;		// ���� ��� ��Ȱ��ȭ
			SetColor(Color::LightYellow);	// ���� ���� (�⺻ ���� ǥ��)

			Level* OwningLevel = GetOwner();
			if (!OwningLevel)
			{
				__debugbreak();
			}

			// �۷ι� ���� ��Ȱ��ȭ
			OwningLevel->SetEnableGlobalVision(false);
		}
	}
}

void Player::PlayerMovement(float deltaTime)
{
	// ���� �̵� (A �Ǵ� LEFT Ű)
	if (Input::Get().GetKeyDown('A') || Input::Get().GetKeyDown(VK_LEFT))
	{
		HorizontalMove(false, false); // �ʱ� �̵� (�ݺ� �ƴ�)
	}
	else if (Input::Get().GetKey('A') || Input::Get().GetKey(VK_LEFT))  // GetKeyDown���� GetKey�� ���� (���� �Է� Ȯ��)
	{
		playerRepeatMoveXTimer.Tick(deltaTime); // Ÿ�̸� ������Ʈ
		HorizontalMove(false, true); // �ݺ� �̵�
	}
	else if (Input::Get().GetKeyUp('A') || Input::Get().GetKeyUp(VK_LEFT))  // GetKeyDown���� GetKeyUp���� ���� (Ű ����)
	{
		playerRepeatMoveXTimer.Reset(); // Ÿ�̸� ����
	}

	// ������ �̵� (D �Ǵ� RIGHT Ű)
	if (Input::Get().GetKeyDown('D') || Input::Get().GetKeyDown(VK_RIGHT))
	{
		HorizontalMove(true, false); // �ʱ� �̵�
	}
	else if (Input::Get().GetKey('D') || Input::Get().GetKey(VK_RIGHT))
	{
		playerRepeatMoveXTimer.Tick(deltaTime); // Ÿ�̸� ������Ʈ
		HorizontalMove(true, true); // �ݺ� �̵�
	}
	else if (Input::Get().GetKeyUp('D') || Input::Get().GetKeyUp(VK_RIGHT))
	{
		playerRepeatMoveXTimer.Reset();
	}

	// ���� �̵� (W �Ǵ� UP Ű)
	if (Input::Get().GetKeyDown('W') || Input::Get().GetKeyDown(VK_UP))
	{
		VerticalMove(false, false); // �ʱ� �̵�
	}
	else if (Input::Get().GetKey('W') || Input::Get().GetKey(VK_UP))
	{
		playerRepeatMoveYTimer.Tick(deltaTime); // Ÿ�̸� ������Ʈ
		VerticalMove(false, true); // �ݺ� �̵�
	}
	else if (Input::Get().GetKeyUp('W') || Input::Get().GetKeyUp(VK_UP))
	{
		playerRepeatMoveYTimer.Reset();
	}

	// �Ʒ��� �̵� (S �Ǵ� DOWN Ű)
	if (Input::Get().GetKeyDown('S') || Input::Get().GetKeyDown(VK_DOWN))
	{
		VerticalMove(true, false); // �ʱ� �̵�
	}
	else if (Input::Get().GetKey('S') || Input::Get().GetKey(VK_DOWN))
	{
		playerRepeatMoveYTimer.Tick(deltaTime); // Ÿ�̸� ������Ʈ
		VerticalMove(true, true); // �ݺ� �̵�
	}
	else if (Input::Get().GetKeyUp('S') || Input::Get().GetKeyUp(VK_DOWN))
	{
		playerRepeatMoveYTimer.Reset();
	}
}

void Player::HorizontalMove(bool sign, bool isRepeat)
{
	// ���� ��ġ ����
	Vector2 position = Position();

	// �ݺ� �̵� �� Ÿ�̸� Ȯ��
	if (isRepeat)
	{
		if (!playerRepeatMoveXTimer.IsTimeout())
		{
			return; // �̵� �Ӱ찪�� ������ �ʾ����� ������ �̵� �� ��
		}

		// Ÿ�̸�(�̵� �Ӱ찪) ����
		playerRepeatMoveXTimer.Reset();
	}

	// X�� �̵� ����
	if (sign) position.x++;
	else position.x--;

	// �̵� ���� ���� Ȯ��
	if (gameLevelInterface->CanMove(this, Position(), position))
	{
		// ��ġ ������Ʈ
		SetPosition(position);

		// ������ �÷��̾� ��ġ ����
		GetOwner()->SetPlayerPos(position);
	}
}

void Player::VerticalMove(bool sign, bool isRepeat)
{
	Vector2 position = Position();

	// �ݺ� �̵� �� Ÿ�̸� Ȯ��
	if (isRepeat)
	{
		if (!playerRepeatMoveYTimer.IsTimeout())
		{
			return; // �̵� �Ӱ찪�� ������ �ʾ����� ������ �̵� �� ��
		}

		// Ÿ�̸�(�̵� �Ӱ찪) ����
		playerRepeatMoveYTimer.Reset();
	}

	// Y�� �̵� ����
	if (sign) position.y++;
	else position.y--;

	if (gameLevelInterface->CanMove(this, Position(), position))
	{
		// ��ġ ������Ʈ
		SetPosition(position);

		// ������ �÷��̾� ��ġ ����
		GetOwner()->SetPlayerPos(position);
	}
}
