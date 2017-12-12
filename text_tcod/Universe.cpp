#include "precompiled.h"

#include "Universe.h"

#include "World.h"
#include "Template.h"
#include "Player.h"
#include "Messages.h"

#include <set>

Universe::Universe() : _local_player_id(0)
{
    _world = new World(200, 200);

    CircleTemplate ct(100, 100, 75, 3);

    ct.Apply(_world);

	_players[_local_player_id] = new Player(_local_player_id);

    for (int i = 0; i < 200; i++) {
        if (_world->IsWalkable(Coord(100, i)))
        {
            _world->AddActor(Coord(100, i), _players[_local_player_id]);
            break;
        }
    }

	_input = new InputHandler();
	_input->SetCommandReceiver(this);

	_queue.AddFutureStep(_input, 0.0f);
	_queue.AddFutureStep(_players[_local_player_id], 0.5f);
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
	temp._player_id = _local_player_id;

	_pass_commands_to->ReceiveCommand(temp);
}

void Universe::ProcessCommand(const Command& cmd)
{
	if (!ProcessGlobalCommand(cmd))
	{
		GetPlayer(cmd._player_id)->ReceiveCommand(cmd);
	}
}

bool Universe::ProcessGlobalCommand(const Command & cmd)
{
	return false;
}

void Universe::SetLocalPlayerId(int id)
{
	Player* p = _players[_local_player_id];

	_players.erase(_local_player_id);

	p->SetId(id);

	_local_player_id = id;

	_players[_local_player_id] = p;
}

Player* Universe::GetPlayer(int i)
{
	auto it = _players.find(i);

	if (it == _players.end())
		return nullptr;

	return it->second;
}

const Player* Universe::GetPlayer(int i) const
{
	return const_cast<Universe*>(this)->GetPlayer(i);
}
