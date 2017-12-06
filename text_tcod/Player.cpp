#include "Player.h"

#include "Map.h"
#include "World.h"
#include "Coord.h"

#include "sys.hpp"

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

	TCOD_key_t key;
	TCOD_event_t ev = TCODSystem::checkForEvent(TCOD_EVENT_KEY, &key, nullptr);

	if (key.vk >= TCODK_KP1 && key.vk <= TCODK_KP9 && key.vk != TCODK_KP5)
	{
		Coord::KeyMapType::iterator it = Coord::KeyMap.find(key.vk);
		assert(it != Coord::KeyMap.end());

		Coord::Dir d = it->second;

		Coord old_pos = GetPos();
		Coord new_pos = old_pos.Step(d);

		if (w->InRange(new_pos)) {
			w->RemoveActor(old_pos);
			w->AddActor(new_pos, this);
		}
	}

	return 1.0f;
}

DisplayChar& Player::Disp() const
{
	return s_player[_num];
}
