#pragma once

#include "Coord.h"
#include "World.h"

#include "color.hpp"
#include "console.hpp"
#include "fov.hpp"

class Entity;
class Terrain;
class World;
class Player;

class DisplayChar {
public:
	DisplayChar(wchar_t c, TCODColor fcol, TCODColor bcol) : _char(c), _fcol(fcol), _bcol(bcol), _use_bcol(true) {}
	DisplayChar(wchar_t c, TCODColor fcol) : _char(c), _fcol(fcol), _use_bcol(false) {}

	wchar_t _char;
	TCODColor _fcol;
	TCODColor _bcol;
	bool _use_bcol;
};

class Map {
private:
	class MapChar {
	public:
		MapChar() : _dc(' ', TCOD_white, TCOD_black), _frame(0) {}
		MapChar(DisplayChar dc, int frame) : _dc(dc), _frame(frame) {}

		DisplayChar _dc;
		uint64_t _frame;
	};

public:
    Map(const Player* p) :
        _width(0),
        _height(0),
        _frame(0),
        _p(p),
        _tcod_map(nullptr),
        _world(nullptr)
    {
    }

	~Map() {
		delete[] _map;
        delete _tcod_map;
	}

    void SetWorld(const World* world)
    {
        if (world == _world)
            return;

        _world = world;

        ClearWorld();

		_width = _world->GetWidth();
		_height = _world->GetHeight();

		_map = new MapChar[_width * _height];
        _tcod_map = new TCODMap(_width, _height);

        for (int i = 0; i < _width; i++) {
            for (int j = 0; j < _height; j++) {
                _tcod_map->setProperties(i, j, false, false);
            }
        }
    }

    void ClearWorld() {
        delete _map;
        delete _tcod_map;

		_map = nullptr;
		_tcod_map = nullptr;

		_width = 0;
        _height = 0;

    }
    
    DisplayChar GetChar(Coord pos) const {
		return _map[idx(pos)]._dc;
	}

	void SetChar(Coord pos, const Entity* entity, const Terrain* terrain);

	int idx(Coord pos) const {
		return pos._x + pos._y * _width;
	}

	void NextFrame() { _frame++; }

	void Draw(TCODConsole* console);

private:
    void ReadWorld(Coord eye_pos);

    int _width;
	int _height;

	MapChar* _map;

	int _frame;

	const Player* _p;

	TCODMap* _tcod_map;

    const World* _world;

	static DisplayChar s_void;
};