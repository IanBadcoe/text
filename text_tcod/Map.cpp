#include "Map.h"
#include "Entity.h"
#include "World.h"

DisplayChar Map::s_void(L' ', TCOD_white);

void Map::SetChar(int x, int y, const Entity* entity, const Entity* terrain) {
	assert(terrain);

	_map[idx(x, y)]._frame = _frame;

	if (entity)
	{
		DisplayChar ent_dc = entity->Disp();

		_map[idx(x, y)]._dc = ent_dc;

		if (!ent_dc._use_bcol)
		{
			_map[idx(x, y)]._dc._bcol = terrain->Disp()._bcol;
		}
	}
	else
	{
		_map[idx(x, y)]._dc = s_void;
	}
}

void Map::Draw(TCODConsole* console, int x, int y, int w, int h) {
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			const DisplayChar& dc = _map[idx(i + x, j + y)]._dc;
			console->putCharEx(i, j, dc._char, dc._fcol, dc._bcol);
		}
	}
}

void Map::ReadWorld(const World& world, int x, int y, int w, int h) {
	for (int i = x; i < x + w; i++)
	{
		for (int j = y; j < y + h; j++)
		{
			SetChar(i, j, world.GetEntity(i, j), world.GetTerrain(i, j));
		}
	}
}
