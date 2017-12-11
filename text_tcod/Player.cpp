#include "precompiled.h"

#include "Player.h"

#include "Map.h"
#include "World.h"
#include "Coord.h"

TCODColor Player::s_foreground[4] = {
	TCODColor(128, 32, 32),
	TCODColor(32, 128, 32),
	TCODColor(32, 32, 128),
	TCODColor(32, 128, 128)
};

DisplayChar Player::s_player[4] = {
	DisplayChar('@', s_foreground[0]),
	DisplayChar('@', s_foreground[1]),
	DisplayChar('@', s_foreground[2]),
	DisplayChar('@', s_foreground[3])
};

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


DisplayChar& Player::Disp() const
{
	return s_player[_num];
}

float Player::ExecuteCommand(const Command & cmd)
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
