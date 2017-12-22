#pragma once

#include "Command.h"

class CommandCollator : public ICommandReceiver, public ICommandSequenceSender
{
public:
	// server "always_send"s because clients require the empty packet to know the frame has happened
	CommandCollator(bool always_send) : _is_ended(false), _always_send(always_send) {}

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

        _wip._frame++;

		if (_always_send || _wip._commands.size())
		{
			_wip._commands.clear();
			_destination->ReceiveCommandSequence(ret);
		}
    }

	bool IsEnded() const { return _is_ended; }

	int GetFrame() const { return _wip._frame; }
	void SetFrame(int i) { _wip._frame = i; }

private:
    CommandSequence _wip;
    ICommandSequenceReceiver*  _destination;
	bool _is_ended;
	bool _always_send;
};

