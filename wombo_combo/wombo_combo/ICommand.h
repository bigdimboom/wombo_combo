#pragma once

class ICommand
{
public:
	virtual ~ICommand() {}
	virtual void execute() = 0;
	virtual void undo() = 0;
};