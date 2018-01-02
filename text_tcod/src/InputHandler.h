#pragma once

#include "Stepable.h"
#include "Command.h"

class TCODConsole;

class InputHandler : public Stepable, public ICommandSender
{
public:
	InputHandler(Universe* u);
	~InputHandler();

	virtual float InnerStep() override;

	// ICommandSender
	virtual void SetCommandReceiver(ICommandReceiver* cr) override;

	// for mouse click cell translation etc...
	void SetConsole(TCODConsole* console) {
		_console = console;
	}

private:
	Coord FixConsoleCell(int x, int y);

	ICommandReceiver* _command_dest;
	TCODConsole* _console;
};

