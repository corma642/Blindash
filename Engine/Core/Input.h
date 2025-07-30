#pragma once

#include "Core.h"

class ENGINE_API Input
{
	// friend ����
	friend class Engine;

	// Ű �Է� ���� ����ü
	struct KeyState
	{
		// ���� �����ӿ� Ű�� ���ȴ��� ����
		bool isKeyDown = false;

		// ���� �����ӿ� Ű�� ���ȴ��� ����
		bool previousKeyDown = false;
	};

public:
	Input();

	// Ű �Է� Ȯ�� �Լ�
	bool GetKey(int keyCode);
	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);

	static Input& Get();

private:
	void ProcessInput();
	void SavePreviousKeyStates();

private:
	static Input* instance;

	// Ű �Է� ���� ���� ����
	KeyState keyStates[255]{ };
};