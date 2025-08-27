#include "ExpandVisionEffect.h"

// ȿ�� ����� ����� ���ڿ� ������ (������ �ִϸ��̼� ������).
static const ExpandVisionEffectFrame sequence[] =
{
	ExpandVisionEffectFrame("o", 0.01f, Color::LightBlue),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightGreen),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightRed),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightSkyBlue),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightViolet),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightYellow),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightGreen),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightBlue),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightGreen),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightRed),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightSkyBlue),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightViolet),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightYellow),
	ExpandVisionEffectFrame("o", 0.01f, Color::LightGreen),
};

ExpandVisionEffect::ExpandVisionEffect(const Vector2& position)
	: Actor(sequence[0].frame, Color::LightGreen, position)
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

void ExpandVisionEffect::Tick(float deltaTime)
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