#include "AddTimeLimitEffect.h"

// ȿ�� ����� ����� ���ڿ� ������ (������ �ִϸ��̼� ������).
static const AddTimeLimitEffectFrame sequence[] =
{
	AddTimeLimitEffectFrame("+1", 0.1f, Color::Green),
	AddTimeLimitEffectFrame("+1", 0.05f, Color::Red),
	AddTimeLimitEffectFrame("+1", 0.1f, Color::Green),
};

AddTimeLimitEffect::AddTimeLimitEffect(const char* frameChar, const Vector2& position)
	: Actor(frameChar, Color::Green, position)
{
	// �ִϸ��̼� ������ ���� ���ϱ�
	effectSequenceCount = sizeof(sequence) / sizeof(sequence[0]);

	// ���� �ִϸ��̼Ǳ��� ����� �ð�.
	timer.SetTargetTime(sequence[0].playTime);

	// ���� ����.
	color = sequence[0].color;

	// ���� ���� ����
	sortingOrder = SortingOrder::Effect;
}

void AddTimeLimitEffect::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// �ִϸ��̼� ����� ���� Ÿ�̸� ������Ʈ
	timer.Tick(deltaTime);
	if (!timer.IsTimeout())
	{
		return;
	}

	// �ִϸ��̼� ��� �������� Ȯ��.
	// �������� ����.
	if (currentSequenceIndex == effectSequenceCount - 1)
	{
		Destroy();
		return;
	}

	// ������ ������Ʈ.
	++currentSequenceIndex;
	timer.Reset();

	// ���� ���������� ����� �ð����� Ÿ�̸� �缳��.
	timer.SetTargetTime(sequence[currentSequenceIndex].playTime);

	// ������ ���� �̹��� ����.
	SafeDelete(image);

	// �ִϸ��̼� �����ӿ� ����� ���ڿ��� ���Ϳ� ����.
	ChangeImage(sequence[currentSequenceIndex].frame);

	// ���� ����.
	color = sequence[currentSequenceIndex].color;
}