#pragma once

#include "Math/Vector2.h"

class Actor;

// 게임 레벨과 관련되어 수행할 함수들을 모아둔 인터페이스
class IGameLevelInterface
{
public:
	// 해당 위치로 이동할 수 있는지 검사하는 함수
	virtual bool CanMove(
		class Actor* inActor,
		const Vector2& currentPosition,
		const Vector2& nextPosition) = 0;
};