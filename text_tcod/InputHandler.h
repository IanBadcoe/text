#pragma once

#include "Actor.h"
#include "Command.h"

class InputHandler : public Stepable, public ICommandSender
{
public:
	InputHandler();

	virtual float InnerStep() override;

	// ICommandSender
	virtual void SetCommandReceiver(ICommandReceiver* cr) override;

private:
	ICommandReceiver* _command_dest;
};

