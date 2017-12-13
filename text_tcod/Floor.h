#pragma once

#include "Entity.h"

#include "DisplayChar.h"

#include "color.hpp"

class Floor : public Terrain {
public:
    Floor(std::istringstream& in) : Terrain(in) {}
	Floor() : Terrain(EntityType::Floor, true, true) {}

	virtual DisplayChar Disp() const {
		return s_floor;
	}

	// Inherited via Terrain
	virtual void SerialiseTo(std::ostringstream& out) const override;

private:
	static DisplayChar s_floor;
	static TCODColor s_floor_foreground;
	static TCODColor s_floor_background;

    static EntityCreator s_creator;
};
