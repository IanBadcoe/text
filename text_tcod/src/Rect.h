#pragma once

#include "Coord.h"

// includes bl, does not include tr:
//
//                          o <- tr
//       xxxxxxxxxxxxxxxxxxx
//       xxxxxxxxxxxxxxxxxxx
//       xxxxxxxxxxxxxxxxxxx
// bl -> oxxxxxxxxxxxxxxxxxx
class Rect
{
public:
	Rect() {}
	Rect(int left, int bottom, int right, int top) : _bl(left, bottom), _tr(right, top) {}
	Rect(Coord bl, Coord tr) : _bl(bl), _tr(tr) {}

	bool Contains(Coord c) const {
		return c._x >= _bl._x && c._x < _tr._x
			&& c._y >= _bl._y && c._y < _tr._y;
	}

	bool Contains(Rect r) const {
		return Contains(r._bl) && Contains(r._tr - Coord(1, 1));
	}

	Coord Size() const {
		return Coord(_tr._x - _bl._x, _tr._y - _bl._y);
	}

	Coord _bl;
	Coord _tr;
};
