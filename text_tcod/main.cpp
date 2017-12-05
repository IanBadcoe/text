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
		if (world.IsWalkable(100, i))
		{
			world.SetEntity(100, i, new Player(0));
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
		int px = p->GetX();
		int py = p->GetY();
		int sx = std::min(std::max(px - 40, 0), 199);
		int sy = std::min(std::max(py - 25, 0), 199);
		map.ReadWorld(world, 0, 0, 200, 200, px, py);
		map.Draw(TCODConsole::root, sx, sy, 80, 50);
		TCODConsole::flush();
		map.NextFrame();
	}

	return 0;
}