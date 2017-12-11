#pragma once

#include "Command.h"

class CommandCollator : public ICommandReceiver, public ICommandSequenceSender
{
public:
    // ICommandReceiver
    void ReceiveCommand(const Command& c)
    {
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

private:
    CommandSequence _wip;
    ICommandSequenceReceiver*  _destination;
};

