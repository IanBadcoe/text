#pragma once

#include "Command.h"

class CommandCollator : public ICommandReceiver, public ICommandSequenceSender
{
public:
	CommandCollator() : _is_ended(false) {}

    // ICommandReceiver
    void ReceiveCommand(const Command& c)
    {
		// as a special case, process end command here so that we do not need a Universe on the server

		if (c._type == Command::Type::Exit)
		{
			_is_ended = true;
			return;
		}

        _wip._commands.push_back(c);
    }

    // ICommandSequenceSender
    virtual void SetCommandSequenceReceiver(ICommandSequenceReceiver* csr) override {
        _destination = csr;
    }

    void EndFrame() {
        CommandSequence ret = _wip;

        _wip._commands.clear();
        _wip._frame++;

        _destination->ReceiveCommandSequence(ret);
    }

	bool IsEnded() const { return _is_ended; }

private:
    CommandSequence _wip;
    ICommandSequenceReceiver*  _destination;
	bool _is_ended;
};

