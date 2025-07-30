#include "Engine.h"
#include "Level\Level.h"
#include "Utils/Utils.h"

#include <iostream>
#include <Windows.h>

// ���� ���� �ʱ�ȭ
Engine* Engine::instance = nullptr;

BOOL WINAPI ConsoleMessageProcedure(DWORD CtrlType)
{
	switch (CtrlType)
	{
	case CTRL_CLOSE_EVENT:
		// Engine�� �޸� ����
		Engine::Get().CleanUp();
		return false;
	}

	return false;
}

Engine::Engine()
{
	instance = this;

	// �ܼ� Ŀ�� ����
	CONSOLE_CURSOR_INFO info;
	info.bVisible = FALSE; // ���� ����
	info.dwSize = 1; // Ŀ�� ������ 1

	// �ܼ� Ŀ�� ����
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	// �ܼ� â �̺�Ʈ ���
	SetConsoleCtrlHandler(ConsoleMessageProcedure, TRUE);

	// ���� ���� �ε�
	LoadEngineSettings();
}

Engine::~Engine()
{
	CleanUp();
}

void Engine::Run()
{
	// �ý��� �ð��� �и��� ������ �˷���
	//float currentTime = timeGetTime();
	LARGE_INTEGER currentTime;
	LARGE_INTEGER previousTime;
	QueryPerformanceCounter(&currentTime); // RDTSC
	previousTime = currentTime;

	// �ϵ���� �ð��� ���е�(���ļ�) ��������.
	// ���߿� �ʷ� ��ȯ�ϱ� ����
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	// Ÿ�� ������
	float targetFrameRate =
		(settings.framerate == 0.0f) ? 60.f : settings.framerate;

	// Ÿ�� �� ������ �ð�.
	float oneFrameTime = 1.0f / targetFrameRate;

	// GameLoop
	while (true)
	{
		// ���� ���� ���� Ȯ��.
		if (isQuit)
		{
			// ���� ����.
			break;
		}

		// ������ �ð� ���
		// (���� �ð� - ���� �ð�) / ���ļ� = �� ����.
		QueryPerformanceCounter(&currentTime);

		// ������ �ð�.
		float deltaTime =
			(currentTime.QuadPart - previousTime.QuadPart)
			/ static_cast<float>(frequency.QuadPart);

		// �Է��� �ִ��� ����
		input.ProcessInput();

		// ���� ������.
		if (deltaTime >= oneFrameTime)
		{
			BeginPlay();
			Tick(deltaTime);
			Render();

			// ���� FPS ����ϱ�
			char title[50]{};
			sprintf_s(title, 50, "FPS: %f", (1.0f / deltaTime));
			SetConsoleTitleA(title);

			// �ð� ������Ʈ
			previousTime = currentTime;

			// ���� �������� Ű �Է��� ���
			input.SavePreviousKeyStates();
		}
	}

	// ����(�ؽ�Ʈ ���� ������� ��������)
	Utils::SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

void Engine::AddLevel(Level* newLevel)
{
	// ������ �ִ� ���� ����
	if (mainLevel)
	{
		delete mainLevel;
	}

	mainLevel = newLevel;
}

void Engine::CleanUp()
{
	// ���� ����
	SafeDelete(mainLevel);
}

void Engine::Quit()
{
	// ���� �÷��� ����.
	isQuit = true;
}

Engine& Engine::Get()
{
	return *instance;
}

void Engine::BeginPlay()
{
	// ���� ����
	if (mainLevel)
	{
		mainLevel->BeginPlay();
	}
}

void Engine::Tick(float deltaTime)
{
	// ���� ������Ʈ
	if (mainLevel)
	{
		mainLevel->Tick(deltaTime);
	}
}

void Engine::Render()
{
	Utils::SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN);

	// ���� ������
	if (mainLevel)
	{
		mainLevel->Render();
	}
}

void Engine::LoadEngineSettings()
{
	// ���� ���� ���� ����
	FILE* file = nullptr;
	fopen_s(&file, "../Settings/EngineSettings.txt", "rt");

	if (!file)
	{
		std::cout << "Failed to load engine settings." << "\n";
		__debugbreak();
		return;
	}

	// �ε�
	// FP(File Position) �����͸� ���� �ڷ� �ű��
	fseek(file, 0, SEEK_END);

	// �� ��ġ ���ϱ�
	size_t fileSize = ftell(file);

	// �ٽ� ù ��ġ�� �ű��
	rewind(file);

	// ���� ������ ������ ���� �Ҵ�
	char* buffer = new char[fileSize + 1];
	memset(buffer, 0, fileSize + 1);

	// ���� ���� �б�
	size_t readSize = fread(buffer, sizeof(char), fileSize, file);

	// �Ľ�(Parcing, ���� �ؼ� -> �ʿ��� ������ ��� ����)
	char* context = nullptr;
	char* token = nullptr;

	token = strtok_s(buffer, "\n", &context);

	// ���� �м�
	while (token)
	{
		// Ű/�� �и�
		char header[10]{};

		// sscanf_s�� ����� �����Ϸ��� Ű�� �� ������ ��ĭ�� �־�� ��
		sscanf_s(token, "%s", header, 10);

		// ��� ���ڿ� ��
		if (strcmp(header, "width") == 0)
		{
			sscanf_s(token, "width = %d", &settings.width);
		}
		else if (strcmp(header, "height") == 0)
		{
			sscanf_s(token, "height = %d", &settings.height);
		}
		else if (strcmp(header, "framerate") == 0)
		{
			sscanf_s(token, "framerate = %f", &settings.framerate);
		}

		// �� ������ �и�
		token = strtok_s(nullptr, "\n", &context);
	}

	// ���� �޸� ����
	SafeDeleteArray(buffer);

	// ���� �ݱ�
	fclose(file);
}
