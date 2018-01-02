#include "precompiled.h"

#include "Wall.h"

DisplayChar Wall::s_strong_wall(L'#', TCOD_white);
DisplayChar Wall::s_weak_wall(L'#', TCOD_gray);

static Entity* CreateWall(std::istringstream& in, World* w) {
    return new Wall(in);
}

EntityCreator Wall::s_creator(EntityType::Wall, CreateWall);

Wall::Wall(std::istringstream& in) : Terrain(in)
{
    in >>= _str;
}

void Wall::SerialiseTo(std::ostringstream& out) const
{
	Terrain::SerialiseTo(out);

	out <<= _str;
}

void Wall::CalcDisp(const Terrain* surrounds[8])
{
	uint8_t code = 0;
	TCODColor brightest_bg(0, 0, 0);

	for (int k = 0; k < 8; k++) {
		if (surrounds[k])
		{
			if (DrawCompatWith(surrounds[k])) {
				code |= 1 << k;
			}
	
			// we take our background from the brightest adjoining floor or empty space
			if (surrounds[k]->GetType() == EntityType::Floor || surrounds[k]->GetType() == EntityType::ShadedVoid)
			{
				brightest_bg = std::max(brightest_bg, surrounds[k]->Disp()._fcol);
			}
		}
	}

	uint8_t c = GetWallCharacter(code);

	if (_str > 500) {
		SetDisplayChar(DisplayChar(c, TCOD_white, brightest_bg));
	}
	else {
		SetDisplayChar(DisplayChar(c, TCOD_gray, brightest_bg));
	}
}

bool Wall::DrawCompatWith(const Terrain* other) const {
	if (other->GetType() != EntityType::Wall)
		return false;

	const Wall* wother = dynamic_cast<const Wall*>(other);

	// both strong or both weak
	return (_str > 500) == wother->_str > 500;
}
