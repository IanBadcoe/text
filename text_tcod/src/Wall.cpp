#include "precompiled.h"

#include "Wall.h"

#include "Map.h"

DisplayChar Wall::s_strong_wall(L'#', TCOD_white);
DisplayChar Wall::s_weak_wall(L'#', TCOD_gray);

static Entity* CreateWall(std::istringstream& in) {
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

void Wall::CalcDisp(uint8_t code)
{
	uint8_t c = GetBlock(code);

	if (_str > 500) {
		SetDisplayChar(DisplayChar(c, TCOD_white));
	}
	else {
		SetDisplayChar(DisplayChar(c, TCOD_gray));
	}
}

bool Wall::DrawCompatWith(const Terrain* other) const {
	if (other->GetType() != EntityType::Wall)
		return false;

	const Wall* wother = dynamic_cast<const Wall*>(other);

	// both strong or both weak
	return (_str > 500) == wother->_str > 500;
}
