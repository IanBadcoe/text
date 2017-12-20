#include "precompiled.h"

#include "Path.h"

Path::Path(std::istringstream & in) {
	in >>= _step;

	int num_steps = 0;
	in >>= num_steps;

	for (int i = 0; i < num_steps; i++) {
		Coord c;
		in >>= c;

		_path.push_back(c);
	}
}

void Path::SerialiseTo(std::ostringstream & out) const {
	out <<= _step;
	out <<= (int)_path.size();
	for (int i = 0; i < _path.size(); i++) {
		out <<= _path[i];
	}
}
