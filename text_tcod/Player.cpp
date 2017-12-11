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
	World* w = GetWorld();
	assert(w);

	return 0;
}


/*Coord old_pos = GetPos();
Coord new_pos = old_pos.Step(d);

if (w->InRange(new_pos)) {
	if (!w->GetActor(new_pos)
		&& w->GetTerrain(new_pos)->IsWalkable())
	{
		w->RemoveActor(old_pos);
		w->AddActor(new_pos, this);
	}
} */

DisplayChar& Player::Disp() const
{
	return s_player[_num];
}
