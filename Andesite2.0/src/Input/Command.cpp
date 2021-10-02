#include "../pch.h"
#include "Command.h"

void JumpCommand::execute(Player* player)
{
	std::cout << "Jump" << std::endl;
}

void MoveLeftCommand::execute(Player* player)
{
	std::cout << "Move Left" << std::endl;
	player->MoveLeft();
}

void MoveRightCommand::execute(Player* player)
{
	std::cout << "Move Right" << std::endl;
	player->MoveRight();
}

void BiteCommand::execute(Player* player)
{
	std::cout << "Bite" << std::endl;
}