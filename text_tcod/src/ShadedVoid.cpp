#include "precompiled.h"

#include "ShadedVoid.h"

#include "Map.h"

TCODColor ShadedVoid::s_foreground[4] = {
	TCODColor(16, 16, 255),
	TCODColor(12, 12, 192),
	TCODColor(8, 8, 128),
	TCODColor(4, 4, 64)
};

TCODColor ShadedVoid::s_background[4] = {
	TCODColor(12, 12, 192),
	TCODColor(8, 8, 128),
	TCODColor(4, 4, 64),
	TCODColor(0, 0, 0)
};

static Entity* CreateShadedVoid(std::istringstream& in) {
    return new ShadedVoid(in);
}

EntityCreator ShadedVoid::s_creator(EntityType::ShadedVoid, CreateShadedVoid);

ShadedVoid::ShadedVoid(std::istringstream & in) : Terrain(in) {
	in >>= _depth;
}

void ShadedVoid::SerialiseTo(std::ostringstream& out) const
{
	Terrain::SerialiseTo(out);

	out <<= _depth;
}

void ShadedVoid::CalcDisp(uint8_t code)
{
}

bool ShadedVoid::DrawCompatWith(const Terrain* other) const {
	if (other->GetType() != EntityType::ShadedVoid)
		return false;

	const ShadedVoid* shother = dynamic_cast<const ShadedVoid*>(other);

	return _depth == shother->_depth;
}
