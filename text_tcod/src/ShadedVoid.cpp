#include "precompiled.h"

#include "ShadedVoid.h"

TCODColor ShadedVoid::s_foreground[4] = {
	TCODColor(16, 16, 128),
	TCODColor(12, 12, 96),
	TCODColor(8, 8, 64),
	TCODColor(4, 4, 32)
};

TCODColor ShadedVoid::s_background[4] = {
	TCODColor(12, 12, 96),
	TCODColor(8, 8, 64),
	TCODColor(4, 4, 32),
	TCODColor(0, 0, 0)
};

static Entity* CreateShadedVoid(std::istringstream& in, const CreatorArg& ca) {
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

void ShadedVoid::CalcDisp(const Terrain* surrounds[8])
{
	uint8_t code = 0;

	// for blocks we look only at orthogonals...
	for (int k = 0; k < 8; k += 2) {
		if (surrounds[k]) {
			if (DrawCompatWith(surrounds[k])) {
				code |= 1 << (k / 2);
			}
		}
	}

	uint8_t c = GetBlockCharacter(code);

	SetDisplayChar(DisplayChar(c, s_foreground[_depth], s_background[_depth]));
}

bool ShadedVoid::DrawCompatWith(const Terrain* other) const {
	if (other->GetType() != EntityType::ShadedVoid)
		return true;

	const ShadedVoid* shother = dynamic_cast<const ShadedVoid*>(other);

	// we are "compatible" with shallower void, so that we fully fill in squares adjoining shallower
	// ones
	return _depth >= shother->_depth;
}
