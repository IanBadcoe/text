#include "precompiled.h"

#include "PlayerOwned.h"

#include "Player.h"
#include "World.h"
#include "Universe.h"

PlayerOwned::PlayerOwned(std::istringstream& in, World* w) : Actor(in) {
	int player_id;
	in >>= player_id;

	_player = w->GetUniverse()->GetPlayer(player_id);
}

void PlayerOwned::SerialiseTo(std::ostringstream& out) const {
	Actor::SerialiseTo(out);

	out <<= _player->GetId();
}

