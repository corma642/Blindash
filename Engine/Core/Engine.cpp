#include "Engine.h"
#include "Level\Level.h"
#include "Utils/Utils.h"
#include "Render/ScreenBuffer.h"
#include "../Game/Game/Game.h"

#include <iostream>

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

	// ���� ���� ���
	SetConsoleTitleA("Blindash");

	// �ܼ� Ŀ�� ����
	CONSOLE_CURSOR_INFO info;
	info.bVisible = FALSE; // ���� ����
	info.dwSize = 1; // Ŀ�� ������ 1

	// �ܼ� Ŀ�� ����
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	// ���� ���� �ε�
	LoadEngineSettings();

	// �̹��� ���� ���� / �ֿܼ� ���� ���� ����
	Vector2 screenSize(settings.width, settings.height);
	imageBuffer = new ImageBuffer((screenSize.x + 1) * screenSize.y + 1);

	// �̹��� ���� �ʱ�ȭ
	ClearImageBuffer();

	// �� ���� ���� ����
	renderTargets[0] = new ScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE), screenSize);
	renderTargets[1] = new ScreenBuffer(screenSize);

	// ���� ��ȯ.
	Present();

	// �ܼ� â �̺�Ʈ ���
	SetConsoleCtrlHandler(ConsoleMessageProcedure, TRUE);

	// cls ȣ��.
	system("cls");
}

Engine::~Engine()
{
	CleanUp();
}

void Engine::Run()
{
	LARGE_INTEGER currentTime; // ���� �ð�
	LARGE_INTEGER previousTime; // ���� �ð�
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

	// ���� ����
	while (true)
	{
		// ���� ���� ���� Ȯ��.
		if (isQuit)
		{
			break;
		}

		// ������ �ð� ���
		// (���� �ð� - ���� �ð�) / ���ļ� = �� ����.
		QueryPerformanceCounter(&currentTime);

		// ������ �ð�.
		float deltaTime =
			(currentTime.QuadPart - previousTime.QuadPart)
			/ static_cast<float>(frequency.QuadPart);

		// �Է� ó��
		input.ProcessInput();

		// ���� ������.
		if (deltaTime >= oneFrameTime)
		{
			// ���� �̺�Ʈ ó��
			BeginPlay();
			Tick(deltaTime);
			Render();

			// �ð� ������Ʈ
			previousTime = currentTime;

			// ���� �������� Ű �Է��� ���
			input.SavePreviousKeyStates();

			if (mainLevel)
			{
				// ���� �����ӿ� �߰� �� ���� ��û�� ���� ó��
				mainLevel->ProcessAddAndDestroyActors();

				// ���� ���� ó��
				if (mainLevel->GameOver())
				{
					Sleep(2000);
					OnGameOver();
				}
			}

			// ���� ���� ��û�� �������� ó��.
			if (levelChangeRequested && changeRequestedLevel)
			{
				AddLevel(changeRequestedLevel);
				changeRequestedLevel = nullptr;
				levelChangeRequested = false;
			}
		}
	}

	// ���� ���� ���� ��, ����
	// �ܼ� �ؽ�Ʈ�� ������� ����
	Utils::SetConsoleTextColor(Color::White);
}

void Engine::WriteToBuffer(const Vector2& position, const char* image, Color color, int sortingOrder)
{
	// ���ڿ� ����.
	int length = static_cast<int>(strlen(image));

	for (int i = 0; i < length; ++i)
	{
		// ����� ���� ��ġ
		int index = (position.y * (settings.width)) + position.x + i;

		// ���� ��ġ�� �׷��� �̹����� ���� ������ �� ������ ��ȯ
		if (imageBuffer->sortingOrderArray[index] > sortingOrder)
		{
			return;
		}

		// ���ۿ� ����/���� ���.
		imageBuffer->charInfoArray[index].Char.AsciiChar = image[i];
		imageBuffer->charInfoArray[index].Attributes = (WORD)color;
		imageBuffer->sortingOrderArray[index] = sortingOrder;
	}
}

void Engine::AddLevel(Level* newLevel)
{
	// ������ �ִ� ���� ����
	SafeDelete(mainLevel);

	mainLevel = newLevel;
}

void Engine::ChangeLevel(Level* newLevel)
{
	// ���� ���� ��û �÷��� ����.
	levelChangeRequested = true;

	// ������ ���� ����.
	changeRequestedLevel = newLevel;
}

void Engine::CleanUp()
{
	// ���� ����
	SafeDelete(mainLevel);

	// ���� ���� ����.
	SafeDelete(imageBuffer);

	// ���� Ÿ�� ����.
	SafeDelete(renderTargets[0]);
	SafeDelete(renderTargets[1]);
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

void Engine::OnInitialized()
{
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

void Engine::Clear()
{
	ClearImageBuffer();
	GetRenderer()->Clear();
}

void Engine::Render()
{
	// ȭ�� �����.
	Clear();

	// ���� ������
	if (mainLevel)
	{
		mainLevel->Render();
	}

	// ����ۿ� ������ ����.
	GetRenderer()->Render(imageBuffer->charInfoArray);

	// ���� ��ȯ.
	Present();
}

void Engine::Present()
{
	// ���� ��ȯ.
	SetConsoleActiveScreenBuffer(GetRenderer()->buffer);

	// �ε��� ������. 1->0, 0->1.
	currentRenderTargetIndex = 1 - currentRenderTargetIndex;
}

ScreenBuffer* Engine::GetRenderer() const
{
	return renderTargets[currentRenderTargetIndex];
}

void Engine::ClearImageBuffer()
{
	// ���� ���� �����.
	for (int y = 0; y < settings.height; ++y)
	{
		for (int x = 0; x < settings.width; ++x)
		{
			int index = (y * (settings.width)) + x;
			CHAR_INFO& buffer = imageBuffer->charInfoArray[index];
			buffer.Char.AsciiChar = ' ';
			buffer.Attributes = 0;
			imageBuffer->sortingOrderArray[index] = -1;
		}

		// �� �� ���� ���� ���� �߰�.
		int index = (y * (settings.width)) + settings.width;
		CHAR_INFO& buffer = imageBuffer->charInfoArray[index];
		buffer.Char.AsciiChar = '\n';
		buffer.Attributes = 0;
		imageBuffer->sortingOrderArray[index] = -1;
	}

	// �������� �� ���� �߰�.
	int index = (settings.width) * settings.height + 1;
	CHAR_INFO& buffer = imageBuffer->charInfoArray[index];
	buffer.Char.AsciiChar = '\0';
	buffer.Attributes = 0;
	imageBuffer->sortingOrderArray[index] = -1;
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
		if (strcmp(header, "framerate") == 0)
		{
			sscanf_s(token, "framerate = %f", &settings.framerate);
		}
		else if (strcmp(header, "width") == 0)
		{
			sscanf_s(token, "width = %d", &settings.width);
		}
		else if (strcmp(header, "height") == 0)
		{
			sscanf_s(token, "height = %d", &settings.height);
		}

		// �� ������ �и�
		token = strtok_s(nullptr, "\n", &context);
	}

	// ���� �޸� ����
	SafeDeleteArray(buffer);

	// ���� �ݱ�
	fclose(file);
}
