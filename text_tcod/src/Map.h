#pragma once

#include "Coord.h"
#include "World.h"
#include "DisplayChar.h"

#include "color.hpp"
#include "console.hpp"
#include "fov.hpp"

class Entity;
class Terrain;
class World;
class Player;

class Map {
public:
    Map() :
        _width(0),
        _height(0),
        _frame(0),
        _p(nullptr),
        _tcod_map(nullptr),
        _world(nullptr) {}

	~Map() {
		ClearWorld();
	}

	void SetPlayer(const Player* p) { _p = p; }

	void SetWorld(const World* world);

	void ClearWorld();
    
	void SetChars(Coord pos, const Actor* actor, const Terrain* terrain);

	int idx(Coord pos) const {
		return pos._x + pos._y * _width;
	}

	void NextFrame() { _frame++; }

	void Draw(TCODConsole* console);

private:
    void ReadWorld(Coord eye_pos);

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

	static DisplayChar s_void;
};