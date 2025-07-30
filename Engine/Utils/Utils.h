#pragma once

#include <Windows.h>
#include "Math/Vector2.h"
#include "Math/Color.h"

// 프로젝트에서 다양하게 사용할 유틸리티 함수 모음
namespace Utils
{
	// 두 값을 서로 바꿀 때 사용하는 함수
	template<typename T>
	void Swap(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

	// 콘솔 출력 핸들 가져오는 함수
	inline HANDLE GetConsoleHandle()
	{
		return GetStdHandle(STD_OUTPUT_HANDLE);
	}

	// 콘솔 커서 위치 이동 함수 (COORD ver)
	inline void SetConsolePosition(COORD coord)
	{
		static HANDLE handle = GetConsoleHandle();
		SetConsoleCursorPosition(handle, coord);
	}

	// 콘솔 커서 위치 이동 함수 (Vector2 ver)
	inline void SetConsolePosition(const Vector2& position)
	{
		SetConsolePosition(static_cast<COORD>(position));
	}

	// 콘솔 텍스트 색상 설정 함수 (WORD ver)
	inline void SetConsoleTextColor(WORD color)
	{
		static HANDLE handle = GetConsoleHandle();
		SetConsoleTextAttribute(handle, color);
	}

	// 콘솔 텍스트 색상 설정 함수 (Color ver)
	inline void SetConsoleTextColor(Color color)
	{
		SetConsoleTextColor(static_cast<WORD>(color));
	}
}
