#pragma once

//
// �� ������ �ð� ����� �ʿ��� �� ���
// ex) 3�ʰ� �������� ���� Ȯ���� �� �����
//

class Timer
{
public:
	Timer(float targetTime = 0.0f);

	// �ʽð��� �ð��� �귯������ �ϱ� ���� �ʿ���
	void Tick(float deltaTime);

	// ���� �Լ�
	void Reset();

	// ������ �ð��� ����ߴ��� Ȯ���ϴ� �Լ�
	bool IsTimeout() const;

	// Ÿ�̸� �ð� ���� �Լ�
	void SetTargetTime(float newTargetTime);

private:
	// ��� �ð�
	float elapsedTime = 0.0f;

	// ��ǥ �ð�
	float targetTime = 0.0f;
};