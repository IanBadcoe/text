#pragma once

#include "Coord.h"

#include <vector>

struct Command {
    Coord _from;
    Coord _to;

    int _player;

    enum Type {
        Move
    };

    Type _type;

    Coord::Dir _dir;
};

struct CommandSequence {
    CommandSequence() : _frame(0) {}

    int _frame;

    std::vector<Command> _commands;
};

class ICommandReceiver {
public:
    virtual void ReceiveCommand(const Command& c) = 0;
};

class ICommandSender {
public:
    virtual void SetCommandReceiver(ICommandReceiver* cr) = 0;
};

class ICommandSequenceReceiver {
public:
    virtual void ReceiveCommandSequence(const CommandSequence& cs) = 0;
};

class ICommandSequenceSender {
public:
    virtual void SetCommandSequenceReceiver(ICommandSequenceReceiver* csr) = 0;
};
