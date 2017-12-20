#pragma once

#include "Coord.h"
#include "DisplayChar.h"
#include "Rect.h"

#include "color.hpp"
#include "console.hpp"

class Player;

class HUD {
public:
	HUD(Rect pos_in_console) :
		_pos_in_console(pos_in_console) {
	}

	~HUD() {
	}

	void SetPlayer(const Player* p) { _p = p; }

	void Draw(TCODConsole* console);

private:
	const Player* _p;

	Rect _pos_in_console;
};