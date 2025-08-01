#pragma once

#include "Actor/Actor.h"

class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player(const Vector2& position);

	virtual void Tick(float deltaTime) override;

private:
	// 플레이어 이동 속도 (단위: 초당 유닛)
	float xMoveSpeed = 10.0f;
	float yMoveSpeed = 5.0f;

	// 좌표가 정수이기 때문에 사이 값 계산을 위해 float 변수 선언
	float xPos = 0.0f;
	float yPos = 0.0f;
};
