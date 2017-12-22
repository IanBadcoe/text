#include "precompiled.h"

#include "Rock.h"

TCODColor Rock::s_foregrounds[(int)Rock::Type::NumRockTypes] = {
	TCODColor(192, 192, 192),				// Hard,
	TCODColor(128, 128, 128),				// Soft,
	TCODColor(66, 40, 7),					// Earth,
	TCODColor(160, 47, 23),					// RedOre,
	TCODColor(4, 63, 2),					// GreenOre,
	TCODColor(19, 59, 140)					// BlueOre,
};


static Entity* CreateRock(std::istringstream& in, const CreatorArg& ca) {
    return new Rock(in);
}

EntityCreator Rock::s_creator(EntityType::Rock, CreateRock);

Rock::Rock(std::istringstream& in) : Terrain(in)
{
    in >>= _t;
}

void Rock::SerialiseTo(std::ostringstream& out) const
{
	Terrain::SerialiseTo(out);

	out <<= _t;
}

void Rock::CalcDisp(const Terrain* surrounds[8])
{
	uint8_t code = 0;
	TCODColor brightest_bg(0, 0, 0);

	// for blocks we look only at orthogonals...
	for (int k = 0; k < 8; k += 2) {
		if (surrounds[k]) {
			if (DrawCompatWith(surrounds[k])) {
				code |= 1 << (k / 2);
			} else {
				// we take our background from the brightest adjoining empty space's foreground
				if (surrounds[k]->GetType() == EntityType::ShadedVoid || surrounds[k]->GetType() == EntityType::Floor) {
					brightest_bg = std::max(brightest_bg, surrounds[k]->Disp()._fcol);
				}
			}
		}
	}

	uint8_t c = GetBlockCharacter(code);

	SetDisplayChar(DisplayChar(c, s_foregrounds[(int)_t], brightest_bg));
}

bool Rock::DrawCompatWith(const Terrain* other) const {
	return other->GetType() == EntityType::Rock;
}
