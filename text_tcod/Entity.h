#pragma once

#include <assert.h>

#include "Coord.h"

class World;
class DisplayChar;

enum class EntityType {
	Player,
	Wall,
	Floor
};

class Entity {
public:
	Entity(EntityType t) : _type(t), _pos(0, 0), _w(nullptr) {}

	virtual DisplayChar& Disp() const = 0;

	void SetPos(Coord pos) {
		_pos = pos;
	}
	EntityType GetType() const { return _type; }

	Coord GetPos() const { return _pos; }

	World* GetWorld() { return _w;  }

	void SetWorld(World* w) {
		assert(!w || !_w || w == _w);

		_w = w;
	}

private:
	EntityType _type;
	Coord _pos;
	World* _w;
};

class Terrain : public Entity {
public:
	Terrain(EntityType et, bool is_walkable, bool is_transparent) :
		Entity(et),
		_is_walkable(is_walkable),
		_is_transparent(is_transparent) {}

	bool IsWalkable() const { return _is_walkable; }
	bool IsTransparent() const { return _is_transparent; }

private:
	bool _is_walkable;
	bool _is_transparent;
};
