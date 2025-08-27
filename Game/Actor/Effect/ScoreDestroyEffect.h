#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"

// �ִϸ��̼� ����Ʈ ������ ����ü.
struct ScoreDestroyEffectFrame
{
	ScoreDestroyEffectFrame(const char* frame, float playTime = 0.05f, Color color = Color::LightGreen)
		: playTime(playTime), color(color)
	{
		// ���ڿ� ����.
		size_t length = strlen(frame) + 1;
		this->frame = new char[length];
		strcpy_s(this->frame, length, frame);
	}

	~ScoreDestroyEffectFrame()
	{
		SafeDelete(frame);
	}

	// ���ڿ� ���� (ȭ�鿡 ������ ���ڿ�).
	char* frame = nullptr;

	// ��� �ð�.
	float playTime = 0.0f;

	// ����Ʈ ���� 
	Color color = Color::LightWhite;
};

class ScoreDestroyEffect : public Actor
{
	RTTI_DECLARATIONS(ScoreDestroyEffect, Actor)

public:
	ScoreDestroyEffect(const Vector2& position);

	virtual void Tick(float deltaTime) override;

private:
	// ������ ���ڿ� ��(�迭 ���� ��).
	int effectSequenceCount = 0;

	// ���� �������� ������ �ε���.
	int currentSequenceIndex = 0;

	// �ִϸ��̼� ����� ����� Ÿ�̸�.
	// ������ ���̿� �ð� ����.
	Timer timer;
};