#include "precompiled.h"

#include "Floor.h"

TCODColor Floor::s_foreground(32, 32, 32);

static Entity* CreateFloor(std::istringstream& in, const CreatorArg& ca) {
    return new Floor(in);
}

EntityCreator Floor::s_creator(EntityType::Floor, CreateFloor);

void Floor::SerialiseTo(std::ostringstream& out) const
{
	Terrain::SerialiseTo(out);
}

void Floor::CalcDisp(const Terrain* surrounds[8])
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
				if (surrounds[k]->GetType() == EntityType::ShadedVoid) {
					brightest_bg = std::max(brightest_bg, surrounds[k]->Disp()._fcol);
				}
			}
		}
	}

	uint8_t c = GetBlockCharacter(code);

	// special case for floor, use tiles for whole squares
	if (c == 0xfe)
	{
		c = 0x80;
		brightest_bg = TCODColor(48, 48, 48);
	}

	SetDisplayChar(DisplayChar(c, s_foreground, brightest_bg));
}

bool Floor::DrawCompatWith(const Terrain* other) const {
	return other->GetType() != EntityType::ShadedVoid;
}
