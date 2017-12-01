// text.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "World.h"
#include "Wall.h"
#include "Floor.h"
#include "Map.h"

World world(1000, 1000);
Map map(1000, 1000);

static bool is_border(int x, int y) {
	return x == 0 || y == 0 || x == world.GetWidth() - 1 || y == world.GetHeight() - 1;
}

static int orth_x[4] = { 1, 0, -1, 0 };
static int orth_y[4] = { 0, -1, 0, 1 };

static Entity* clone_entity(Entity* e) {
	if (!e) return nullptr;

	if (e->GetType() == EntityType::Wall) {
		return new Wall((rand() % 1000 + static_cast<Wall*>(e)->GetStr()) / 2);
	}
	else if (e->GetType() == EntityType::Floor)
	{	
		return new Floor();
	}

	assert(false);

	return nullptr;
}

int main()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	// fullscreen
	::SendMessage(::GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
	// set screen buffer same size as screen
	CONSOLE_SCREEN_BUFFER_INFO inf;
	GetConsoleScreenBufferInfo(console, &inf);
	SetConsoleScreenBufferSize(console, inf.dwMaximumWindowSize);

	srand(17);

	for(int k = 0; k < world.GetWidth(); k++)
	{
		for (int j = 0; j < world.GetWidth(); j++)
		{
			int r = rand();
			
			if (r < RAND_MAX / 3 || is_border(k, j)) {
				world.SetTerrain(k, j, new Wall(rand() % 1000));
			}
			else if (r < RAND_MAX / 3 * 2)
			{
				world.SetTerrain(k, j, new Floor());
			}
		}
	}

	for (int k = 0; k < 50; k++) {
		for (int i = 0; i < world.GetWidth(); i++)
		{
			for (int j = 0; j < world.GetWidth(); j++)
			{
				int d = rand() % 4;

				Entity* e = clone_entity(world.GetTerrain(i, j));

				world.SetTerrain(i, j, e);
			}
		}
	}

	for (int i = 0; i < inf.dwMaximumWindowSize.X; i++)
	{
		for (int j = 0; j < inf.dwMaximumWindowSize.Y; j++)
		{
			map.SetChar(i, j, world[i][j]);
		}
	}

	map.Draw(console, 0, 0, inf.dwMaximumWindowSize.X, inf.dwMaximumWindowSize.Y);

    return 0;
}

