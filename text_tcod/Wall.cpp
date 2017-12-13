#include "precompiled.h"

#include "Wall.h"

#include "Map.h"

DisplayChar Wall::s_strong_wall(L'#', TCOD_white);
DisplayChar Wall::s_weak_wall(L'#', TCOD_gray);

void Wall::SerialiseTo(std::ostringstream& out) const
{
	Terrain::SerialiseTo(out);

	out <<= _str;
}
