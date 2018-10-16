#include "precompiled.h"

#include "Player.h"

#include "World.h"
#include "Base.h"
#include "Miner.h"
#include "Universe.h"

std::vector<TCODColor> Player::s_foreground;
int Player::s_max_players = 0;

static Entity* CreatePlayer(std::istringstream& in, World* w) {
	return new Player(in);
}

EntityCreator Player::s_creator(EntityType::Player, CreatePlayer);

TCODColor Player::GetColour() const {
	return s_foreground[_id];
}

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

void Player::BecomeIdle() {
}

void Player::ExecuteCommand()
{
	if (!_command_queue.size())
		return;

	Command cmd = _command_queue.front();
	_command_queue.pop();

	World* w = GetWorld();
	assert(w);

	Universe* u = w->GetUniverse();

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
		if (!w->ComputePath(this, GetPos(), cmd._world_cell, _current_path)) {
			// state unchanged...
			return;
		}

		_state = State::FollowingPath;

		// first step on path should follow immediately
		return;
	}

	case Command::Type::DebugCreateNPC:
	{
		// we try to spawn from our base
		Coord where = w->GetBase(cmd._player_id)->GetPos();

		IterateNxNSpiralOut it(where, 5);

		while (!it.Ended()) {
			Coord pos = it.Current();
			const Terrain* t = w->GetTerrain(pos);

			if (t && t->IsWalkable() && w->GetActor(pos) == nullptr) {
				new Miner(w, pos, u->GetPlayer(cmd._player_id));

				return;
			}

			it.Next();
		}

		return;
	}

	default:
		assert(false);
	}

	assert(false);
}
