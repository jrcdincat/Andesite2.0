#pragma once
#include "../Objects/Player.h"

class Command {
public: 
	virtual void execute(Player* actor) = 0;
};

class JumpCommand : public Command
{
public:
	virtual void execute(Player* actor);
};

class BiteCommand : public Command
{
public:
	virtual void execute(Player* actor);
};

class MoveLeftCommand : public Command
{
public:
	virtual void execute(Player* actor);
};

class MoveRightCommand : public Command
{
public:
	virtual void execute(Player* actor);
};

class EscapeCommand : public Command
{
public: 
	virtual void execute(Player* actor);
};
