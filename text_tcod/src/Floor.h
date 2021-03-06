#pragma once

#include "Terrain.h"

class Floor : public Terrain {
public:
	Floor(std::istringstream& in) : Terrain(in) {}
	Floor() : Terrain(EntityType::Floor, SerialiseOrder::TerrainDontCare, true, true) {
		SetDisplayChar(DisplayChar('x', s_foreground));
	}

	// Inherited via Terrain
	virtual void SerialiseTo(std::ostringstream& out) const override;

	static TCODColor s_foreground;

private:

    static EntityCreator s_creator;

	// Inherited via Terrain
	virtual void CalcDisp(const Terrain* surrounds[8]) override;

	// Inherited via Terrain
	virtual bool DrawCompatWith(const Terrain* other) const override;
};
