#pragma once

#include "Actor.h"

class Player;

class PlayerOwned : public Actor
{
public:
	PlayerOwned(std::istringstream& in, World* w);
	PlayerOwned(EntityType t, World* w, const Coord& pos, Player* p) :
		Actor(t, SerialiseOrder::ActorAfterPlayer, w, pos),
		_player(p) {
	}
	PlayerOwned(EntityType t, SerialiseOrder so, World* w, const Coord& pos, Player* p) :
		Actor(t, so, w, pos),
		_player(p) {
		assert(so >= SerialiseOrder::ActorAfterPlayer);
	}

	// Inherited via Actor
	virtual void SerialiseTo(std::ostringstream& out) const override;

protected:
	Player* _player;
};
