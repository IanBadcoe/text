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

	for (int i = 0; i < 200; i++) {
		if (world.IsWalkable(Coord(100, i)))
		{
			world.AddActor(Coord(100, i), new Player(0));
			break;
		}
	}

	Map map(200, 200, world.GetPlayer(0));

	while (!TCODConsole::isWindowClosed()) {
		TCOD_key_t key;
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
		TCODConsole::root->clear();
//		TCODConsole::root->putChar(40, 25, '@');
		Player* p = world.GetPlayer(0);
		Coord pp = p->GetPos();
		Coord map_corner = pp - Coord(40, 25);
		map_corner = world.ClampCoord(map_corner);
		map.ReadWorld(world, pp);
		map.Draw(TCODConsole::root, map_corner, Coord(80, 50));
		TCODConsole::flush();
		map.NextFrame();
	}

	return 0;
}