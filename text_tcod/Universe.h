#pragma once

#include "Command.h"

#include "World.h"

class Universe : public ICommandSequenceReceiver, public ICommandSender, public ICommandReceiver
{
public:
    Universe();
    ~Universe();

    const World* GetWorld() const { return _world; }

    // Inherited via ICommandSequenceReceiver
    virtual void ReceiveCommandSequence(const CommandSequence & cs) override;

    // Inherited via ICommandSequenceSender
    virtual void SetCommandReceiver(ICommandReceiver * csr) override;

    // Inherited via ICommandReceiver
    virtual void ReceiveCommand(const Command & c) override;

private:
    ICommandReceiver* _pass_commands_to;

    World* _world;
};

