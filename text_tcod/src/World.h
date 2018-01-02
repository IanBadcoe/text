#pragma once

#include "Terrain.h"

class World;
class Player;
class Actor;
class Universe;
class Base;
class Path;

struct Command;

class World {
public:
	World(Universe* u);

	~World();

	void SetSize(int width, int height);
	void Clear();

	// Inherited via ISerialisable
	void SerialiseTo(std::ostringstream& out) const;
	void SerialiseFrom(std::istringstream& in);

	Terrain* GetTerrain(Coord pos) {
		return _terrain[idx(pos)];
	}

	const Terrain* GetTerrain(Coord pos) const {
		return const_cast<World*>(this)->GetTerrain(pos);
	}

	Actor* GetActor(Coord pos) {
		return _actors[idx(pos)];
	}

	const Actor* GetActor(Coord pos) const {
		return _actors[idx(pos)];
	}

	void SetTerrain(Coord pos, Terrain* e);
	void ClearTerrain(Coord pos);
	void RecalcTerrainDisps();

	void AddActor(Coord pos, Actor* e);
	void RemoveActor(Coord pos);

	int idx(int i, int j) const {
		return i + j * _width;
	}

	int idx(Coord pos) const {
		return idx(pos._x, pos._y);
	}

	void ClearEntity(Coord pos);

	int GetWidth() const {
		return _width;
	}

	int GetHeight() const {
		return _height;
	}

	bool IsWalkable(Coord pos) {
		Terrain* t = _terrain[idx(pos)];

		return t ? t->IsWalkable() : false;
	}

	Coord ClampCoord(Coord c) const {
		return Coord(std::min(std::max(c._x, 0), _width),
			std::min(std::max(c._y, 0), _height));
	}

	bool InRange(Coord pos) {
		return pos._x >= 0 && pos._x < _width
			&& pos._y >= 0 && pos._y < _height;
	}

	bool ComputePath(Player* player, Coord from, Coord to, Path& output) const;

	// for debug facilities...
	bool ProcessWorldCommand(const Command& c);

	void EnsureBase(int base_id);
	Base* GetBase(int base_id);
	void RegisterBase(int base_id, Base* base);

	Universe* GetUniverse() { return _universe; }

private:
	bool TryBase(int i, Coord c);

	int _width;
	int _height;

	Terrain **_terrain;
	Actor **_actors;

	bool _dirty_terrain;

	// make this a collection of maps for different kinds of locomotion, e.g. Actors who can open doors or not, Actors who can fly or not
	TCODMap* _tcod_map;

	// for passing commands to...
	Universe* _universe;

	std::map<int, Base*> _base_map;
};
