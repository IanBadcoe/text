#include "precompiled.h"

#include "InputHandler.h"

#include "sys.hpp"

static int s_create_guard = 0;

InputHandler::InputHandler() :
	Stepable(1.0f),
	_command_dest(nullptr)
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
	TCOD_event_t ev = TCODSystem::checkForEvent(TCOD_EVENT_KEY, &key, nullptr);

	if (key.vk >= TCODK_KP1 && key.vk <= TCODK_KP9 && key.vk != TCODK_KP5 && key.pressed)
	{
		Coord::KeyMapType::iterator it = Coord::KeyMap.find(key.vk);
		assert(it != Coord::KeyMap.end());

		Coord::Dir d = it->second;

		Command cmd;
		cmd._type = Command::Type::Move;
		cmd._dir = d;

		_command_dest->ReceiveCommand(cmd);

		return 15.0;
	}

	if (key.vk == TCODK_ESCAPE)
	{
		Command cmd;
		cmd._type = Command::Type::Exit;

		_command_dest->ReceiveCommand(cmd);
	}

	// Input handler runs are in lock-step with the ends of frames...
	return 1.0f;
}

void InputHandler::SetCommandReceiver(ICommandReceiver* cr)
{
	_command_dest = cr;
}
