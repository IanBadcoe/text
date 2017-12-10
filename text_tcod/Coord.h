#pragma once

#include <map>

#include "console_types.h"

class Coord {
public:
	Coord() : _x(0), _y(0) {}
	Coord(int x, int y) : _x(x), _y(y) {}

	enum class Dir {
		North,
		NorthEast,
		East,
		SouthEast,
		South,
		SouthWest,
		West,
		NorthWest
	};

	static int DX[8];
	static int DY[8];

	static Coord DC[8];

	typedef std::map<TCOD_keycode_t, Dir> KeyMapType;
	static KeyMapType KeyMap;

	Coord Step(Dir d) {
		return *this + DC[(int)d];
	}

	Coord operator+(const Coord& c) const {
		return Coord(_x + c._x, _y + c._y);
	}

	Coord operator-(const Coord& c) const {
		return Coord(_x - c._x, _y - c._y);
	}

    Coord operator/(int v) const {
        return Coord(_x / v, _y / 2);
    }

	int _x;
	int _y;
};