#pragma once

#include "Actor/Actor.h"

class Enemy : public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)

public:
	Enemy(const Vector2 position);

	virtual void Tick(float deltaTime);

private:

};

