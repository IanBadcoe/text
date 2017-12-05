#include "Map.h"
#include "Entity.h"
#include "World.h"

DisplayChar Map::s_void(L' ', TCOD_white);

void Map::SetChar(Coord pos, const Entity* entity, const Terrain* terrain) {
	assert(terrain);

	_map[idx(pos)]._frame = _frame;

	if (entity)
	{
		DisplayChar ent_dc = entity->Disp();

		_map[idx(pos)]._dc = ent_dc;

		if (!ent_dc._use_bcol)
		{
			_map[idx(pos)]._dc._bcol = terrain->Disp()._bcol;
		}
	}
	else
	{
		_map[idx(pos)]._dc = terrain->Disp();
	}

	_tcod_map.setProperties(pos._x, pos._y, terrain->IsTransparent(), terrain->IsWalkable());
}

void Map::Draw(TCODConsole* console, Coord bottom_left, Coord screen_size) {
	for (int i = 0; i < screen_size._x; i++)
	{
		for (int j = 0; j < screen_size._y; j++)
		{
			Coord world_pos = Coord(i, j) + bottom_left;
			const MapChar& mc = _map[idx(world_pos)];
			float fade = (100 - (_frame - mc._frame)) / 100.0f;

			if (fade < 0.0)
			{
				console->putCharEx(i, j, ' ', TCOD_white, TCOD_black);
			}
			else
			{
				const DisplayChar& dc = _map[idx(world_pos)]._dc;
				console->putCharEx(i, screen_size._y - j - 1,
					dc._char, dc._fcol * fade, dc._bcol * fade);
			}
		}
	}
}

void Map::ReadWorld(const World& world, Coord eye_pos) {
	_tcod_map.computeFov(eye_pos._x, eye_pos._y, 1000, true, FOV_PERMISSIVE_6);
	assert(_width == world.GetWidth());
	assert(_height == world.GetHeight());

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			if (_tcod_map.isInFov(i, j))
			{
				Coord pos(i, j);
				const Terrain* t = world.GetTerrain(pos);
				SetChar(pos, world.GetActor(pos), t);
			}
		}
	}
}
