#include <iostream>
#include "Game/Game.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// std::cout �񵿱� ����
	// c stdio���� ����ȭ�� ����, C++�� << ȣ�� ����� ����
	std::ios::sync_with_stdio(false);
	std::cout.tie(nullptr);

	Game game;
	game.Run();
}