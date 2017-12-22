#include "precompiled.h"

#include "HUD.h"

void HUD::Draw(TCODConsole* console) {
	if (!_p)
		return;

	console->putCharEx(0, 0, 0xd6, TCOD_white, TCOD_black);
	console->putCharEx(0, console->getHeight() - 1, 0xd3, TCOD_white, TCOD_black);
	console->putCharEx(_pos_in_console._bl._x, 0, 0xd2, TCOD_white, TCOD_black);
	console->putCharEx(_pos_in_console._bl._x, console->getHeight() - 1, 0xd0, TCOD_white, TCOD_black);
	console->putCharEx(console->getWidth() - 1, 0, 0xb7, TCOD_white, TCOD_black);
	console->putCharEx(console->getWidth() - 1, console->getHeight() - 1, 0xbd, TCOD_white, TCOD_black);

	for (int i = 1; i < _pos_in_console._bl._x; i++) {
		console->putCharEx(i, 0, 0xc4, TCOD_white, TCOD_black);
		console->putCharEx(i, console->getHeight() - 1, 0xc4, TCOD_white, TCOD_black);
	}

	for (int i = _pos_in_console._bl._x + 1; i < console->getWidth() - 1; i++) {
		console->putCharEx(i, 0, 0xc4, TCOD_white, TCOD_black);
		console->putCharEx(i, console->getHeight() - 1, 0xc4, TCOD_white, TCOD_black);
	}

	for (int i = 1; i < console->getHeight() - 1; i++) {
		console->putCharEx(0, i, 0xba, TCOD_white, TCOD_black);
		console->putCharEx(console->getWidth() - 1, i, 0xba, TCOD_white, TCOD_black);
		console->putCharEx(_pos_in_console._bl._x, i, 0xba, TCOD_white, TCOD_black);
	}
}
