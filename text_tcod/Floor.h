#pragma once

#include "Entity.h"

#include "color.hpp"

class Floor : public Terrain {
public:
	Floor() : Terrain(EntityType::Floor, true, true) {}

	virtual DisplayChar& Disp() const {
		return s_floor;
	}
private:
	static DisplayChar s_floor;
	static TCODColor s_floor_foreground;
	static TCODColor s_floor_background;
};
