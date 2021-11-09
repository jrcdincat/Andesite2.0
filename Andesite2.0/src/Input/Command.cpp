#include "../pch.h"
#include "Command.h"

void JumpCommand::execute(Player* player)
{
	std::cout << "Jump" << std::endl;
	player->isJump = true;
}

void JumpLeftCommand::execute(Player* player)
{
	std::cout << "Jump Left" << std::endl;
	player->isJump = true;
	player->isMoveLeft = true;
	player->isMoveRight = false;
}

void JumpRightCommand::execute(Player* player)
{
	std::cout << "Jump Right" << std::endl;
	player->isJump = true;
	player->isMoveRight = true;
	player->isMoveLeft = false;
}

void MoveLeftCommand::execute(Player* player)
{
	std::cout << "Move Left" << std::endl;
	player->isMoveLeft = true;
	player->isMoveRight = false;
}

void MoveRightCommand::execute(Player* player)
{
	std::cout << "Move Right" << std::endl;
	player->isMoveRight = true;
	player->isMoveLeft = false;
}

void EscapeCommand::execute(Player* player)
{
	player->isEscape = true;
}