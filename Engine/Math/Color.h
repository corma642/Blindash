#pragma once

// �ؽ�Ʈ ���� ��
enum class Color : unsigned __int8
{
	Blue = 1,
	Green = 2,
	Red = 4,
	Intensity = 8,
	White = Red | Green | Blue,
};
