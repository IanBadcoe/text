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

	void SetPos(World* w, Coord pos) {
		assert(_w == nullptr);

		_pos = pos;
		_w = w;
	}
	void LeaveWorld() {
		assert(_w);
		_w = nullptr;
	}

	EntityType GetType() const { return _type; }

	Coord GetPos() const { return _pos; }

	World* GetWorld() { return _w; }
	const World* GetWorld() const { return _w; }

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

class Actor : public Entity {
public:
	Actor(EntityType et, float speed) :
		Entity(et), _speed(speed) {
	}

	void Step();
	virtual float InnerStep() = 0;

private:
	float _speed;
};
