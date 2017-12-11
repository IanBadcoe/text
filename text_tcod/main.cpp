#include "precompiled.h"

#include "libtcod.hpp"

#include "Map.h"
#include "World.h"
#include "Template.h"
#include "Player.h"
#include "Networker.h"
#include "Universe.h"
#include "Command.h"
#include "CommandCollator.h"

//#include "enet/enet.h"

int main(int argc, char* argv[]) {
	TCODConsole::setCustomFont("dejavu16x16_gs_tc.png", TCOD_FONT_LAYOUT_TCOD | TCOD_FONT_TYPE_GREYSCALE);
	TCODConsole::initRoot(80, 50, "text", false);

	TCODColor player_foregrounds[4] = {
		TCODColor(128, 32, 32),
		TCODColor(32, 128, 32),
		TCODColor(32, 32, 128),
		TCODColor(32, 128, 128)
	};

	Player::SetMaxPlayers(4, player_foregrounds);

    if (argc == 2 && std::string("-nn") == argv[1])
    {
		Universe u;

		CommandCollator cc;

		Map map(u.GetPlayer(0));

		cc.SetCommandSequenceReceiver(&u);
        u.SetCommandReceiver(&cc);

        while (!u.IsEnded())
        {
            while (u.Step())
                ;

            cc.EndFrame();

            map.Draw(TCODConsole::root);
            TCODConsole::flush();
            map.NextFrame();
        }

        return 0;
    }

    Networker network(5432);

/*    if (network.IsServer())
    {
        s_server = new ServerTest(network);
    }
    else
    {
        s_client = new ClientTest(network);
    }

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

	sq.AddFutureStep(p0, 1.5f);

	while (!TCODConsole::isWindowClosed() && sq.AnythingToStep()) {
		while (sq.AnythingToStep() && sq.Step())
			;
	} */

	return 0;
}