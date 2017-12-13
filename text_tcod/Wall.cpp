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
