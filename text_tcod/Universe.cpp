#include "precompiled.h"

#include "Universe.h"

#include "World.h"
#include "Template.h"
#include "player.h"

Universe::Universe(int player_id) : _is_ended(false), _local_player(nullptr), _player_id(player_id)
{
    _world = new World(200, 200);

    CircleTemplate ct(100, 100, 75, 3);

    ct.Apply(_world);

	_local_player = new Player(_player_id);

    for (int i = 0; i < 200; i++) {
        if (_world->IsWalkable(Coord(100, i)))
        {
            _world->AddActor(Coord(100, i), _local_player);
            break;
        }
    }

	_input = new InputHandler();
	_input->SetCommandReceiver(this);

	_queue.AddFutureStep(_input, 0.0f);
	_queue.AddFutureStep(_local_player, 0.5f);
}


Universe::~Universe()
{
}

void Universe::ReceiveCommandSequence(const CommandSequence & cs)
{
	for (int i = 0; i < cs._commands.size(); i++)
	{
		ProcessCommand(cs._commands[i]);
	}
}

void Universe::SetCommandReceiver(ICommandReceiver * csr)
{
    _pass_commands_to = csr;
}

void Universe::ReceiveCommand(const Command & c)
{
	Command temp(c);
	temp._player_id = _player_id;

	_pass_commands_to->ReceiveCommand(temp);
}

void Universe::ProcessCommand(const Command& cmd)
{
	if (cmd._player_id == _player_id)
	{
		if (!ProcessGlobalCommand(cmd))
		{
			_local_player->ReceiveCommand(cmd);
		}
	}
}

bool Universe::ProcessGlobalCommand(const Command & cmd)
{
	if (cmd._type == Command::Type::Exit)
	{
		_is_ended = true;

		return true;
	}

	return false;
}
