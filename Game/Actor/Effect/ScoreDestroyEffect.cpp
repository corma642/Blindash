#include "ScoreDestroyEffect.h"

// ȿ�� ����� ����� ���ڿ� ������ (������ �ִϸ��̼� ������)
static const ScoreDestroyEffectFrame sequence[] =
{
	ScoreDestroyEffectFrame(".", 0.15f, Color::LightGreen),
	ScoreDestroyEffectFrame(".", 0.15f, Color::LightYellow),
	ScoreDestroyEffectFrame(".", 0.15f, Color::LightSkyBlue),
};

ScoreDestroyEffect::ScoreDestroyEffect(const Vector2& position)
	: Actor(sequence[0].frame, Color::LightGreen, position)
{
	// �ִϸ��̼� ������ ���� ���ϱ�
	effectSequenceCount = sizeof(sequence) / sizeof(sequence[0]);

	// ���� �ִϸ��̼Ǳ��� ����� �ð�
	timer.SetTargetTime(sequence[0].playTime);

	// ���� ����
	color = sequence[0].color;

	// ���� ���� ����
	sortingOrder = SortingOrder::Effect;
}

void ScoreDestroyEffect::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// �ִϸ��̼� ����� ���� Ÿ�̸� ������Ʈ
	timer.Tick(deltaTime);
	if (!timer.IsTimeout())
	{
		return;
	}

	// �ִϸ��̼� ��� �������� Ȯ��
	// �������� ����
	if (currentSequenceIndex == effectSequenceCount - 1)
	{
		Destroy();
		return;
	}

	// ������ ������Ʈ
	++currentSequenceIndex;
	timer.Reset();

	// ���� ���������� ����� �ð����� Ÿ�̸� �缳��
	timer.SetTargetTime(sequence[currentSequenceIndex].playTime);

	// ������ ���� �̹��� ����
	SafeDelete(image);

	// �ִϸ��̼� �����ӿ� ����� ���ڿ��� ���Ϳ� ����
	ChangeImage(sequence[currentSequenceIndex].frame);

	// ���� ����
	color = sequence[currentSequenceIndex].color;
}