#pragma once

#include "Coord.h"
#include "World.h"
#include "DisplayChar.h"
#include "Rect.h"

#include "color.hpp"
#include "console.hpp"
#include "fov.hpp"

class Entity;
class Terrain;
class World;
class Player;

class Map {
public:
	Map(Rect pos_in_console) :
		_width(0),
		_height(0),
		_frame(0),
		_p(nullptr),
		_tcod_map(nullptr),
		_world(nullptr),
		_pos_in_console(pos_in_console) {
	}

	~Map() {
		ClearWorld();
	}

	void SetPlayer(const Player* p) { _p = p; }

	void SetWorld(const World* world);
	void ClearWorld();

	void SetCell(Coord pos, const Actor* actor, const Terrain* terrain);

	int idx(Coord pos) const {
		return pos._x + pos._y * _width;
	}

	void NextFrame() { _frame++; }

	void Draw(TCODConsole* console);

	Coord MapToWorld(Coord pos) const {
		return pos + _last_map_to_world;
	}

private:
    void ReadWorld(Coord eye_pos);
	void SetMapToWorld(const Coord& pos) { _last_map_to_world = pos; }

    int _width;
	int _height;

	DisplayChar* _terrain;
	DisplayChar* _actors;
	int64_t* _frames;
	bool* _has_actor;

	int64_t _frame;

	const Player* _p;

	TCODMap* _tcod_map;

    const World* _world;

	// caching what we calculated from the player's position when we last drew
	Coord _last_map_to_world;

	Rect _pos_in_console;

	static DisplayChar s_void;
};