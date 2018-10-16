#include "precompiled.h"

#include "Universe.h"

#include "World.h"
#include "Template.h"
#include "Player.h"
#include "Map.h"
#include "Base.h"

Universe::Universe() : _local_player_id(-1), _world(nullptr), _input(nullptr), _map(nullptr)
{
	_input = new InputHandler(this);
	_input->SetCommandReceiver(this);
}

Universe::~Universe()
{
}

void Universe::CreateTestWorld() {
	_world = new World(this);
	_world->SetSize(200, 200);

	//	CircleTemplate ct(100, 100, 75, 3);
	SkyslandTemplate ct(100, 100, 25, 3);
	//	TestTemplate ct;

	ct.Apply(_world);
}

void Universe::ReceiveCommandSequence(const CommandSequence& cs)
{
	for (int i = 0; i < cs._commands.size(); i++)
	{
		ProcessCommand(cs._commands[i]);
	}
}

void Universe::ProcessCommand(const Command& cmd)
{
	if (!ProcessGlobalCommand(cmd))
	{
		if (!_world->ProcessWorldCommand(cmd))
		{
			GetPlayer(cmd._player_id)->ReceiveCommand(cmd);
		}
	}
}

void Universe::SetCommandReceiver(ICommandReceiver* csr)
{
    _pass_commands_to = csr;
}

void Universe::ReceiveCommand(const Command& c)
{
	Command temp = TranslateCommand(c);

	_pass_commands_to->ReceiveCommand(temp);
}

bool Universe::ProcessGlobalCommand(const Command& cmd)
{
	return false;
}

Command Universe::TranslateCommand(Command cmd) {
	cmd._player_id = _local_player_id;

	if (cmd._type == Command::Type::ConsoleCellClick) {
		cmd._type = Command::Type::WorldCellClick;
		cmd._world_cell = _map->MapToWorld(cmd._map_cell);
	}

	return cmd;
}

void Universe::SerialiseTo(std::ostringstream& out) const
{
	assert(_world);

	_world->SerialiseTo(out);

	_stepable_queue.SerialiseTo(out);

	out <<= (int)_players.size();

	for (auto it = _players.begin(); it != _players.end(); it++) {
		out <<= it->first;
		out <<= it->second->GetPos();
	}
}

void Universe::SerialiseFrom(std::istringstream& in)
{
	assert(!_world);

	_world = new World(this);
	_world->SerialiseFrom(in);

	_stepable_queue.SerialiseFrom(in, this);

	int num_players;

	in >>= num_players;

	for (int i = 0; i < num_players; i++)
	{
		int id;
		in >>= id;
		Coord p;
		in >>= p;

		assert(dynamic_cast<Player*>(_world->GetActor(p)));

		_players[id] = static_cast<Player*>(_world->GetActor(p));

		EnsurePlayer(id, false);
	}
}

void Universe::EnsurePlayer(int player_id, bool is_local)
{
	Base* b = _world->GetBase(player_id);

	if (!GetPlayer(player_id))
	{
		Coord place_where;

		if (!b)
		{
			// no base, place in first free space in top of centre line
			// (may not happen any more...)
			for (int i = 100; i < 200; i++) {
				Coord try_pos(100, i);

				if (_world->IsWalkable(try_pos) && !_world->GetActor(try_pos)) {
					place_where = try_pos;

					break;
				}
			}
		} else {
			// if this player has a base in the world, place them near it
			for (IterateNxNSpiralOut it(b->GetPos(), 5); !it.Ended(); it.Next()) {
				Coord try_pos(it.Current());
				
				if (_world->IsWalkable(try_pos) && !_world->GetActor(try_pos)) {
					place_where = try_pos;

					break;
				}
			}
		}

		_players[player_id] = new Player(player_id, _world, place_where);
	}

	if (b) {
		// if we now have a player and a base, tell the base...
		b->SetPlayer(_players[player_id]);
	}

	if (is_local)
	{
		// if this is our locl player, and we didn't know...
		assert(_local_player_id == -1);
		_local_player_id = player_id;
	}
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
