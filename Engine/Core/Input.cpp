#include "Input.h"

#include <Windows.h>

// static ���� ����
Input* Input::instance = nullptr;

Input::Input()
{
	// �̱��� ������ ���� instance ���� ����
	instance = this;
}

Input& Input::Get()
{
	return *instance;
}

bool Input::GetKey(int keyCode)
{
	// Ű�� ������ ���� ��
	return keyStates[keyCode].isKeyDown;
}

bool Input::GetKeyDown(int keyCode)
{
	// ������ ������ �ʰ� �ִٰ�, ó�� Ű�� ���� ��
	return keyStates[keyCode].isKeyDown && !keyStates[keyCode].previousKeyDown;
}

bool Input::GetKeyUp(int keyCode)
{
	// Ű�� ������ �ִٰ� Ű�� ���� ��
	return !keyStates[keyCode].isKeyDown && keyStates[keyCode].previousKeyDown;
}

void Input::ProcessInput()
{
	// Ű �Է� Ȯ��
	for (int i = 0; i < 255; ++i)
	{
		keyStates[i].isKeyDown = GetAsyncKeyState(i) & 0x8000;
	}
}

void Input::SavePreviousKeyStates()
{
	// ���� �������� Ű �Է��� ���
	for (int i = 0; i < 255; ++i)
	{
		keyStates[i].previousKeyDown = keyStates[i].isKeyDown;
	}
}