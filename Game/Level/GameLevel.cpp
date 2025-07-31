#include "GameLevel.h"
#include "Math/Vector2.h"

#include <iostream>

GameLevel::GameLevel()
{
	ReadStageFile("Stage_01.txt");
}

void GameLevel::Render()
{
	super::Render();
}

void GameLevel::ReadStageFile(const char* fileName)
{
	// ���� ���� ��� �ϼ�
	char filePath[256]{};
	sprintf_s(filePath, 256, "../Assets/%s", fileName);

	FILE* mapFile = nullptr;
	fopen_s(&mapFile, filePath, "rb");

	// ����ó��
	if (!mapFile)
	{
		std::cout << "�� ���� �б� ����: " << fileName << "\n";
		__debugbreak();
		return;
	}

	// �Ľ�(Parcing, �ؼ�)
	fseek(mapFile, 0, SEEK_END);
	size_t fileSize = ftell(mapFile);
	rewind(mapFile);

	// Ȯ���� ���� ũ�⸦ Ȱ���� ���� �Ҵ�
	char* buffer = new char[fileSize + 1];
	memset(buffer, 0, fileSize + 1);
	size_t readSize = fread(buffer, sizeof(char), fileSize, mapFile);

	// �迭 ��ȸ�� ���� �ε��� ����
	int index = 0;

	// ���ڿ� ���� ��
	int size = static_cast<int>(readSize);

	// x, y�� ��ǥ
	Vector2 pos;

	// ���� �迭 ��ȸ
	while (index < size)
	{
		// �� ���� Ȯ��
		char mapCharacter = buffer[index++];

		// ���� ���� ó��
		if (mapCharacter == '\n')
		{
			// ���� �ٷ� �ѱ�鼭, x ��ǥ �ʱ�ȭ
			pos.x = 0;
			++pos.y;

			// Todo: �׽�Ʈ�뵵. ��ü ���� ��, ���߿� �����ؾ� ��
			std::cout << "\n";

			continue;
		}

		// �� ���ں� ó��
		switch (mapCharacter)
		{
		case '8': // Wall ���� ����
			//AddActor(new Wall(pos));
			std::cout << "8";
			break;
		case '.': // Score ���� ����
			// Ground ���� ����
			//AddActor(new Ground(pos));
			std::cout << ".";
			break;
		case 'D':
			// Ground�� ���� ����
			//AddActor(new Ground(pos));

			// Player ���� ����
			//AddActor(new Player(pos));
			std::cout << "D";
			break;
		//case 'b':
		//	// ���� ���� ����
		//	AddActor(new Ground(pos));

		//	// Box ���� ����
		//	AddActor(new Box(pos));
		//	break;
		//case 't':
		//	// Target ���� ����
		//	AddActor(new Target(pos));

		//	// ��ǥ ���� ���� ó��
		//	targetScore++;
		//	break;
		}
		// x��ǥ ���� ó��
		pos.x++;
	}

	// ���� ����
	delete[] buffer;

	// ���� �ݱ�
	fclose(mapFile);
}
