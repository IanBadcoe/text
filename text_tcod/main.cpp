#include "libtcod.hpp"

#include "Map.h"
#include "World.h"
#include "Template.h"
#include "Player.h"

#include <algorithm>

int main() {
	TCODConsole::initRoot(80, 50, "text", false);

	World world(200, 200);
	CircleTemplate ct(100, 100, 75, 3);

	ct.Apply(world);

	Player* p0;

	for (int i = 0; i < 200; i++) {
		if (world.IsWalkable(Coord(100, i)))
		{
			world.AddActor(Coord(100, i), new Player(0));
			break;
		}
	}

	p0 = world.GetPlayer(0);

	Map map(200, 200, world.GetPlayer(0));

	StepableQueue sq;
	sq.AddFutureStep(p0, 1.5f);

	while (!TCODConsole::isWindowClosed() && sq.AnythingToStep()) {
		while (sq.AnythingToStep() && sq.Step())
			;

		Coord pp = p0->GetPos();
		Coord map_corner = pp - Coord(40, 25);
		map_corner = world.ClampCoord(map_corner);
		map.ReadWorld(world, pp);
		map.Draw(TCODConsole::root, map_corner, Coord(80, 50));
		TCODConsole::flush();
		map.NextFrame();
	}

	return 0;
}