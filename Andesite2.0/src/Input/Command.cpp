#include "../pch.h"
#include "Command.h"

void JumpCommand::execute(Player* player)
{
	std::cout << "Jump" << std::endl;
	player->isJump = true;
}

void MoveLeftCommand::execute(Player* player)
{
	std::cout << "Move Left" << std::endl;
	player->isMoveLeft = true;
}

void MoveRightCommand::execute(Player* player)
{
	std::cout << "Move Right" << std::endl;
	player->isMoveRight = true;
}

void EscapeCommand::execute(Player* player)
{
	player->isEscape = true;
}