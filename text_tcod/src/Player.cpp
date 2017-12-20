#include "precompiled.h"

#include "Player.h"

#include "Map.h"
#include "World.h"
#include "Coord.h"

std::vector<TCODColor> Player::s_foreground;
int Player::s_max_players = 0;

static Entity* CreatePlayer(std::istringstream& in) {
	return new Player(in);
}

EntityCreator Player::s_creator(EntityType::Player, CreatePlayer);

Player::Player(std::istringstream& in) : Actor(in) {
    in >>= _id;

    int queue_size;
    in >>= queue_size;

    for (int i = 0; i < queue_size; i++)
    {
        Command c;

        in >>= c;
        _command_queue.push(c);
    }
}

void Player::SerialiseTo(std::ostringstream& out) const
{
    Actor::SerialiseTo(out);

    out <<= _id;

    out <<= (int)_command_queue.size();

    std::queue<Command> temp = _command_queue;

    while (temp.size())
    {
        out <<= temp.front();
        temp.pop();
    }
}

float Player::InnerStep()
{
	// does nothing if there isn't one, or it cannot be run in this state
	// otherwise leaves us in a state that will return an appropriate delay below
	ExecuteCommand();

	// pump the state machine
	return Actor::InnerStep();
}

void Player::ExecuteCommand()
{
	if (!_command_queue.size())
		return;

	Command cmd = _command_queue.front();
	_command_queue.pop();

	World* w = GetWorld();
	assert(w);

	switch (cmd._type)
	{
	case Command::Type::Move:
	{
		Coord::Dir d = cmd._dir;

		Coord old_pos = GetPos();
		_dest = old_pos.Step(d);

		_state = State::SingleStep;

		return;
	}

	case Command::Type::WorldCellClick:
	{
		if (!GetWorld()->ComputePath(this, GetPos(), cmd._world_cell, _current_path)) {
			// state unchanged...
			return;
		}

		_state = State::FollowingPath;

		// first step on path should follow immediately
		return;
	}
	}

	assert(false);
}
