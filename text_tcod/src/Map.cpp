#include "precompiled.h"

#include "Map.h"

#include "Entity.h"
#include "Actor.h"
#include "World.h"
#include "Player.h"

DisplayChar Map::s_void(L' ', TCOD_white);

void Map::SetChar(Coord pos, const Entity* entity, const Terrain* terrain) {
	assert(terrain);

    SetWorld(_p->GetWorld());

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

	_tcod_map->setProperties(pos._x, pos._y, terrain->IsTransparent(), terrain->IsWalkable());
}

void Map::Draw(TCODConsole* console) {
	if (!_p)
		return;

    SetWorld(_p->GetWorld());

    Coord screen_size(console->getWidth(), console->getHeight());

    Coord pp = _p->GetPos();

    ReadWorld(pp);

    Coord map_corner = pp - screen_size / 2;
    map_corner = _world->ClampCoord(map_corner);
    
    for (int i = 0; i < screen_size._x; i++)
	{
		for (int j = 0; j < screen_size._y; j++)
		{
			Coord world_pos = Coord(i, j) + map_corner;
			const MapChar& mc = _map[idx(world_pos)];
			float fade = (100 - (_frame - mc._frame)) / 100.0f;

			if (fade < 0.0)
			{
				console->putCharEx(i, j, ' ', TCOD_white, TCOD_black);
			}
			else
			{
				DisplayChar dc = _map[idx(world_pos)]._dc;
				console->putCharEx(i, screen_size._y - j - 1,
					dc._char, dc._fcol * fade, dc._bcol * fade);
			}
		}
	}
}

void Map::ReadWorld(Coord eye_pos) {
	_tcod_map->computeFov(eye_pos._x, eye_pos._y, 1000, true, FOV_PERMISSIVE_6);
	assert(_width == _world->GetWidth());
	assert(_height == _world->GetHeight());

	for (int i = 0; i < _width; i++)
	{
		for (int j = 0; j < _height; j++)
		{
			if (_tcod_map->isInFov(i, j))
			{
				Coord pos(i, j);
				const Terrain* t = _world->GetTerrain(pos);
				SetChar(pos, _world->GetActor(pos), t);
			}
		}
	}
}
