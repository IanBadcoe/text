#include "precompiled.h"

#include "Floor.h"

#include "Map.h"

TCODColor Floor::s_floor_foreground(64, 64, 64);
TCODColor Floor::s_floor_background(32, 32, 32);
DisplayChar Floor::s_floor(L'.', Floor::s_floor_foreground, Floor::s_floor_background);

static Entity* CreateFloor(std::istringstream& in) {
    return new Floor(in);
}

EntityCreator Floor::s_creator(EntityType::Floor, CreateFloor);

void Floor::SerialiseTo(std::ostringstream& out) const
{
	Terrain::SerialiseTo(out);
}

void Floor::CalcDisp(uint8_t code)
{
}

bool Floor::DrawCompatWith(const Terrain* other) const {
	return other->GetType() == EntityType::Floor;
}
