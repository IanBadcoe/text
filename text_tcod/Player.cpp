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

    out <<= _command_queue.size();

    std::queue<Command> temp = _command_queue;

    while (temp.size())
    {
        out <<= temp.front();
        temp.pop();
    }
}

float Player::InnerStep()
{
	if (!_command_queue.size())
	{
		return 0.1f;
	}

	Command c = _command_queue.front();
	_command_queue.pop();

	return ExecuteCommand(c);
}

DisplayChar Player::Disp() const
{
	return DisplayChar('@', s_foreground[_id]);
}

float Player::ExecuteCommand(const Command& cmd)
{
	World* w = GetWorld();
	assert(w);

	switch (cmd._type)
	{
	case Command::Type::Move:
	{
		Coord::Dir d = cmd._dir;

		Coord old_pos = GetPos();
		Coord new_pos = old_pos.Step(d);

		if (w->InRange(new_pos)) {
			if (!w->GetActor(new_pos)
				&& w->GetTerrain(new_pos)->IsWalkable())
			{
				w->RemoveActor(old_pos);
				w->AddActor(new_pos, this);
			}
		}

		return 3.0f;
	}
	}

	assert(false);

	return 0.0f;
}
