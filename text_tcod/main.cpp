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
#include "Server.h"
#include "Client.h"

#define MAX_PLAYERS 4
#define DESIRED_FRAMETIME 1000 / 30

int main(int argc, char* argv[]) {
	TCODConsole::setCustomFont("dejavu16x16_gs_tc.png", TCOD_FONT_LAYOUT_TCOD | TCOD_FONT_TYPE_GREYSCALE);
	TCODConsole::initRoot(80, 50, "text", false);

	TCODColor player_foregrounds[4] = {
		TCODColor(128, 32, 32),
		TCODColor(32, 128, 32),
		TCODColor(32, 32, 128),
		TCODColor(32, 128, 128)
	};

	assert(sizeof(player_foregrounds) / sizeof(TCODColor) >= MAX_PLAYERS);

	Player::SetMaxPlayers(MAX_PLAYERS, player_foregrounds);

	Universe u;

	Map map;

    if (argc == 2 && std::string("-nn") == argv[1])
    {
		// create local player object
		u.EnsurePlayer(0, true);
		map.SetPlayer(u.GetPlayer(0));

		CommandCollator cc;

		cc.SetCommandSequenceReceiver(&u);
        u.SetCommandReceiver(&cc);

        while (!cc.IsEnded())
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

	bool force_no_client = false;

	if (argc == 2 && std::string("-fnc") == argv[1])
	{
		force_no_client = true;
	}

    Networker network(1234, MAX_PLAYERS, force_no_client);

	while (!network.IsReady())
		;

	if (network.IsServer())
	{
		Server server(&network, &u);

		// create local player object
		// use a nullptr peer to reserve an id for the local player
		int id = server.AddPeer(nullptr);

		// we are assuming that the first player is id zero is not the local player
		assert(id == 0);
		u.EnsurePlayer(0, true);
		map.SetPlayer(u.GetPlayer(0));


		// universe sends commands to server
		u.SetCommandReceiver(&server);

		DWORD _end_frame = timeGetTime() + DESIRED_FRAMETIME;

		while (!server.IsEnded())
		{
			// processing received events for Universe::_current_frame
			while (u.Step())
				;

			DWORD now = timeGetTime();
			if (now < _end_frame)
			{
				Sleep(_end_frame - timeGetTime());
			}

			_end_frame = now + DESIRED_FRAMETIME;

			// server services network events, including incoming commands
			network.SendEvents(&server);

			// increment the frame number and send accumulated commands to local universe
			// and peers
			server.EndFrame();

			map.Draw(TCODConsole::root);
			TCODConsole::flush();
			map.NextFrame();
		}
	}
	else
	{
		Client client(&network, &u);

		// universe sends commands to server
		u.SetCommandReceiver(&client);
		// client sends command sequences to our local universe
		client.SetCommandSequenceReceiver(&u);

		bool first_frame = true;

		while (!client.IsEnded())
		{
			while (!client.FrameReceived())
			{
				Sleep(1);

				// client services network events, including incoming command sequences
				network.SendEvents(&client);
			}

			if (first_frame)
			{
				// once we've had the first frame events, the universe is initialised and will have our player object
				map.SetPlayer(u.GetLocalPlayer());
			}

			while (u.Step())
				;

			map.Draw(TCODConsole::root);
			TCODConsole::flush();
			map.NextFrame();

			client.EndFrame();
		}
	}
	
	/*
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