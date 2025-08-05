#pragma once

#include "Level/Level.h"
#include "Interface/GameLevelInterface.h"
#include "Utils/Timer.h"

class GameLevel : public Level, public IGameLevelInterface
{
	RTTI_DECLARATIONS(GameLevel, Level)

public:
	GameLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Render() override;

	// �̵� ���� ���θ� ��ȯ�ϴ� �Լ�
	virtual bool CanMove(
		class Actor* inActor,
		const Vector2& currentPosition,
		const Vector2& nextPosition) override;

private:
	void ReadStageFile(const char* fileName);

	// ���� ���ھ� ��� �Լ�
	void PrintScore();

	// �÷��̾�� ������ ó��
	void ProcessPlayerAndScore(Actor* inPlayer, Actor* inScore);

	// �÷��̾�� ���� ó��
	void ProcessPlayerAndEnemy(Actor* inPlayer, Actor* inEnemy);

private:
	// Ŭ������� ���� ����(���ھ�)
	int remainingScore = 0;

	// ���� x, y�� ��ǥ
	Vector2 stagePos = Vector2::Zero;

	// �÷��̾��� ���� ó���� ���� ����
	bool isPlayerDead = false;

	// Todo: ���� �þ� on/off �ɼ�
	bool useDark = true;
};