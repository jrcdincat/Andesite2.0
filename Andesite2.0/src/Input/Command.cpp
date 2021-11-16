#include "../pch.h"
#include "Command.h"

void JumpCommand::execute(Player* player)
{
	player->isJump = true;
}

void JumpLeftCommand::execute(Player* player)
{
	player->isJump = true;
	player->isMoveLeft = true;
	player->isMoveRight = false;
}

void JumpRightCommand::execute(Player* player)
{
	player->isJump = true;
	player->isMoveRight = true;
	player->isMoveLeft = false;
}

void MoveLeftCommand::execute(Player* player)
{
	player->isMoveLeft = true;
	player->isMoveRight = false;
}

void MoveRightCommand::execute(Player* player)
{
	player->isMoveRight = true;
	player->isMoveLeft = false;
}
