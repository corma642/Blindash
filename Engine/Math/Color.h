#pragma once

// �ؽ�Ʈ ���� ��
enum class Color : unsigned __int8
{
	Blue = 1,
	Green = 2,
	SkyBlue = 3,
	Red = 4,
	Violet = 5,
	Yellow = 6,
	Intensity = 8,
	White = Red | Green | Blue,
};
