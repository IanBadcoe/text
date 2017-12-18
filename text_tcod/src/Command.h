#pragma once

#include "Coord.h"

struct Command {
    Coord _from;
    Coord _to;

    int _player;

    enum class Type {
        Move,
		Exit
    };

    Type _type;

    Coord::Dir _dir;

	int _player_id;
	int _from_frame;
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
