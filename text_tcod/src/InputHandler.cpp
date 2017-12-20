#include "precompiled.h"

#include "InputHandler.h"

#include "sys.hpp"

static int s_create_guard = 0;

InputHandler::InputHandler() :
	Stepable(1.0f),
	_command_dest(nullptr),
	_console(nullptr)
{
	assert(!s_create_guard);
	s_create_guard++;
}

InputHandler::~InputHandler()
{
	assert(s_create_guard == 1);
	s_create_guard--;
}

float InputHandler::InnerStep()
{
	DWORD now = timeGetTime();

	TCOD_key_t key;
	TCOD_mouse_t mouse;

	TCOD_event_t ev = TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS | TCOD_EVENT_MOUSE_PRESS, &key, &mouse);

	if (ev == TCOD_EVENT_KEY_PRESS)
	{
		if (key.vk >= TCODK_KP1 && key.vk <= TCODK_KP9 && key.vk != TCODK_KP5) {
			Coord::KeyMapType::iterator it = Coord::KeyMap.find(key.vk);
			assert(it != Coord::KeyMap.end());

			Coord::Dir d = it->second;

			Command cmd;
			cmd._type = Command::Type::Move;
			cmd._dir = d;

			_command_dest->ReceiveCommand(cmd);
		}

		if (key.vk == TCODK_ESCAPE) {
			Command cmd;
			cmd._type = Command::Type::Exit;

			_command_dest->ReceiveCommand(cmd);
		}
	} else if (ev == TCOD_EVENT_MOUSE_PRESS) {
		Coord console_pos = FixConsoleCell(mouse.cx, mouse.cy);

		Command cmd;
		cmd._type = Command::Type::ConsoleCellClick;
		cmd._map_cell = console_pos;

		_command_dest->ReceiveCommand(cmd);
	}

	return 0.5f;
}

void InputHandler::SetCommandReceiver(ICommandReceiver* cr)
{
	_command_dest = cr;
}

Coord InputHandler::FixConsoleCell(int x, int y) {
	return Coord(x, _console->getHeight() - y - 1);
}
