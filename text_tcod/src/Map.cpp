#include "precompiled.h"

#include "Map.h"

#include "Terrain.h"
#include "Actor.h"
#include "World.h"
#include "Player.h"

DisplayChar Map::s_void(L' ', TCOD_white);

void Map::SetWorld(const World * world) {
	if (world == _world)
		return;

	_world = world;

	ClearWorld();

	_width = _world->GetWidth();
	_height = _world->GetHeight();

	_terrain = new DisplayChar[_width * _height];
	_actors = new DisplayChar[_width * _height];
	_has_actor = new bool[_width * _height];
	_tcod_map = new TCODMap(_width, _height);
	_frames = new int64_t[_width * _height];

	memset(_frames, 0, _width * _height * sizeof(uint64_t));

	for (int i = 0; i < _width; i++) {
		for (int j = 0; j < _height; j++) {
			_tcod_map->setProperties(i, j, false, false);
		}
	}
}

void Map::ClearWorld() {
	delete[] _terrain;
	delete[] _actors;
	delete[] _has_actor;
	delete _tcod_map;
	delete[] _frames;

	_terrain = nullptr;
	_actors = nullptr;
	_has_actor = nullptr;
	_tcod_map = nullptr;
	_frames = nullptr;

	_width = 0;
	_height = 0;
}

void Map::SetChars(Coord pos, const Actor* actor, const Terrain* terrain) {
    SetWorld(_p->GetWorld());

	int index = idx(pos);

	_frames[index] = _frame;

	bool walkable = false;
	bool transparent = true;
	if (terrain)
	{
		walkable = terrain->IsWalkable();
		transparent = terrain->IsTransparent();
	}

	if (actor)
	{
		DisplayChar ent_dc = actor->Disp();

		_actors[index] = ent_dc;

		_has_actor[index] = true;
	} else {
		_has_actor[index] = false;
	}
	
	if (terrain)
	{
		DisplayChar dc = terrain->Disp();
		_terrain[index] = dc;
	}
	else
	{
		_terrain[index] = s_void;
	}

	_tcod_map->setProperties(pos._x, pos._y, transparent, walkable);
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
			int index = idx(world_pos);

			if (_frames[index] > 0)
			{
				float fade = (200 - (_frame - _frames[index])) / 200.0f;
				fade = std::max(0.4f, fade);

				DisplayChar bg_dc = _terrain[index];
				console->putCharEx(i, screen_size._y - j - 1,
					bg_dc._char, bg_dc._fcol * fade, bg_dc._bcol * fade);

				if (_has_actor[index]) {
					DisplayChar a_dc = _actors[index];

					if (a_dc._use_bcol) {
						console->putCharEx(i, screen_size._y - j - 1,
							a_dc._char, a_dc._fcol * fade, a_dc._bcol * fade);
					} else {
						console->putCharEx(i, screen_size._y - j - 1,
							a_dc._char, a_dc._fcol * fade, bg_dc._fcol * fade);
					}
				}
			} else {
				console->putCharEx(i, screen_size._y - j - 1,
					s_void._char, s_void._fcol, s_void._bcol);
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
//			if (_tcod_map->isInFov(i, j))
			{
				Coord pos(i, j);
				const Terrain* t = _world->GetTerrain(pos);
				SetChars(pos, _world->GetActor(pos), t);
			}
		}
	}
}
