#pragma once

// DLL �ܺη� ���ø��� ������ �� �߻��ϴ� ��� ��Ȱ��ȭ
#pragma warning(disable: 4251)

// ���� ������ �ּҸ� ��ȯ�� �� �߻��ϴ� ��� ��Ȱ��ȭ
#pragma warning(disable: 4172)

// �޸� ���� �����
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

#if BuildEngineDLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif


// �޸� ���� �Լ�
template<typename T>
void SafeDelete(T*& target) // �������� ���۷��� �ޱ�
{
	if (target)
	{
		delete target;
		target = nullptr;
	}
}

// �迭 �޸� ���� �Լ�
template<typename T>
void SafeDeleteArray(T*& target) // �������� ���۷��� �ޱ�
{
	if (target)
	{
		delete[] target;
		target = nullptr;
	}
}