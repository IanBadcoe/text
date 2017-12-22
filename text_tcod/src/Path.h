#pragma once

#include "Coord.h"

class Path
{
public:
	Path(std::istringstream& in);
	Path() : _step(0) {}
	void SerialiseTo(std::ostringstream& out) const;

	void Clear() {
		_step = 0;
		_path.clear();
	}

	std::vector<Coord> _path;
	int _step;
};
