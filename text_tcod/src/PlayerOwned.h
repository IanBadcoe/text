#pragma once

#include "Actor.h"

class PlayerOwned : public Actor
{
public:
	PlayerOwned(std::istringstream& in);
	PlayerOwned(EntityType t, World* w, const Coord& pos) : Actor(t, w, pos) {}
};
