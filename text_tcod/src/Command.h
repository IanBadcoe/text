#pragma once

#include "Coord.h"
#include "Entity.h"

struct Command {
	enum class Type {
		Move,
		Exit,
		ConsoleCellClick,
		WorldCellClick,
		DebugCreateNPC
	};

	Type _type;

	Coord _map_cell;
    Coord _world_cell;

    int _player;

    Coord::Dir _dir;

	int _player_id;
	int _from_frame;

	EntityType _npc_type;
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
