#pragma once

#include "Actor/Actor.h"

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player(const Vector2& position);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

private:
	// 플레이어 이동 처리 함수
	// xy(false) = x, xy(true) = y
	// sign(false) = -, sign(true) = +
	// sequence = 연속 입력 인지 확인
	void PlayerMove(const bool xy, const bool sign, const bool sequence, float deltaTime);

private:
	// 플레이어 이동 속도 (단위: 초당 유닛)
	float xMoveSpeed = 7.0f;
	float yMoveSpeed = 3.0f;

	// 좌표가 정수이기 때문에 사이 값 계산을 위해 float 변수 선언
	float xPos = 0.0f;
	float yPos = 0.0f;

	// 게임 레벨 인터페이스
	class IGameLevelInterface* gameLevelInterface = nullptr;
};
