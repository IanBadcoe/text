#pragma once

#include "Terrain.h"

#include "DisplayChar.h"

#include "color.hpp"

class Floor : public Terrain {
public:
	Floor(std::istringstream& in) : Terrain(in) { }
	Floor() : Terrain(EntityType::Floor, true, true) { }

	// Inherited via Terrain
	virtual void SerialiseTo(std::ostringstream& out) const override;

private:
	static TCODColor s_foreground;

    static EntityCreator s_creator;

	// Inherited via Terrain
	virtual void CalcDisp(const Terrain* surrounds[8]) override;

	// Inherited via Terrain
	virtual bool DrawCompatWith(const Terrain* other) const override;
};
