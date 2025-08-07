#pragma once

#include "Level/Level.h"
#include "Interface/GameLevelInterface.h"
#include "Utils/Timer.h"

class GameLevel : public Level, public IGameLevelInterface
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel(const int stageNumber, const float timeLimit);

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	// �̵� ���� ���θ� ��ȯ�ϴ� �Լ�
	virtual bool CanMove(
		class Actor* inActor,
		const Vector2& currentPosition,
		const Vector2& nextPosition) override;

	// �÷��̾� �þ� �ݰ� ���� �Լ�
	virtual void SetPlayerVisionRadius(float newVisionWidth, float newVisionHeight) override;

	// �÷��̾��� �þ� �ݰ��� �þ �ܰ� �׵θ� Ÿ�� ���ϴ� �Լ�
	void CalculateVisionRing();

private:
	void ReadStageFile(const char* fileName);

	// ���� ���ھ� ��� �Լ�
	void PrintRemainingScore();

	// ���� ���� �ð� ��� �Լ�
	void PrintTimeLimit();

	// �÷��̾�� ������ ó��
	void ProcessPlayerAndScore(Actor* inPlayer, Actor* inScore);

	// �÷��̾�� ���� ó��
	void ProcessPlayerAndEnemy(Actor* inPlayer, Actor* inEnemy);

private:
	// Ŭ������� ���� ����
	int remainingScore = 0;

	// �������� ���� �ð�
	float timeLimit = 0.0f;

	// ���� �������� ����
	int currnetStage = 0;
};