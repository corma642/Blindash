#pragma once

#include "Level/Level.h"
#include "Interface/GameLevelInterface.h"
#include "Utils/Timer.h"

class GameLevel : public Level, public IGameLevelInterface
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel(const int stageNum);

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
	void PrintScore();

	// �÷��̾�� ������ ó��
	void ProcessPlayerAndScore(Actor* inPlayer, Actor* inScore);

	// �÷��̾�� ���� ó��
	void ProcessPlayerAndEnemy(Actor* inPlayer, Actor* inEnemy);

private:
	// ���� ����
	int currentScore = 0;

	// Ŭ������� ���� ����
	int remainingScore = 0;

	// ���� �������� ����
	int currnetStage = 0;
};