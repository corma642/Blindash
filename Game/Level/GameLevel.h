#pragma once

#include "Level/Level.h"

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();

	virtual void Render() override;

private:
	void ReadStageFile(const char* fileName);
};

