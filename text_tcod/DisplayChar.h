#pragma once

#include "color.hpp"

class DisplayChar {
public:
	DisplayChar(wchar_t c, TCODColor fcol, TCODColor bcol) : _char(c), _fcol(fcol), _bcol(bcol), _use_bcol(true) {}
	DisplayChar(wchar_t c, TCODColor fcol) : _char(c), _fcol(fcol), _use_bcol(false) {}

	wchar_t _char;
	TCODColor _fcol;
	TCODColor _bcol;
	bool _use_bcol;
};



