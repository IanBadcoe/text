#include "precompiled.h"

#include "Universe.h"

#include "World.h"
#include "Template.h"
#include "player.h"

Universe::Universe() : _is_ended(false)
{
    _world = new World(200, 200);

    CircleTemplate ct(100, 100, 75, 3);

    ct.Apply(_world);

    for (int i = 0; i < 200; i++) {
        if (_world->IsWalkable(Coord(100, i)))
        {
            _world->AddActor(Coord(100, i), new Player(0));
            break;
        }
    }

	_input = new InputHandler();
	_input->SetCommandReceiver(this);

	_queue.AddFutureStep(_input, 0);
}


Universe::~Universe()
{
}

void Universe::ReceiveCommandSequence(const CommandSequence & cs)
{
	for (int i = 0; i < cs._commands.size(); i++)
	{

	}
}

void Universe::SetCommandReceiver(ICommandReceiver * csr)
{
    _pass_commands_to = csr;
}

void Universe::ReceiveCommand(const Command & c)
{
    _pass_commands_to->ReceiveCommand(c);
}
