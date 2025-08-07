#include "ExpandVisionEffect.h"

// 효과 재생에 사용할 문자열 시퀀스 (일종의 애니메이션 프레임).
static const ExpandVisionEffectEffectFrame sequence[] =
{
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::Blue),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::Green),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::Red),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::SkyBlue),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::Violet),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::Yellow),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::Green),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::Blue),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::Green),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::Red),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::SkyBlue),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::Violet),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::Yellow),
	ExpandVisionEffectEffectFrame("o", 0.01f, Color::Green),
};

ExpandVisionEffect::ExpandVisionEffect(const Vector2& position)
	: Actor(sequence[0].frame, Color::Green, position)
{
	// 애니메이션 시퀀스 개수 구하기
	effectSequenceCount = sizeof(sequence) / sizeof(sequence[0]);

	// 다음 애니메이션까지 대기할 시간.
	timer.SetTargetTime(sequence[0].playTime);

	// 색상 설정.
	color = sequence[0].color;

	// 정렬 순서 설정
	sortingOrder = SortingOrder::Effect;
}

void ExpandVisionEffect::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 애니메이션 재생을 위한 타이머 업데이트
	timer.Tick(deltaTime);
	if (!timer.IsTimeout())
	{
		return;
	}

	// 애니메이션 재생 끝났는지 확인.
	// 끝났으면 삭제.
	if (currentSequenceIndex == effectSequenceCount - 1)
	{
		Destroy();
		return;
	}

	// 프레임 업데이트.
	++currentSequenceIndex;
	timer.Reset();

	// 다음 시퀀스에서 재생할 시간으로 타이머 재설정.
	timer.SetTargetTime(sequence[currentSequenceIndex].playTime);

	// 액터의 기존 이미지 제거.
	SafeDelete(image);

	// 애니메이션 프레임에 사용할 문자열을 액터에 복사.
	ChangeImage(sequence[currentSequenceIndex].frame);

	// 색상 설정.
	color = sequence[currentSequenceIndex].color;
}