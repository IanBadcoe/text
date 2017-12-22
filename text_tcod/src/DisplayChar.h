#pragma once

class DisplayChar {
public:
	DisplayChar() : _char('x'), _fcol(TCODColor(0, 0, 0)), _bcol(TCODColor(0, 0, 0)), _use_bcol(true) {}
	DisplayChar(uint8_t c, TCODColor fcol, TCODColor bcol) : _char(c), _fcol(fcol), _bcol(bcol), _use_bcol(true) {}
	DisplayChar(uint8_t c, TCODColor fcol) : _char(c), _fcol(fcol), _use_bcol(false) {}

	uint8_t _char;
	TCODColor _fcol;
	TCODColor _bcol;
	bool _use_bcol;
};



