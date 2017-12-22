#include "precompiled.h"

#include "Coord.h"

// Directions are clockwise from North
int Coord::DX[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int Coord::DY[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

Coord Coord::DC[8] = {
	Coord(DX[0], DY[0]),
	Coord(DX[1], DY[1]),
	Coord(DX[2], DY[2]),
	Coord(DX[3], DY[3]),
	Coord(DX[4], DY[4]),
	Coord(DX[5], DY[5]),
	Coord(DX[6], DY[6]),
	Coord(DX[7], DY[7])
};

static std::map<TCOD_keycode_t, Coord::Dir> s_init_dir_map() {
	std::map<TCOD_keycode_t, Coord::Dir> ret;
	ret[TCODK_KP1] = Coord::Dir::SouthWest;
	ret[TCODK_KP2] = Coord::Dir::South;
	ret[TCODK_KP3] = Coord::Dir::SouthEast;
	ret[TCODK_KP6] = Coord::Dir::East;
	ret[TCODK_KP9] = Coord::Dir::NorthEast;
	ret[TCODK_KP8] = Coord::Dir::North;
	ret[TCODK_KP7] = Coord::Dir::NorthWest;
	ret[TCODK_KP4] = Coord::Dir::West;

	return ret;
}

std::map<TCOD_keycode_t, Coord::Dir> Coord::KeyMap = s_init_dir_map();
