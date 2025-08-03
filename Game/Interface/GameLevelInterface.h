#pragma once

#include "Math/Vector2.h"

class Actor;

// ���� ������ ���õǾ� ������ �Լ����� ��Ƶ� �������̽�
class IGameLevelInterface
{
public:
	// �ش� ��ġ�� �̵��� �� �ִ��� �˻��ϴ� �Լ�
	virtual bool CanMove(
		const class Actor* inActor,
		const Vector2& currentPosition,
		const Vector2& nextPosition) = 0;
};